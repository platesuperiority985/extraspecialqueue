This is a repo for guitar hero/clone hero guitar code.

yes this is what me(mari) and ornej have been using 

many people have asked for and complained about this code basically saying that its cheating blah blah blah.. i present this shitty code to you to show its not cheating and its not this magical game changing thing some of you believe it to be.
anyways moving on to how to use this cause thats what you all want right?

DISCLAIMER!!! 
this is not a good tutorial cause i just cant be bothered but it should be good enough i think and if not just contact me i guess..
discord: marikogh

first things first you need yourself a arduinotar with either a pi pico or pro micro.
now that weve got that we need to download the arduino ide software so we can upload the code to the guitar.

im going to skip the part where you plug the guitar in and choose it in ide cause its self explanatory.

if you used guitar configurator prior you will need to go back on it and restore your board back to normal

alright so now that we have ide setup and the guitar its time to grab the code. 

theres some things we need to change in the code 

toward the top you will see 

const int buttoncount = 9; // amount of pins in use

const unsigned int buttonpins[] = {2, 3, 4, 5, 6, 8, 9, 10, 16}; // pins being used (first 5 MUST be GRYBO)

these need to be changed for your guitar so figure out what pins you have buttons soldered to and add them in here.
first 5 MUST be GRYBO so lets say my guitars frets were pins 4, 5, 1, 3, 2 and my strum up/down were 6, 8 and start select 9, 10
i would put 

const unsigned int buttonpins[] = {4, 5, 1, 3, 2, 6, 8, 9, 10}

understand? i hope so. if you have less like for example you dont care for start select or sum idk then you can remove them by just not adding those pins in like this 

const unsigned int buttonpins[] = {4, 5, 1, 3, 2, 6, 8};

you will also need to change the the other variable too so since we removed 2 pins we only have 7

const int buttoncount = 7;

you shouldnt need to do this but its so you get the idea if you do need to do this. 

now last thing would be whammy. some may not even have this but if you do its down toward the bottom and looks something like 

int w = analogRead(A0);

what you would change here is the A0 and you just change it to the pin you have whammy on.

another thing to note is the hz setting by default its 1000 in the code. 
if you are on pro micro you will be stuck at 1000 but if you are on pi pico you could run higher (typically u want it around ur fps)


depending on the board it will be different so if you use pro micro this is easy and for pico theres a extra step.

for pro micro you will need the joystick.h file but for pico you wont

for pro micro you should already have the joystick library installed.
now you just copy paste the code for pro micro into ide and upload to guitar.

for pi pico you will need to download the PicoGamepad library listed in this repo (creds to the guy who made it i forget where but yea sorry)
you need to put this inside "IDE-LOCATION/libraries/" (depending on ide version u might have to add it manually in another way or folder location but yea)
once that is done you should be able to copy paste the code into ide and then upload to guitar.
