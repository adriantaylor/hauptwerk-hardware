/* Wyvern C356 Manuals Matrix Scanner
   Adrian Taylor 2013

   Rows {{ 9, 3, 12, 13, 2, 4, 11, 10 }};
   Cols {{ 14, 8, 15, 7, 16, 6, 17, 5 }};
   13, 3, 12, 4, 11, 5. 10, 6

   2, 8, A1, A0, 9, 7, A2, A3

  Rows (nano) {{ 2, 8, a1,	a0, 9, 7, a2,	a3 }}
   Cols (Nano) {{ 15, 3, 14, 4, 16, 5, 10, 6}}
  

   Based on code taken from here - 
   https://forum.arduino.cc/t/keyboard-piano-scanner-with-sensitivity-and-sustain-arduino-mega-2560/648586
*/

#include <MIDI.h>
#include <Keypad.h>

#define LIST_MAX 64  // You'll probably need to change this in libraries/Keypad/src/Keypad.h

/*
Choir = 0
Great = 1
Swell = 2
Pedals = 3
Pistons = 7

*/
byte channel = 2;  // Change this per manual
const byte ROWS = 8;
const byte COLS = 8;

char keys[ROWS][COLS] = {
  { 36, 37, 38, 39, 40, 41, 42, 43 },
  { 44, 45, 46, 47, 48, 49, 50, 51 },
  { 52, 53, 54, 55, 56, 57, 58, 59 },
  { 60, 61, 62, 63, 64, 65, 66, 67 },
  { 68, 69, 70, 71, 72, 73, 74, 75 },
  { 76, 77, 78, 79, 80, 81, 82, 83 },
  { 84, 85, 86, 87, 88, 89, 90, 91 },
  { 92, 93, 94, 95, 96, 97, 98, 99 },
};

byte rowPins[ROWS] = { 2, 8, A1, A0, 9, 7, A2, A3 };
byte colPins[COLS] = { 15, 3, 14, 4, 16, 5, 10, 6};


Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

byte pressed = 32;


void setup() {
    Serial1.begin(31250); // Serial MIDI
}

void loop() {
  if (kpd.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {
      if (kpd.key[i].stateChanged) {
        pressed = kpd.key[i].kchar;
        switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            sendMIDI(channel | 0x90, pressed, 100);
            break;
          case RELEASED:
            sendMIDI(channel | 0x80, pressed, 64);
            break;
        }
      }
    }
  }
}

void sendMIDI(byte type, byte note, byte velocity) {
  Serial1.write(type);
  Serial1.write(note & 0x7F);
  Serial1.write(velocity);
}