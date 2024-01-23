#include <Control_Surface.h> // Include the Control Surface library
 
// Instantiate the two MIDI interfaces
auto &serial = Serial;
// Instantiate a Serial MIDI interface at the default MIDI baud rate.
SerialMIDI_Interface<decltype(serial)> midiserial {serial, 115200};
// Select the serial port to use.

auto &serial1 = Serial1;
// Instantiate a Serial MIDI interface at the default MIDI baud rate.
SerialMIDI_Interface<decltype(serial1)> midiserial1 {serial1, 31250};
// Instantiate a MIDI pipe to connect the two interfaces
BidirectionalMIDI_Pipe mpipe;

void setup() {
  // Create a bidirectional route between the interfaces: 
  // all input to one interface is sent to the output of the other
  midiserial1 | mpipe | midiserial;
  // Initialize all MIDI interfaces
  MIDI_Interface::beginAll();
}
 
void loop() {
  // Continuously handle MIDI input
  MIDI_Interface::updateAll();
}
