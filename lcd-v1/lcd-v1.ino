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

/* Current sensing */

// Define analog input
#define ANALOG_IN_PIN A1

float volts = 0.00;


LiquidCrystal_I2C lcd0(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd1(0x26, 16, 2); // I2C address 0x27, 16 column and 2 rows

LiquidCrystal_I2C lcd[2] = {lcd0, lcd1};

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
//  Serial1.begin(115200);
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

  Control_Surface.begin();
  Control_Surface.setMIDIInputCallbacks(nullptr, sysExMessageCallback, nullptr, nullptr);


}

// Check the AC Current
float volt_check() {
  // Floats for ADC voltage & Input voltage
  float adc_voltage = 0.0;
  float in_voltage = 0.0;

  // Floats for resistor values in divider (in ohms)
  float R1 = 30000.0;
  float R2 = 7500.0;

  // Float for Reference Voltage
  float ref_voltage = 5.0;

  // Integer for ADC value
  int adc_value = 0;
  // Read the Analog Input
  adc_value = analogRead(ANALOG_IN_PIN);

  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 1024.0;

  // Calculate voltage at divider input
  in_voltage = adc_voltage / (R2 / (R1 + R2));
  return in_voltage;
}

void loop() {
  volts = volt_check();
  delay(100);
  if (volts < 3.0) {
    lcd[0].setBacklight(0);
    lcd[1].setBacklight(0);
  }
  else
  {
    lcd[0].setBacklight(255);
    lcd[1].setBacklight(255);
  }

  Control_Surface.loop();
}
