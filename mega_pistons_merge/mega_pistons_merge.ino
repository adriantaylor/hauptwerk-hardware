/* Setup MIDI merge */

/*
// Simple DIY Electronic Music Projects
//    diyelectromusic.wordpress.com
//
//  Arduino Multi MIDI Merge
//  https://diyelectromusic.wordpress.com/2021/12/05/arduino-multi-midi-merge/
//
      MIT License
      
      Copyright (c) 2020 diyelectromusic (Kevin)
      
      Permission is hereby granted, free of charge, to any person obtaining a copy of
      this software and associated documentation files (the "Software"), to deal in
      the Software without restriction, including without limitation the rights to
      use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
      the Software, and to permit persons to whom the Software is furnished to do so,
      subject to the following conditions:
      
      The above copyright notice and this permission notice shall be included in all
      copies or substantial portions of the Software.
      
      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
      FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
      COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHERIN
      AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*
  Using principles from the following tutorials:
    Arduino MIDI Library - https://github.com/FortySevenEffects/arduino_midi_library
    Arduino Software Serial Library - https://www.arduino.cc/en/Reference/softwareSerial
    USB Host 2.0 Library - https://github.com/felis/USB_Host_Shield_2.0
    Arduino USB Transport - https://github.com/lathoub/Arduino-USBMIDI
    Arduino UHS2MIDI Transport - https://github.com/YuuichiAkagawa/Arduino-UHS2MIDI
*/
#include <MIDI.h>

// Uncomment which of these should be included as active MIDI IN
// devices to be end points for the MIDI merging.
//
// Different boards will support different interfaces, for example:
//   Uno/Nano: HW_SERIAL, SW_SERIAL, SW_SERIAL2, USB_HOST (with additional shield).
//   Leonardo: USB_DEVICE, HW_SERIAL2, SW_SERIAL, SW_SERIAL2, USB_HOST (with additional shield).
//   Mega:     HW_SERIAL, HW_SERIAL2, HW_SERIAL3, HW_SERIAL4, SW_SERIAL, SW_SERIAL2, USB_HOST (with additional sheild).
//
#define MIDI_HW_SERIAL 1
// #define MIDI_HW_SERIAL2 2
// #define MIDI_HW_SERIAL3 3
// #define MIDI_HW_SERIAL4 4
//#define MIDI_SW_SERIAL 5
//#define MIDI_SW_SERIAL2 6
//#define MIDI_USB_HOST 7
//#define MIDI_USB_DEVICE 8
#define MIDI_LEDS        8  // Set to last possible MIDI device defined

#define MIDI_OUT MIDI_HW_SERIAL

// Optional: Set pins for LED indicators for each of the above
//           Set to NOPIN to disable that port/pin
#define NOPIN (-1)
int ledPins[MIDI_LEDS] = { NOPIN, NOPIN, NOPIN, NOPIN, NOPIN, NOPIN, NOPIN, NOPIN };
//int ledPins[MIDI_LEDS] = { 2, 3, 4, 5, 6, 7, 8, 9 };
#define LED_COUNT 200
int ledCnt[MIDI_LEDS];


// ---- Definitions for MIDI INPUT devices ----
//
#ifdef MIDI_HW_SERIAL
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI_HS);
#endif

#ifdef MIDI_HW_SERIAL2
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI_HS2);
#endif

#ifdef MIDI_HW_SERIAL3
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI_HS3);
#endif

#ifdef MIDI_HW_SERIAL4
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI_HS4);
#endif

#ifdef MIDI_SW_SERIAL
#include <SoftwareSerial.h>
// From https://www.arduino.cc/en/Reference/softwareSerial
// This default configurationis not supported on ATmega32U4
// or ATmega2560 based boards, see MIDI_SW_SERIAL2.
#define SS_RX  2
#define SS_TX  3
using Transport = MIDI_NAMESPACE::SerialMIDI<SoftwareSerial>;
SoftwareSerial sSerial = SoftwareSerial(SS_RX, SS_TX);
Transport serialMIDI(sSerial);
MIDI_NAMESPACE::MidiInterface<Transport> MIDI_SS((Transport&)serialMIDI);
#endif

