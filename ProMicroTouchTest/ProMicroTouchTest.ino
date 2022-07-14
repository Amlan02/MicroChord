 
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "MIDIUSB.h"

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

//variables needed for the key, note and chord
int key = 48;
int chord = 0;
int note = 0;
int noteArray[13] = {0,49,56,51,58,53,48,55,50,57,52,59,54};

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
  
  //serial begin
  Serial.begin(9600);
  
  //begin reading capacitive input
  cap.begin(0x5A);

}

void loop() {
  
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
