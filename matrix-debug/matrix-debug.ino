// Rows {{ 9, 3, 12, 13, 2, 4, 11, 10 }};
// Cols{{ 14, 8, 15, 7, 16, 6, 17, 5 }};
#include <MIDI.h>
#include <Keypad.h>

#define LIST_MAX 64

const byte ROWS = 8; // 6 Linhas
const byte COLS = 8; // 11 Colunas
 
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
 
byte rowPins[ROWS] = {9, 3, 12, 13, 2, 4, 11, 10}; 
byte colPins[COLS] = {A0, 8, A1, 7, A2, 6, A3, 5}; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

byte pressed = 32;
byte chanel = 0;

void setup() {
 Serial.begin(115200); // definir isso o mesmo que no Hairless
}


void loop() {
 // Fills kpd.key[49] array with up-to 10 active keys.
 // Returns true if there are ANY active keys.
 if (kpd.getKeys())
 {
   for (int i = 0; i < LIST_MAX; i++) // Scan the whole key list.
   {
     if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
     {
       pressed = kpd.key[i].kchar;
       switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
         case PRESSED:
           sendMIDI(chanel | 0x90, pressed, 100);
           break;
          case RELEASED:
           sendMIDI(chanel | 0x80, pressed, 64);
           break;
       }
     }
   }
 }
}  // End loop

void sendMIDI(byte type, byte note, byte velocity){
  Serial.write(type);
  Serial.write(note & 0x7F);
  Serial.write(velocity);
}