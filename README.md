A sketch for scrolling text on the Adafruit 16x8 matrix Featherwings.

![Hello World!](matrix.gif)

Current state: Mostly functional.  Tested with an Adafruit Feamther M0 Express and a Feather 328p.

**Introduction**

This was written for the HT16K33-based Adafruit 8x16 Matrix FeatherWing.  The bulk of the code should be directly transferrable to any HT16K33 controlled LED matrix, but if it isn't wired in the same way as the FeatherWing your data may not display properly.

The premise is that you send serial data to a microcontroller using the Arduino's serial window, and this text is then displayed in a scrolling manner across the LEDs.

**How to use it**

1) Assemble the FeatherWing as per [Adafruit's assembly guide](https://learn.adafruit.com/adafruit-8x16-led-matrix-featherwing/assembly).
2) Plug the assembled board into a Feather microcontroller board.  This sketch has been tested with a Feather M0 Express and a Feather 328p, but should work with most/all other Feathers.
3) Upload the sketch in the same way you would for any other Arduino sketch: copy and paste the code into a new blank sketch.  Select your board and port using the drop-down menu at the top, and upload with the tick button in the upper left.
4) Open the Serial Monitor using the button in the upper right of the Arduino IDE, making sure that the baudrate at the bottom is set to 9600.  Type text into the box at the top and hit enter.

The text should now be scrolling across the matrix.

**Notes**
1) I'm not sure why but this was buggy on the Feather 328p when the serial baudrate was set to 115200.  9600 seems to work fine.
2) There is a const at the top of the sketch called "pause".  This is how long the sketch pauses a "frame" of LEDs before continuing the scrolling animation.  Increasing this slows the speed with which text moves across the LEDs, decreasing it increases the speed.


**To do**

- Fix the gif in the tutorial, it looks awful and doesn't properly represent how the setup looks.
