#include <Control_Surface.h> // Include the Control Surface library
 
// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

CCButton buttonA0 {
  // Push button on pin 5:
  A0,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_1, CHANNEL_1},
};

CCButton buttonA1 {
  // Push button on pin 5:
  A1,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_1, CHANNEL_2},
};

CCButton buttonA2 {
  // Push button on pin 5:
  A2,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_1, CHANNEL_3},
};

CCButton buttonA3 {
  // Push button on pin 5:
  A3,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_1, CHANNEL_4},
};

CCButton buttonA4 {
  // Push button on pin 5:
  A3,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_1, CHANNEL_4},
};

CCButton button2 {
  // Push button on pin 5:
  2,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_2, CHANNEL_1},
};

CCButton button3 {
  // Push button on pin 5:
  3,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_2, CHANNEL_2},
};

CCButton button4 {
  // Push button on pin 5:
  4,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_2, CHANNEL_3},
};

CCButton button5 {
  // Push button on pin 5:
  5,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_2, CHANNEL_4},
};

CCButton button6 {
  // Push button on pin 5:
  6,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_2, CHANNEL_5},
};

CCButton button7 {
  // Push button on pin 5:
  7,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_2, CHANNEL_6},
};

CCButton button8 {
  // Push button on pin 5:
  8,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_2, CHANNEL_7},
};

CCButton button9 {
  // Push button on pin 5:
  9,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_3, CHANNEL_1},
};

CCButton button10 {
  // Push button on pin 5:
  10,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_3, CHANNEL_2},
};

CCButton button16 {
  // Push button on pin 5:
  16,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_3, CHANNEL_3},
};

CCButton button14 {
  // Push button on pin 5:
14,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_3, CHANNEL_4},
};

 
void setup() {
  Control_Surface.begin();
}
 
void loop() {
  Control_Surface.loop();
}