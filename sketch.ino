#include <Wire.h>

const byte device_address = 0x70;

//Declare arrays containing the letters
const byte letters[28][6] = {
  {0b00111111, 0b01001000, 0b01001000, 0b01001000, 0b00111111, 0b00000000}, //A
  {0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110110, 0b00000000}, //B
  {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b01000001, 0b00000000}, //C
  {0b01111111, 0b01000001, 0b01000001, 0b01000001, 0b00111110, 0b00000000}, //D
  
  {0b01111111, 0b01001001, 0b01001001, 0b01000001, 0b01000001, 0b00000000}, //E
  {0b01111111, 0b01001000, 0b01001000, 0b01001000, 0b01000000, 0b00000000}, //F
  {0b00111110, 0b01000001, 0b01001001, 0b01001001, 0b00000110, 0b00000000}, //G
  {0b01111111, 0b00001000, 0b00001000, 0b00001000, 0b01111111, 0b00000000}, //H
  
  {0b01000001, 0b01000001, 0b01111111, 0b01000001, 0b01000001, 0b00000000}, //I
  {0b01000001, 0b01000001, 0b01111110, 0b01000000, 0b01000000, 0b00000000}, //J
  {0b01111111, 0b00001000, 0b00010100, 0b00100010, 0b01000001, 0b00000000}, //K
  {0b01111111, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000000}, //L

  {0b01111111, 0b00100000, 0b00011000, 0b00100000, 0b01111111, 0b00000000}, //M
  {0b01111111, 0b00100000, 0b00011000, 0b00000100, 0b01111111, 0b00000000}, //N
  {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110, 0b00000000}, //O
  {0b01111111, 0b01001000, 0b01001000, 0b01001000, 0b00110000, 0b00000000}, //P

  {0b00111110, 0b01000001, 0b01000101, 0b01000010, 0b00111101, 0b00000000}, //Q
  {0b01111111, 0b01001000, 0b01001100, 0b01001010, 0b00110001, 0b00000000}, //R
  {0b00110001, 0b01001001, 0b01001001, 0b01000101, 0b01000010, 0b00000000}, //S
  {0b01000000, 0b01000000, 0b01111111, 0b01000000, 0b01000000, 0b00000000}, //T

  {0b01111110, 0b00000001, 0b00000001, 0b00000001, 0b01111110, 0b00000000}, //U
  {0b01111000, 0b00000110, 0b00000001, 0b00000110, 0b01111000, 0b00000000}, //V
  {0b01111111, 0b00000010, 0b00001100, 0b00000010, 0b01111111, 0b00000000}, //W
  {0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011, 0b00000000}, //X

  {0b01100000, 0b00011000, 0b00000111, 0b00011000, 0b01100000, 0b00000000}, //Y
  {0b01000011, 0b01000101, 0b01001001, 0b01010001, 0b01100001, 0b00000000}, //Z
  {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110, 0b00000000}, //
  {0b01111111, 0b01001000, 0b01001000, 0b01001000, 0b00110000, 0b00000000}, //
  
                           };

//Declare an array for blanking the display
byte blank_display[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  Wire.begin();
  Serial.begin(115200);

  //Turn the oscillator to ON
  oscillator(0b00000001);

  //Set the RowInt reg to ROW mode
  rowInt(1);

  //Set the display to ON, NO blinking
  displaySetup(0b10000001);

  //Blank the display
  //Blank the display
  block_write(0x00, blank_display, 16);

}

void loop()
{
  Serial.println("Restarting loop");
  //Declare a large array for holding the data to be displayed
  byte data[96];

  //An int for counting the number of characters to be displayed
  byte numLetters = 0;

  //If there is serial data, write the LED values for each char to "data"
  while (Serial.available() > 0)
    {
      int incomingByte = Serial.read();
      memcpy(data + (numLetters * 6), letters[incomingByte - 65], 6);
      numLetters += 1;
    }

  //Add 16 columns (an entire display) of blank to the end
  memcpy(data + (numLetters * 6), blank_display, 16);
 
  //Calculate the number of bytes which need sent
  int numBytes = (numLetters * 6) + 16;
 
  //For each column of data which needs shown (except the last 16)
  for (int i = 0; i < (numBytes - 16); i++)
    {
      //Display that column at column 1 plus the next 15 columns
      block_write(0x00, &data[i], 16);
      delay(150);
    }
  
  Serial.println("Finished loop");
}

void displaySetup (byte mode)
{
  if (mode >= 0b10000000 and mode <= 0b10000111)
  {
    Wire.beginTransmission(device_address);
    Wire.write(mode);
    Wire.endTransmission();
  }
}

void rowInt (byte mode)
{
  if (mode == 0 or mode == 1)
  {
    Wire.beginTransmission(device_address);
    Wire.write(0b10100000 || mode);
    Wire.endTransmission();
  }
}

void oscillator (byte mode)
{
  Serial.println(mode);
  if ((mode == 0) or (mode == 1))
  {
    Serial.print("Setting oscillator to ");
    Serial.println(mode);
    Wire.beginTransmission(0x70);
    Wire.write(0b00100000 | mode);
    Wire.endTransmission();
  }
}

void block_write(byte reg, byte *values, int numValues)
{
  //Set initial bank of 16 rows
  byte leds[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  //find out how many bytes to write: if more than 8, set as 8
  int a = numValues;
  if (numValues > 8)
  {
    a = 8;
  }
  //Write the first 8 bytes to EVEN registers (the first 8 rows of LEDs)
  for (int i = 0; i < a; i++)
  {
    leds[i * 2] = values[i];
  }

  //Find out how many bytes there are beyond 8
  int b = numValues - 8;
  //Write each of these to ODD registers (the second 8 rows of LEDs)
  for (int i = 0; i < b; i++)
  {
    leds[i * 2 + 1] = values[i + 8];
  }

  //Actually write the bytes
  Wire.beginTransmission(device_address);
  Wire.write(reg);
  for (int x = 0; x < 16; x++)
  {
    Wire.write(leds[x]);
  }
  Wire.endTransmission();
}
