#include <Control_Surface.h> // Include the Control Surface library
 
// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;
 
// Instantiate a CCButton object
CCButton button = {
  // Push button on pin 5:
  2,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_1, CHANNEL_1},
};
 
void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}
 
void loop() {
  Control_Surface.loop(); // Update the Control Surface
}