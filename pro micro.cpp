#include <Joystick.h>
Joystick_ Joystick;

const int buttoncount = 9; // amount of pins in use
const unsigned int buttonpins[] = {2, 3, 4, 5, 6, 8, 9, 10, 16}; // pins being used (first 5 MUST be GRYBO)

unsigned int buttonstate[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int prevbuttonstate[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

const int hz = 1000; // setting for dequeue poll rate

int queueinput[100];
int queueactutate[100];
int front = -1; 
int rear = -1;
int queuecount = 0;

unsigned long time;

void setup() {
  for (int i = 0; i < buttoncount; i++)
    pinMode(buttonpins[i], INPUT_PULLUP);

  Joystick.begin(false);
  time = micros();
}

void addqueue(int item1, int item2) {
  if (rear == 100 - 1) {
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
  }
  else {
    front++;
    queuecount--;
    if (front > rear)
      front = rear = -1;
  }
}

void loop() {
  if (queuecount < 100) {
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
  
  int pollrate = 1.0 / hz * 1000000.0;
  unsigned long newTime = micros();
  if (newTime - time >= pollrate) {
    if (queuecount) {
      Joystick.setButton(queueinput[front], queueactutate[front]);
      Joystick.sendState();
      dequeue();
    }

    for (int i = 0; i < 4; i++) {
      buttonstate[i + 5] = digitalRead(buttonpins[i + 5]);
      if (buttonstate[i + 5] != prevbuttonstate[i + 5]) {
        if (buttonstate[i + 5] == LOW) {
          Joystick.setButton(buttonpins[i + 5], 1);
          Joystick.sendState();
        }
        else {
          Joystick.setButton(buttonpins[i + 5], 0);
          Joystick.sendState();
        }
        prevbuttonstate[i + 5] = buttonstate[i + 5];
      }
    }
    time = newTime;
  }

  int w = analogRead(A0);
  w = map(w, 1023, 0, 1023, 0);
  Joystick.setRxAxis(w);
  
  if (queuecount >= 99) {
    rear = - 1;
    front = - 1;
    queuecount = 0;
  }
}
