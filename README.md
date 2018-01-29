# LedClock
Clock made of 512 Led's

![Led clock](/images/clock1.png)

480 Leds in mounted in a ring of 8 x 60 plus 32 Led's as a four digit 7 Segment display.

I built this clock a long time ago using a 8032 microprocessor with EPROM and RAM. It has got two PCB's, one is the controller 160mm * 100mm and the Led pcb 200mm * 200mm. A third small breadboard in the center has been mounted later to hold a four digit Sevensegment display.

A year ago, the clock did stop working. The controller pcb had a failure, but the led pcb was still ok. Because I did'nt found the 8032 software, I started to rebuild the controller using a ATMega 328.

The 3D model is designed in openscad. I ordered a few pre cutted acryl glass parts:
1* front 218mm x 210mm
1* top 210mm x 54mm
1* back 210mm x 211mm
1* bottom 210mm x 111mm
2* side 211mm x 107mm x 50mm (trapezoid)

The 3d printed parts "bohrschablone" help to drill the holes at the right place in the acryl glass. The parts staenderL.stl and staenderR.stl are on the bottom, halterL.stl and halterR.stl on the top. I had to tap a few M3 threads to fix the acryl glass plates.

The software is written "bare metal" using my [AvrLibrary](https://github.com/saarbastler/AvrLibrary). It can be compiled using Atmel Studio 7. Clicking the turn encoder will enter the menu turning will select a menu, click again will enter the menu for setting the time/date etc. The menu itself is in German.

The java fx application "Designer" compiled in eclipse, is a grafical designer to paint the clock faces and hands. It writes the file zeiger.cpp used in the firmware.
  