#ifdef MIDI_SW_SERIAL2
#include <SoftwareSerial.h>
// From https://www.arduino.cc/en/Reference/softwareSerial
// there are limitations on which pins can be used with a
// Mega 2560 or Leonardo/Micro (ATmega32U4), so be sure to
// check.  There are no pin limitations for the Uno.
// These are fine for the ATmega32U4 based boards.
#define SS2_RX  10
#define SS2_TX  11
using Transport = MIDI_NAMESPACE::SerialMIDI<SoftwareSerial>;
SoftwareSerial sSerial2 = SoftwareSerial(SS2_RX, SS2_TX);
Transport serialMIDI2(sSerial2);
MIDI_NAMESPACE::MidiInterface<Transport> MIDI_SS2((Transport&)serialMIDI2);
#endif

#ifdef MIDI_USB_HOST
#include <UHS2-MIDI.h>
USB Usb;
UHS2MIDI_CREATE_INSTANCE(&Usb, 0, MIDI_UH);
#endif

#ifdef MIDI_USB_DEVICE
#include <USB-MIDI.h>
USBMIDI_CREATE_INSTANCE(0, MIDI_UD);
#endif


// ---- Definitions for MIDI OUTPUT device ----
//
#if (MIDI_OUT==MIDI_HW_SERIAL)
#define MIDI_O MIDI_HS
#elif (MIDI_OUT==MIDI_HW_SERIAL2)
#define MIDI_O MIDI_HS2
#elif (MIDI_OUT==MIDI_HW_SERIAL3)
#define MIDI_O MIDI_HS3
#elif (MIDI_OUT==MIDI_HW_SERIAL4)
#define MIDI_O MIDI_HS4
#elif (MIDI_OUT==MIDI_SW_SERIAL)
#define MIDI_O MIDI_SS
#elif (MIDI_OUT==MIDI_SW_SERIAL2)
#define MIDI_O MIDI_SS2
#elif (MIDI_OUT==MIDI_USB_HOST)
#define MIDI_O MIDI_UH
#elif (MIDI_OUT==MIDI_USB_DEVICE)
#define MIDI_O MIDI_UD
#else // No output!?
#endif

// NB: led goes from 1 to max so has to be changed
//     to go from 0 to max-1 to use as an index.
void ledOn (int led) {
  led = led-1;
  if (led < MIDI_LEDS) {
    if (ledPins[led] != NOPIN) {
      digitalWrite(ledPins[led], HIGH);
      ledCnt[led] = LED_COUNT;
    }
  }
}

void ledOff (int led) {
  led = led-1;
  if (led < MIDI_LEDS) {
    if (ledPins[led] != NOPIN) {
      digitalWrite(ledPins[led], LOW);
    }
  }
}

void ledInit () {
  for (int i=0; i<MIDI_LEDS; i++) {
    if (ledPins[i] != NOPIN) {
      pinMode(ledPins[i], OUTPUT);
      digitalWrite(ledPins[i], LOW);
      ledCnt[i] = 0;
    }
  }
}

void ledScan () {
  for (int i=0; i<MIDI_LEDS; i++) {
    if (ledCnt[i] > 0) {
      ledCnt[i]--;
    } else {
      ledOff(i+1);
      ledCnt[i] = 0;
    }
  }
}

/* MIDI keyboard bit */
MIDI_CREATE_DEFAULT_INSTANCE(); 


// BUTTONS
const int NButtons = 45; //***  total number of push buttons
const int buttonPin[NButtons] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46};

// const int NButtons = 2; //***  total number of push buttons
// const int buttonPin[NButtons] = {12, 13};

