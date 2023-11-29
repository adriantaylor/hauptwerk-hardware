#include <Control_Surface.h>

USBMIDI_Interface midi;

// The note numbers corresponding to the buttons in the matrix
// const AddressMatrix<8, 8> addresses {{
//   {1, 2, 3, 4, 5, 6, 7, 8},
//   {9, 10, 11, 12, 13, 14, 15, 16},
//   {17, 18, 19, 20, 21, 22, 23, 24},
//   {25, 26, 27, 28, 29, 30, 31, 32},
//   {33, 34, 35, 36, 37, 38, 39, 40},
//   {41, 42, 43, 44, 45, 46, 47, 48},
//   {49, 50, 51, 52, 53, 54, 55, 56},
//   {57, 58, 59, 60, 61, 62, 63, 64}
// }};

const AddressMatrix<8, 8> addresses {{
  {36, 37, 38, 39, 40, 41, 42, 43},
  {44, 45, 46, 47, 48, 49, 50, 51},
  {52, 53, 54, 55, 56, 57, 58, 59},
  {60, 61, 62, 63, 64, 65, 66, 67},
  {68, 69, 70, 71, 72, 73, 74, 75},
  {76, 77, 78, 79, 80, 81, 82, 83},
  {84, 85, 86, 87, 88, 89, 90, 91},
  {92, 93, 94, 95, 96, 97, 98, 99}
}};


NoteButtonMatrix<8, 8> buttonmatrix {
  {2, 3, 4, 5, 6, 7, 8, 9}, // row pins
  {10,11,12,13,A0,A1,A2,A3},    // column pins
  addresses,    // address matrix
  Channel_1,    // channel and cable number
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}