#include <Control_Surface.h> // Include the Control Surface library
 
// Instantiate the two MIDI interfaces
USBMIDI_Interface midiusb;
// Select the serial port to use.
auto &serial = Serial1;
// Instantiate a Serial MIDI interface at the default MIDI baud rate.
SerialMIDI_Interface<decltype(serial)> midiserial {serial, 31250};
// Instantiate a MIDI pipe to connect the two interfaces
BidirectionalMIDI_Pipe mpipe;

void setup() {
  // Create a bidirectional route between the interfaces: 
  // all input to one interface is sent to the output of the other
  midiserial | mpipe | midiusb;
  // Initialize all MIDI interfaces
  MIDI_Interface::beginAll();
}
 
void loop() {
  // Continuously handle MIDI input
  MIDI_Interface::updateAll();
}