//*** define digital pins connected from button to Arduino; (ie {10, 16, 14, 15, 6, 7, 8, 9, 2, 3, 4, 5}; 12 buttons)
                                        //** Button NOTE will go up chromatically.  ie. if button is digi pin 2, C; Pin 3, C#; Pin 3, D; etc
                                   
int buttonCState[NButtons] = {};        // stores the button current value
int buttonPState[NButtons] = {};        // stores the button previous value


// debounce
unsigned long lastDebounceTime[NButtons] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    //* the debounce time; increase if the output flickers

// POTENTIOMETERS
const int NPots = 0; //*** total numbers of pots (slide & rotary)
const int potPin[NPots] = {}; //*** Analog pins of each pot connected straight to the Arduino i.e 4 pots, {A3, A2, A1, A0};
                                          // have nothing in the array if 0 pots {}

int potCState[NPots] = {}; // Current state of the pot; delete 0 if 0 pots
int potPState[NPots] = {}; // Previous state of the pot; delete 0 if 0 pots
int potVar = 0; // Difference between the current and previous state of the pot

int midiCState[NPots] = {}; // Current state of the midi value; delete 0 if 0 pots
int midiPState[NPots] = {}; // Previous state of the midi value; delete 0 if 0 pots

const int TIMEOUT = 300; //** Amount of time the potentiometer will be read after it exceeds the varThreshold
const int varThreshold = 10; //** Threshold for the potentiometer signal variation
boolean potMoving = true; // If the potentiometer is moving
unsigned long PTime[NPots] = {}; // Previously stored time; delete 0 if 0 pots
unsigned long timer[NPots] = {}; // Stores the time that has elapsed since the timer was reset; delete 0 if 0 pots


// MIDI
byte midiCh = 6; //** MIDI channel to be used; You can add more if you need to reorganize or have a billion buttons/pots
byte note = 1; //** First note to be used for digital buttons, then go up chromatically in scale according to the sequence in your "buttonPin" array
                // you can look up on a Midi Note chart; 36=C2; 60=Middle C
byte cc = 1; //** First MIDI CC to be used for pots on Analog Pins in order of the "potPin" array; then goes up by 1


void setup()
{

  Serial.begin(115200); //**  Baud Rate 31250 for MIDI class compliant jack | 115200 for Hairless MIDI
  // Buttons
  // Initialize buttons with pull up resistors
  for (int i = 0; i < NButtons; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }

  ledInit();

  // Disable the THRU for all devices.  In principle we could keep
  // it on for the OUTPUT device to automatically pass data coming in
  // on that device straight out, but in reality it makes for simpler
  // code to disable all automatic THRU and handle it all here.
  //
  // Initialise all INPUT devices to listen on all channels.
  //
#ifdef MIDI_HW_SERIAL
  ledOn(MIDI_HW_SERIAL);
  MIDI_HS.begin(MIDI_CHANNEL_OMNI);
  MIDI_HS.turnThruOff();
#endif
#ifdef MIDI_HW_SERIAL2
  ledOn(MIDI_HW_SERIAL2);
  MIDI_HS2.begin(MIDI_CHANNEL_OMNI);
  MIDI_HS2.turnThruOff();
#endif
#ifdef MIDI_HW_SERIAL3
  ledOn(MIDI_HW_SERIAL3);
  MIDI_HS3.begin(MIDI_CHANNEL_OMNI);
  MIDI_HS3.turnThruOff();
#endif
#ifdef MIDI_HW_SERIAL4
  ledOn(MIDI_HW_SERIAL4);
  MIDI_HS4.begin(MIDI_CHANNEL_OMNI);
  MIDI_HS4.turnThruOff();
#endif
#ifdef MIDI_SW_SERIAL
  ledOn(MIDI_SW_SERIAL);
  MIDI_SS.begin(MIDI_CHANNEL_OMNI);
  MIDI_SS.turnThruOff();
#endif
#ifdef MIDI_SW_SERIAL2
  ledOn(MIDI_SW_SERIAL2);
  MIDI_SS2.begin(MIDI_CHANNEL_OMNI);
  MIDI_SS2.turnThruOff();
#endif
#ifdef MIDI_USB_HOST
  ledOn(MIDI_USB_HOST);
  MIDI_UH.begin(MIDI_CHANNEL_OMNI);
  MIDI_UH.turnThruOff();
  Usb.Init();  
#endif
#ifdef MIDI_USB_DEVICE
  ledOn(MIDI_USB_DEVICE);
  MIDI_UD.begin(MIDI_CHANNEL_OMNI);
  MIDI_UD.turnThruOff();
#endif
}

