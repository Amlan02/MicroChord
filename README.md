# Midi-Chord
A midi controller that aims to function like an omnichord

What you need:

- Sparkfun Pro Micro (clone) (Arduino Leonardo is a good alternative)
- MPR-121 capacitive touch module
- 12 button switches
- 12 2.2k resistors
- 1 100k resistor
- copper tape (or maybe come up with something else to get those electrodes going)
- wire

# Update: June 13 2022:

Reddit User forshee9283 was very kind to come up with a small PCB Design that you can use instead of copper tape.
I put the RevA PCB files in this github but they are also on his github repo: https://github.com/forshee9283/Strum.

I ordered 10 of these today and will post updates when the boards are here.

<img src="https://user-images.githubusercontent.com/42961161/173309362-fab22a5c-d46b-4f1f-8e31-f69920780756.png" style="width:400px;">


# Update June 20 2022:

Today the boards arrived and I tested one. It works really well. The sensitivy is great.
I also added a new folder for the raspberry pi pico. The idea is to continue the project on the pico because it offers better IO capabilities.
The code for the pico at the moment has working support for the capacitive touch and USB MIDI. but no buttons yet.


# Update March 15 2023:

Since I have a dayjob I don't have a lot of time to work on this project but I still want to complete it.
The thing I still need to tackle is handling all the push buttons (3 x 12).

My idea is to use the CD74HC4067 for multiplexing and the MCP23017 as I2C expansion.
This way all 3x12 buttons can be connected to the board using only 6 pins of the board.

I drafted a code snippet for this idea but didn't test things yet. Next up is testing this idea.
