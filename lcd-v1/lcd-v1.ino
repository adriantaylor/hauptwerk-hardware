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

LiquidCrystal_I2C lcd0(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd1(0x26, 16, 2); // I2C address 0x27, 16 column and 2 rows

LiquidCrystal_I2C lcd[2] = {lcd0,lcd1};

USBMIDI_Interface midi;

// function to display the SysEx message on LCD display
void update_lcd(uint8_t msg[]) {
  // If this isn't from Hauptwerk, ignore
  if (msg[1] != 0x7d) return; 
  // send to LCD 0 by default
  int dest = 0;
  // But if we get asked to send to the 2nd LCD, do so
  if (msg[3] == 0x1)
    dest = 1;

  // Grab the text string
  char line[32]; 
  for (int i = 0; i <= 32; i++) {
    line[i] = int8_t(msg[i + 6]);
  }

  // Setup the screen
  lcd[dest].clear();
  lcd[dest].home();
  lcd[dest].setCursor(0, 0);
  
  // Output Line 1 
  for (int i = 0; i <= 15; i++)
  {
    // Skip over non-ascii chars
    if (isprint(line[i]))
    {
      lcd[dest].print(line[i]);
    }
    else
    {
      lcd[dest].print(" ");
    }
  }
  
  // Output Line 2
  lcd[dest].setCursor(0, 1);
  for (int i = 16; i <= 32; i++)
  {
    if (isprint(line[i]))
    {
      lcd[dest].print(line[i]);
    }
    else
    {
      lcd[dest].print(" ");
    }
  }
}

// function to capture the incoming SysEx message
bool sysExMessageCallback(SysExMessage se) {
  update_lcd(se.data); 
  return true; // Return true to indicate that handling is done,
}


void setup() {
  //  Serial.begin(31250);
  
  // Initialise Screens
  for (int i = 0; i < 2; i++)
  {
    lcd[i].init(); // initialize the lcd
    lcd[i].backlight();
//    Would be great if this was software configurable - needs a resistor between pins 15-16 to reduce backlight
//    lcd[i].setBacklight(1);
    lcd[i].clear();                 // clear display
    lcd[i].setCursor(0, 0);         // move cursor to   (0, 0)
    lcd[i].print("Screen: ");
    lcd[i].print(i);
  }

// Add in power detection
  Control_Surface.begin();
  Control_Surface.setMIDIInputCallbacks(nullptr, sysExMessageCallback, nullptr, nullptr);
}

void loop() {
  Control_Surface.loop();
}