void loop()
{
  buttons();
  // potentiometers();
#ifdef MIDI_USB_HOST
  // Usb.Task();
#endif

  // // See if any LEDs need turning off
  // ledScan();

#ifdef MIDI_HW_SERIAL
  if (MIDI_HS.read()) {
    ledOn(MIDI_HW_SERIAL);
    midi::MidiType mt  = MIDI_HS.getType();
    if (mt != midi::SystemExclusive) {
      MIDI_O.send(mt, MIDI_HS.getData1(), MIDI_HS.getData2(), MIDI_HS.getChannel());
    } else {
      int mSxLen = MIDI_HS.getData1() + 256*MIDI_HS.getData2();
      MIDI_O.sendSysEx(mSxLen, MIDI_HS.getSysExArray(), true);
    }
  }
#endif
#ifdef MIDI_HW_SERIAL2
  if (MIDI_HS2.read()) {
    ledOn(MIDI_HW_SERIAL2);
    midi::MidiType mt  = MIDI_HS2.getType();
    if (mt != midi::SystemExclusive) {
      MIDI_O.send(mt, MIDI_HS2.getData1(), MIDI_HS2.getData2(), MIDI_HS2.getChannel());
    } else {
      int mSxLen = MIDI_HS2.getData1() + 256*MIDI_HS2.getData2();
      MIDI_O.sendSysEx(mSxLen, MIDI_HS2.getSysExArray(), true);
    }
  }
#endif
#ifdef MIDI_HW_SERIAL3
  if (MIDI_HS3.read()) {
    ledOn(MIDI_HW_SERIAL3);
    midi::MidiType mt  = MIDI_HS3.getType();
    if (mt != midi::SystemExclusive) {
      MIDI_O.send(mt, MIDI_HS3.getData1(), MIDI_HS3.getData2(), MIDI_HS3.getChannel());
    } else {
      int mSxLen = MIDI_HS3.getData1() + 256*MIDI_HS3.getData2();
      MIDI_O.sendSysEx(mSxLen, MIDI_HS3.getSysExArray(), true);
    }
  }
#endif
#ifdef MIDI_HW_SERIAL4
  if (MIDI_HS4.read()) {
    ledOn(MIDI_HW_SERIAL4);
    midi::MidiType mt  = MIDI_HS4.getType();
    if (mt != midi::SystemExclusive) {
      MIDI_O.send(mt, MIDI_HS4.getData1(), MIDI_HS4.getData2(), MIDI_HS4.getChannel());
    } else {
      int mSxLen = MIDI_HS4.getData1() + 256*MIDI_HS4.getData2();
      MIDI_O.sendSysEx(mSxLen, MIDI_HS4.getSysExArray(), true);
    }
  }
#endif
#ifdef MIDI_SW_SERIAL
  if (MIDI_SS.read()) {
    ledOn(MIDI_SW_SERIAL);
    midi::MidiType mt  = MIDI_SS.getType();
    if (mt != midi::SystemExclusive) {
      MIDI_O.send(mt, MIDI_SS.getData1(), MIDI_SS.getData2(), MIDI_SS.getChannel());
    } else {
      int mSxLen = MIDI_SS.getData1() + 256*MIDI_SS.getData2();
      MIDI_O.sendSysEx(mSxLen, MIDI_SS.getSysExArray(), true);
    }
  }
#endif
#ifdef MIDI_SW_SERIAL2
  if (MIDI_SS2.read()) {
    ledOn(MIDI_SW_SERIAL2);
    midi::MidiType mt  = MIDI_SS2.getType();
    if (mt != midi::SystemExclusive) {
      MIDI_O.send(mt, MIDI_SS2.getData1(), MIDI_SS2.getData2(), MIDI_SS2.getChannel());
    } else {
      int mSxLen = MIDI_SS2.getData1() + 256*MIDI_SS2.getData2();
      MIDI_O.sendSysEx(mSxLen, MIDI_SS2.getSysExArray(), true);
    }
  }
#endif
#ifdef MIDI_USB_HOST
  if (MIDI_UH.read()) {
    ledOn(MIDI_USB_HOST);
    midi::MidiType mt  = MIDI_UH.getType();
    if (mt != midi::SystemExclusive) {
      MIDI_O.send(mt, MIDI_UH.getData1(), MIDI_UH.getData2(), MIDI_UH.getChannel());
    } else {
      int mSxLen = MIDI_UH.getData1() + 256*MIDI_UH.getData2();
      MIDI_O.sendSysEx(mSxLen, MIDI_UH.getSysExArray(), true);
    }
  }
#endif
#ifdef MIDI_USB_DEVICE
  if (MIDI_UD.read()) {
    ledOn(MIDI_USB_DEVICE);
    midi::MidiType mt  = MIDI_UD.getType();
    if (mt != midi::SystemExclusive) {
      MIDI_O.send(mt, MIDI_UD.getData1(), MIDI_UD.getData2(), MIDI_UD.getChannel());
    } else {
      int mSxLen = MIDI_UD.getData1() + 256*MIDI_UD.getData2();
      MIDI_O.sendSysEx(mSxLen, MIDI_UD.getSysExArray(), true);
    }
  }
#endif
}

