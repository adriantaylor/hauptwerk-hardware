#include <Control_Surface.h> // Include the Control Surface library
 
USBMIDI_Interface midi;

NoteButton button2 = { 2, {36, CHANNEL_1} };
NoteButton button3 = { 3, {37, CHANNEL_1} };
NoteButton button4 = { 4, {38, CHANNEL_1} };
NoteButton button5 = { 5, {39, CHANNEL_1} };
NoteButton button6 = { 6, {40, CHANNEL_1} };
NoteButton button7 = { 7, {41, CHANNEL_1} };
NoteButton button8 = { 8, {42, CHANNEL_1} };
NoteButton button9 = { 9, {43, CHANNEL_1} };
NoteButton button10 = { 10, {44, CHANNEL_1} };
NoteButton button11 = { 11, {45, CHANNEL_1} };
NoteButton button12 = { 12, {46, CHANNEL_1} };
NoteButton button13 = { 13, {47, CHANNEL_1} };
NoteButton button14 = { 14, {48, CHANNEL_1} };
NoteButton button15 = { 15, {49, CHANNEL_1} };
NoteButton button16 = { 16, {50, CHANNEL_1} };
NoteButton button17 = { 17, {51, CHANNEL_1} };
NoteButton button18 = { 18, {52, CHANNEL_1} };
NoteButton button19 = { 19, {53, CHANNEL_1} };
NoteButton button20 = { 20, {54, CHANNEL_1} };
NoteButton button21 = { 21, {55, CHANNEL_1} };
NoteButton button22 = { 22, {56, CHANNEL_1} };
NoteButton button23 = { 23, {57, CHANNEL_1} };
NoteButton button24 = { 24, {58, CHANNEL_1} };
NoteButton button25 = { 25, {59, CHANNEL_1} };
NoteButton button26 = { 26, {60, CHANNEL_1} };
NoteButton button27 = { 27, {61, CHANNEL_1} };
NoteButton button28 = { 28, {62, CHANNEL_1} };
NoteButton button29 = { 29, {63, CHANNEL_1} };
NoteButton button30 = { 30, {64, CHANNEL_1} };
NoteButton button31 = { 31, {65, CHANNEL_1} };
NoteButton button32 = { 32, {66, CHANNEL_1} };
NoteButton button33 = { 33, {67, CHANNEL_1} };
NoteButton button34 = { 34, {68, CHANNEL_1} };
NoteButton button35 = { 35, {69, CHANNEL_1} };
NoteButton button36 = { 36, {70, CHANNEL_1} };
NoteButton button37 = { 37, {71, CHANNEL_1} };
NoteButton button38 = { 38, {72, CHANNEL_1} };
NoteButton button39 = { 39, {73, CHANNEL_1} };
NoteButton button40 = { 40, {74, CHANNEL_1} };
NoteButton button41 = { 41, {75, CHANNEL_1} };
NoteButton button42 = { 42, {76, CHANNEL_1} };
NoteButton button43 = { 43, {77, CHANNEL_1} };
NoteButton button44 = { 44, {78, CHANNEL_1} };
NoteButton button45 = { 45, {79, CHANNEL_1} };
NoteButton button46 = { 46, {80, CHANNEL_1} };
NoteButton button47 = { 47, {81, CHANNEL_1} };
NoteButton button48 = { 48, {82, CHANNEL_1} };
NoteButton button49 = { 49, {83, CHANNEL_1} };
NoteButton button50 = { 50, {84, CHANNEL_1} };
NoteButton button51 = { 51, {85, CHANNEL_1} };
NoteButton button52 = { 52, {86, CHANNEL_1} };
NoteButton button53 = { 53, {87, CHANNEL_1} };
NoteButton button54 = { 54, {88, CHANNEL_1} };
NoteButton button55 = { 55, {89, CHANNEL_1} };
NoteButton button56 = { 56, {90, CHANNEL_1} };
NoteButton button57 = { 57, {91, CHANNEL_1} };
NoteButton button58 = { 58, {92, CHANNEL_1} };
NoteButton button59 = { 59, {93, CHANNEL_1} };
NoteButton button60 = { 60, {94, CHANNEL_1} };
NoteButton button61 = { 61, {95, CHANNEL_1} };
NoteButton button62 = { 62, {96, CHANNEL_1} };
NoteButton button63 = { 63, {97, CHANNEL_1} };
 
void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}
 
void loop() {
  Control_Surface.loop(); // Update the Control Surface
}