#define NUM_ROWS 8
#define NUM_COLS 8

#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127

#define SERIAL_RATE 115200

// Row output pins
const int row1Pin = 14;
const int row2Pin = 8;
const int row3Pin = 15;
const int row4Pin = 7;
const int row5Pin = 16;
const int row6Pin = 6;
const int row7Pin = 17;
const int row8Pin = 5;

// col input pins
const int col1Pin = 9; 
const int col2Pin = 3;
const int col3Pin = 12;
const int col4Pin = 13;
const int col5Pin = 2;
const int col6Pin = 4;
const int col7Pin = 11;
const int col8Pin = 10;

// 74HC595 pins
const int dataPin = 6;
const int latchPin = 7;
const int clockPin = 8;

boolean keyPressed[NUM_ROWS][NUM_COLS];
uint8_t keyToMidiMap[NUM_ROWS][NUM_COLS];

// bitmasks for scanning columns
int bits[] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000
};
// define first note of keyboard (int note)

int values[8][8] = {
  { 36, 37, 38, 39, 40, 41, 42, 43 },
  { 44, 45, 46, 47, 48, 49, 50, 51 },
  { 52, 53, 54, 55, 56, 57, 58, 59 },
  { 60, 61, 62, 63, 64, 65, 66, 67 },
  { 68, 69, 70, 71, 72, 73, 74, 75 },
  { 76, 77, 78, 79, 80, 81, 82, 83 },
  { 84, 85, 86, 87, 88, 89, 90, 91 },
  { 92, 93, 94, 95, 96, 97, 98, 99 },
};

void setup() {
  int note = 36;

  for(int rowCtr = 0; rowCtr < NUM_ROWS; ++rowCtr)
  {
    for(int colCtr = 0; colCtr < NUM_COLS; ++colCtr)
    {
      keyPressed[rowCtr][colCtr] = false;
      keyToMidiMap[rowCtr][colCtr] = note;
      note++;
    }
  }

  // setup pins output/input mode
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(row1Pin, OUTPUT);
  pinMode(row2Pin, OUTPUT);
  pinMode(row3Pin, OUTPUT);
  pinMode(row4Pin, OUTPUT);
  pinMode(row5Pin, OUTPUT);
  pinMode(row6Pin, OUTPUT);
  pinMode(row7Pin, OUTPUT);
  pinMode(row8Pin, OUTPUT);

  pinMode(col1Pin, INPUT);
  pinMode(col2Pin, INPUT);
  pinMode(col3Pin, INPUT);
  pinMode(col4Pin, INPUT);
  pinMode(col5Pin, INPUT);
  pinMode(col6Pin, INPUT);
  pinMode(col7Pin, INPUT);
  pinMode(col8Pin, INPUT);

  Serial.begin(SERIAL_RATE);
}

void loop() {
  for (int colCtr = 0; colCtr < NUM_COLS; ++colCtr) {
    //scan next column
    scanColumn(colCtr);

    //get row values at this column
    int rowValue[NUM_ROWS];
    rowValue[0] = digitalRead(row1Pin);
    rowValue[1] = digitalRead(row2Pin);
    rowValue[2] = digitalRead(row3Pin);
    rowValue[3] = digitalRead(row4Pin);

    // process keys pressed
    for (int rowCtr = 0; rowCtr < NUM_ROWS; ++rowCtr) {
      if (rowValue[rowCtr] != 0 && !keyPressed[rowCtr][colCtr]) {
        keyPressed[rowCtr][colCtr] = true;
        noteOn(rowCtr, colCtr);
      }
    }

    // process keys released
    for (int rowCtr = 0; rowCtr < NUM_ROWS; ++rowCtr) {
      if (rowValue[rowCtr] == 0 && keyPressed[rowCtr][colCtr]) {
        keyPressed[rowCtr][colCtr] = false;
        noteOff(rowCtr, colCtr);
      }
    }
  }
}

void scanColumn(int colNum) {
  digitalWrite(latchPin, LOW);

  if (0 <= colNum && colNum <= 7) {
    shiftOut(dataPin, clockPin, MSBFIRST, B00000000);     //right sr
    shiftOut(dataPin, clockPin, MSBFIRST, bits[colNum]);  //left sr
  } else {
    shiftOut(dataPin, clockPin, MSBFIRST, bits[colNum - 8]);  //right sr
    shiftOut(dataPin, clockPin, MSBFIRST, B00000000);         //left sr
  }
  digitalWrite(latchPin, HIGH);
}

void noteOn(int row, int col) {
  Serial.write(NOTE_ON_CMD);
  Serial.write(keyToMidiMap[row][col]);
  Serial.write(NOTE_VELOCITY);
  Serial.println(keyToMidiMap[row][col]);
}

void noteOff(int row, int col) {
  Serial.write(NOTE_OFF_CMD);
  Serial.write(keyToMidiMap[row][col]);
  Serial.write(NOTE_VELOCITY);
}