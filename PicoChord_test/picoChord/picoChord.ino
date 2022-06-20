 
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include "Adafruit_MPR121.h"
#include <Wire.h>

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when electrodes are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;


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


void setup() {
  
  Wire.begin();
   
//begin reading capacitive input
  cap.begin(0x5A);

  #if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB such as mbed rp2040
  TinyUSB_Device_Init(0);
  #endif

  // Initialize MIDI, and listen to all MIDI channels
  // This will also call usb_midi's begin()
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // wait until device mounted
  while( !TinyUSBDevice.mounted() ) delay(1);

}

void loop() {

  currtouched = cap.touched();
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      note = key + chordNote[0][i];
      MIDI.sendNoteOn(note, 110, 1);
    }
    // if it *was* touched and now *isnt*
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      note = key + chordNote[0][i];
      MIDI.sendNoteOff(note, 0, 1);
    }
  }

  // reset state
  lasttouched = currtouched;
  MIDI.read();  
  return;
}