// BUTTONS
void buttons() {

  for (int i = 0; i < NButtons; i++) {
  

    buttonCState[i] = digitalRead(buttonPin[i]);  // read pins from arduino

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {

      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();

          if (buttonCState[i] == LOW) {
            Serial.println(i);
            Serial.println(midiCh);
            MIDI.sendProgramChange(i, midiCh); // note, velocity, channel
          }

        buttonPState[i] = buttonCState[i];
      }
    }
  }
}



// POTENTIOMETERS
void potentiometers() {


  for (int i = 0; i < NPots; i++) { // Loops through all the potentiometers

    potCState[i] = analogRead(potPin[i]); // reads the pins from arduino

    midiCState[i] = map(potCState[i], 0, 1023, 0, 127); // Maps the reading of the potCState to a value usable in midi

    potVar = abs(potCState[i] - potPState[i]); // Calculates the absolute value between the difference between the current and previous state of the pot

    if (potVar > varThreshold) { // Opens the gate if the potentiometer variation is greater than the threshold
      PTime[i] = millis(); // Stores the previous time
    }

    timer[i] = millis() - PTime[i]; // Resets the timer 11000 - 11000 = 0ms

    if (timer[i] < TIMEOUT) { // If the timer is less than the maximum allowed time it means that the potentiometer is still moving
      potMoving = true;
    }
    else {
      potMoving = false;
    }

    if (potMoving == true) { // If the potentiometer is still moving, send the change control
      if (midiPState[i] != midiCState[i]) {

        // Sends the MIDI CC 
        MIDI.sendControlChange(cc + i, midiCState[i], midiCh); // cc number, cc value, midi channel
            

        potPState[i] = potCState[i]; // Stores the current reading of the potentiometer to compare with the next
        midiPState[i] = midiCState[i];
      }
    }
  }
}
