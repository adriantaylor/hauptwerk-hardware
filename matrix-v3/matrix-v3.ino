 
#include <Control_Surface.h>
 
#include <AH/STL/bitset>
 
// Pick one MIDI interface to use:
USBMIDI_Interface midi; // baud rate, use for easy debugging
// USBMIDI_Interface midi; // Use for final sketch with MIDI firmware
// HairlessMIDI_Interface midi; // Use for Hairless Serial <-> MIDI bridge
 
// Dimensions of the keyboard matrix
constexpr uint8_t rows = 8;
constexpr uint8_t columns = 8;
 
// The note numbers corresponding to the buttons in the matrix
const AddressMatrix<rows, columns> notes{ {
  { 36, 37, 38, 39, 40, 41, 42, 43 },
  { 44, 45, 46, 47, 48, 49, 50, 51 },
  { 52, 53, 54, 55, 56, 57, 58, 59 },
  { 60, 61, 62, 63, 64, 65, 66, 67 },
  { 68, 69, 70, 71, 72, 73, 74, 75 },
  { 76, 77, 78, 79, 80, 81, 82, 83 },
  { 84, 85, 86, 87, 88, 89, 90, 91 },
  { 92, 93, 94, 95, 96, 97, 98, 99 },
} };
// The MIDI channel to use
const Channel channel = Channel_1;
 
// Address pins A, B, C & D of TC4028
const AH::PinList<8> addressPins {{ 9, 3, 12, 13, 2, 4, 11, 10 }};
// The pins connected to the columns of the keyboard matrix (with external
// pull-down resistors)
const AH::PinList<columns> columnPins {{ 14, 8, 15, 7, 16, 6, 17, 5 }};
 
// Make the given line [0, 9] of the TC4028 high, the nine others low
void selectLine(uint8_t line) {
  uint8_t mask = 1;
  for (pin_t addressPin : addressPins) {
    digitalWrite(addressPin, (line & mask) != 0 ? HIGH : LOW);
    mask <<= 1;
  }
}
 
void setup() {
  for (pin_t addressPin : addressPins)
    pinMode(addressPin, OUTPUT);
  for (pin_t columnPin : columnPins)
    pinMode(columnPin, INPUT);
 
  midi.begin();
}
 
// Array of bits to remember previous switch states for state change detection.
std::bitset<rows * columns> previousStates;
 
void loop() {
  for (uint8_t row = 0; row < rows; ++row) {
    selectLine(row);
    for (uint8_t col = 0; col < columns; ++col) {
      bool newState = digitalRead(columnPins[col]);
      uint8_t index = row * columns + col; // index in the bit array
      // If the state of the switch changed
      if (newState != previousStates[index]) {
        // Send a MIDI note message
        // (note on message with zero velocity == note off)
        uint8_t velocity = newState * 0x7F;
        midi.sendNoteOn({notes[row][col], channel}, velocity);
        // Remember this new state
        previousStates[index] = newState;
        // TODO: add debouncing
      }
    }
  }
  midi.update(); // read and discard MIDI input
}