# What is this?

Here you'll find the electronics design and firmware of a web radio based on the ESP32 microcontroller.

## Design goals
* Use the enclosure of a "Tivolli Model One" lookalike radio. 
* Use the original speaker
* USB powered
* Easy to use : two knobs and a button
  * One knob to change the volume and to switch the radio on/off
  * One knob to change the radio station
  * One button to listen to the last news broadcast
* Cheap, except for the OLED-display, which is facultative

## Why web radio instead of DAB+?
* DAB+ reception quality is not great indoors.  The radio must be located near a window and the antenna must be fully extended.
* More choice of radio stations with web radio
* Sound quality won't probably be worse than DAB+.  I'm using a cheap speaker anyway.
* DAB+ is free, but in most places a wifi connection is available anyway.  The data consumption is low (about 30 MB per hour for a low quality 64kbps).
* DAB+ receiver modules are not widely available.  The probably aren't cheap either.

I think DAB+ has its niche in car radios, but not in home radios.

----
The original repository can be found [here](https://github.com/LieBtrau/esp32-web-radio).