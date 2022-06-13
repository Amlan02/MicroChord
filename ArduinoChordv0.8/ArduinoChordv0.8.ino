 
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "MIDIUSB.h"
#include <ezAnalogKeypad.h>

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when electrodes are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

//buttonArray
int btnValue = 0;
ezAnalogKeypad buttonArray(A0);

//variables needed for the key, note and chord
int key = 48;
int chord = 0;
int note = 0;
int noteArray[13] = {
  0,
  49, //Db
  56, //Ab
  51, //Eb
  58, //Bb
  53, //F
  48, //C
  55, //G
  50, //D
  57, //A
  52, //E
  59, //B
  54 //F#  
};

int chordNote[3][12] = {
  //major = +4, +3, +5
  { 0, 4, 7, 12, 16, 19, 24, 28, 31, 36, 40, 43 },  // maj
  //minor = +3 +4 +5
  { 0, 3, 7, 12, 15, 19, 24, 27, 31, 36, 39, 43}, // min
  //7th = +4, +3, +3, +2
  { 0, 4, 7, 10, 12, 16, 19, 22, 24, 28, 31, 34 }  // 7th
};

  
//midi note on and midi note off functions
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

//midi control change function
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void setup() {
  Serial.begin(9600);
  //begin reading capacitive input
  cap.begin(0x5A);

  //buttonValues in buttonArray
  buttonArray.setNoPressValue(0); //value when NO button is pressed.
  buttonArray.registerKey(1,809);
  buttonArray.registerKey(2,824);
  buttonArray.registerKey(3,838);
  buttonArray.registerKey(4,854);
  buttonArray.registerKey(5,870);
  buttonArray.registerKey(6,886);
  buttonArray.registerKey(7,903);
  buttonArray.registerKey(8,921);
  buttonArray.registerKey(9,940);
  buttonArray.registerKey(10,959);
  buttonArray.registerKey(11,979);
  buttonArray.registerKey(12,1001);
}

void loop() {

  unsigned char button = buttonArray.getKey();

  //If Button (i) is pressed, set key to noteArray(i)
  for(int i=0; i<13; i++) {
    if((int)button != 0 && (int)button == i) {
      key = noteArray[i];
      delay(50);
    }
  }

  currtouched = cap.touched();
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      note = key + chordNote[0][i];
      noteOn(0, note, 64); //C
      MidiUSB.flush();
    }
    // if it *was* touched and now *isnt*
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      note = key + chordNote[0][i];
      noteOff(0, note, 64); //C
      MidiUSB.flush(); 
    }
  }

  // reset state
  lasttouched = currtouched;
  return;
}
