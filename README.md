A sketch for scrolling text on the Adafruit 16x8 matrix Featherwings.

Current state: partial functionality.  Send any **upper case** letters to the device and it will scroll them.  

**To do**

- Add support for spaces and other grammar characters
- Add a way to handle lower case letters
- Add a graceful escape for recieving incompatible characters
- Add a blank at the beginning, so that new letters scroll in from the right instead of suddenly appearing at the far left and then immediately scrolling out of view.
