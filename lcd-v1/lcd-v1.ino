/**
    Simple Hauptwerk friendly LCD screen message printer - 
    receives SysEx from Hauptwerk and dislays on the LCD.

    Adrian Taylor, 2024
    
    Developed from:
        SysEx-Read-MAX7219-Message.ino
        Lowell Bahner 2020-02-15
        MIDI-Input-Callback Written by PieterP, 2019-08-07
        https://github.com/tttapa/Control-Surface
*/

#include <LiquidCrystal_I2C.h>
#include <Control_Surface.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

USBMIDI_Interface midi;

// function to display the SysEx message on LCD display
void update_lcd(uint8_t msg[]) {

  lcd.clear();
  // the sysex message starts with "f0 77 XX ...", start byte, manufacturer code, device code
  if (msg[1] != 0x7d) return; // check for desired manufacturer code
  
//  if (msg[2] == 0x10) lcd.print("SysEx from Device 10:"); // device code 10
//  if (msg[2] == 0x09) Serial.println("SysEx from Device 09:"); // device code 09

    char line[32]; // array for the number as char array  
    for (int i = 0; i <= 32; i++){
      line[i] = int8_t(msg[i + 5]);
    }
    
  
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    for (int i = 0; i <= 16; i++)
    {
      if (isprint(line[i]))
      {
        lcd.print(line[i]);
      }
      else
      {
        lcd.print(" ");
      }
    }
    lcd.setCursor(0,1);
    for (int i = 17; i <= 32; i++)
    {
      if (isprint(line[i]))
      {
        lcd.print(line[i]);
      }
      else
      {
        lcd.print(" ");
      }   
    }
}

// function to capture the incoming SysEx message
bool sysExMessageCallback(SysExMessage se) {
  update_lcd(se.data); // pass the data to MAX7219 function
  return true; // Return true to indicate that handling is done,
}


void setup() {
//  Serial.begin(31250);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();                 // clear display
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("Error: Hauptwerk");        // print message at (0, 0)
  lcd.setCursor(0, 1);         // move cursor to   (2, 1)
  lcd.print("not running?"); // print message at (2, 1)

  Control_Surface.begin();
  Control_Surface.setMIDIInputCallbacks(nullptr, sysExMessageCallback, nullptr, nullptr); 
}

void loop() {
  Control_Surface.loop();
}
