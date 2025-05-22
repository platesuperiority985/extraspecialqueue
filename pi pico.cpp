/*
if you see "// insert comment here" this is a area you can change things for your liking/controller setup
*/

#include <PicoGamepad.h>
PicoGamepad gamepad;

const int buttoncount = 9; // amount of pins in use
const unsigned int buttonpins[] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // pins being used (first 5 MUST be GRYBO)

unsigned int buttonstate[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int prevbuttonstate[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

const int hz = 4000; // setting for dequeue poll rate (hz)
const int debounce = 0; // setting for debounce (ms)

unsigned long pollrate = 1.0 / hz * 1000000.0;

int queueinput[200];
int queueactutate[200];
int front = -1;
int rear = -1;
int queuecount = 0;

unsigned long time2;

void setup() {
  for (int i = 0; i < buttoncount; i++)
    pinMode(buttonpins[i], INPUT_PULLUP);

  randomSeed(analogRead(27)); // initialize RNG for overtap randomness

  time2 = micros();
}

void addqueue(int item1, int item2) {
  if (rear == 200 - 1) {
    // queue full, do nothing
  }
  else {
    if (front == -1)
      front = 0;
    rear++;
    queueinput[rear] = item1;
    queueactutate[rear] = item2;
    queuecount++;
  }
}

void dequeue() {
  if (front == -1) {
    // queue empty, do nothing
  }
  else {
    front++;
    queuecount--;
    if (front > rear)
      front = rear = -1;
  }
}

void loop() {
  if (queuecount < 200) {
    for (int i = 0; i < 5; i++) {
      buttonstate[i] = digitalRead(buttonpins[i]);
      if (buttonstate[i] != prevbuttonstate[i]) {
        if (buttonstate[i] == LOW) {
          addqueue(buttonpins[i], 1);
        }
        else {
          addqueue(buttonpins[i], 0);
        }
        prevbuttonstate[i] = buttonstate[i];
      }
    }
  }

  unsigned long newTime = micros();
  if (newTime - time2 >= pollrate) {
    if (queuecount) {
      int pin = queueinput[front];
      int state = queueactutate[front];

      gamepad.SetButton(pin, state);
      gamepad.send_update();
      dequeue();

      // --------- OVERTAP SECTION START ---------
      if (queuecount > 65 && state == 1) { // only overtap on press
        int chance = random(0, 100); // 0-99
        if (chance < 60) { // 60% chance to overtap
          int overtaps = random(2, 8); // 2-7 extra presses
          for (int i = 0; i < overtaps; i++) {
            delayMicroseconds(400); // small delay between overtaps
            addqueue(pin, 0); // release
            addqueue(pin, 1); // press again
          }
        }
      }
      // --------- OVERTAP SECTION END ---------
    }

    for (int i = 0; i < 4; i++) {
      buttonstate[i + 5] = digitalRead(buttonpins[i + 5]);
      if (buttonstate[i + 5] != prevbuttonstate[i + 5]) {
        if (buttonstate[i + 5] == LOW) {
          gamepad.SetButton(buttonpins[i + 5], 1);
          gamepad.send_update();
        }
        else {
          gamepad.SetButton(buttonpins[i + 5], 0);
          gamepad.send_update();
        }
        prevbuttonstate[i + 5] = buttonstate[i + 5];
      }
    }

    time2 = newTime;
  } else delayMicroseconds(debounce);

  if (queuecount >= 199) {
    rear = -1;
    front = -1;
    queuecount = 0;
  }
}
