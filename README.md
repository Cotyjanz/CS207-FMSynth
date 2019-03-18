#The Genesis Synth

##Overview

This project is for buildign a MIDI synthesizer around a Yamaha YM2612.
This project is loosely based on the work of Fabien Batteix (also known as SkyWodd) and fastman0204 (whose work can be found on youtube). We used their code and build to begin the process of understanding the fairly undocumented YM2612 chip.

The main components of this project are:
AT Mega 2560
Yamaha YM2612 sound chip
MIDI
LM386 Audio Amplifier
DJ Box (sourced from Dollarama)


###YM2612 pinout remapped for ATmega2560 / Arduino Mega

YM2612  ATmega 2560 Pinout : Mega/IDE
IC`   PC5 : 32
CS`   PC4 : 33
WR`   PC3 : 34
RD`   PC2 : 35
A0    PC1 : 36
A1    PC0 : 37
D0~D7 PORTA 0~7 : pin 22-29
clk   B5 is OC1A timer : pin 11


https://console5.com/wiki/YM2612


