#include <Control_Surface.h> // Include the Control Surface library
 
// Instantiate a MIDI over USB interface.
HairlessMIDI_Interface midi;
PCButton button2 { 2, {2, Channel_7}};
PCButton button3 { 3, {3, Channel_7}};
PCButton button4 { 4, {4, Channel_7}};
PCButton button5 { 5, {5, Channel_7}};
PCButton button6 { 6, {6, Channel_7}};
PCButton button7 { 7, {7, Channel_7}};
PCButton button8 { 8, {8, Channel_7}};
PCButton button9 { 9, {9, Channel_7}};
PCButton button10 { 10, {10, Channel_7}};
PCButton button11 { 11, {11, Channel_7}};
PCButton button12 { 12, {12, Channel_7}};
PCButton button13 { 13, {13, Channel_7}};
PCButton button14 { 14, {14, Channel_7}};
PCButton button15 { 15, {15, Channel_7}};
PCButton button16 { 16, {16, Channel_7}};
PCButton button17 { 17, {17, Channel_7}};
PCButton button18 { 18, {18, Channel_7}};
PCButton button19 { 19, {19, Channel_7}};
PCButton button20 { 20, {20, Channel_7}};
PCButton button21 { 21, {21, Channel_7}};
PCButton button22 { 22, {22, Channel_7}};
PCButton button23 { 23, {23, Channel_7}};
PCButton button24 { 24, {24, Channel_7}};
PCButton button25 { 25, {25, Channel_7}};
PCButton button26 { 26, {26, Channel_7}};
PCButton button27 { 27, {27, Channel_7}};
PCButton button28 { 28, {28, Channel_7}};
PCButton button29 { 29, {29, Channel_7}};
PCButton button30 { 30, {30, Channel_7}};
PCButton button31 { 31, {31, Channel_7}};
PCButton button32 { 32, {32, Channel_7}};
PCButton button33 { 33, {33, Channel_7}};
PCButton button34 { 34, {34, Channel_7}};
PCButton button35 { 35, {35, Channel_7}};
PCButton button36 { 36, {36, Channel_7}};
PCButton button37 { 37, {37, Channel_7}};
PCButton button38 { 38, {38, Channel_7}};
PCButton button39 { 39, {39, Channel_7}};
PCButton button40 { 40, {40, Channel_7}};
PCButton button41 { 41, {41, Channel_7}};
PCButton button42 { 42, {42, Channel_7}};
PCButton button43 { 43, {43, Channel_7}};
PCButton button44 { 44, {44, Channel_7}};
PCButton button45 { 45, {45, Channel_7}};
PCButton button46 { 46, {46, Channel_7}};
 
void setup() {
  int midi_channel = 7;
  Control_Surface.begin();
}
 
void loop() {
  Control_Surface.loop();
}