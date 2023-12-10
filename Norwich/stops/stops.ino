#include <Control_Surface.h> // Include the Control Surface library
 
// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;
 
void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}
NoteButton button2 = { 2, {36, CHANNEL_9} };
NoteButton button3 = { 3, {37, CHANNEL_9} };
NoteButton button4 = { 4, {38, CHANNEL_9} };
NoteButton button5 = { 5, {39, CHANNEL_9} };
NoteButton button6 = { 6, {40, CHANNEL_9} };
NoteButton button7 = { 7, {41, CHANNEL_9} };
NoteButton button8 = { 8, {42, CHANNEL_9} };
NoteButton button9 = { 9, {43, CHANNEL_9} };
NoteButton button10 = { 10, {44, CHANNEL_9} };
NoteButton button11 = { A0, {45, CHANNEL_9} };
NoteButton button12 = { A1, {46, CHANNEL_9} };
NoteButton button13 = { A2, {47, CHANNEL_9} };
NoteButton button14 = { 14, {48, CHANNEL_9} };
NoteButton button15 = { 15, {49, CHANNEL_9} };
NoteButton button16 = { 16, {50, CHANNEL_9} };
NoteButton button17 = { A3, {51, CHANNEL_9} };

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}