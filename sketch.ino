#include <Wire.h>

//Const byte for holding the HT16K33's i2c address
const byte device_address = 0x70;

//Const int for holding the delay in ms between each frame moving
const int pause = 100;

//Declare a large array for holding the data to be displayed
byte data[600];

//Declare an array of arrays containing the letters
const byte letters[95][6] = {
  {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, //space
  {0b00000000, 0b00000000, 0b01111101, 0b00000000, 0b00000000, 0b00000000}, //!
  {0b00000000, 0b11100000, 0b00000000, 0b11100000, 0b00000000, 0b00000000}, //"
  {0b00010100, 0b01111111, 0b00010100, 0b01111111, 0b00010100, 0b00000000}, //#

  {0b00110010, 0b01001010, 0b11111111, 0b01001010, 0b01001100, 0b00000000}, //$
  {0b01000100, 0b10101000, 0b01010010, 0b00010101, 0b00100010, 0b00000000}, //%
  {0b01100110, 0b10011001, 0b10010110, 0b01100101, 0b00001001, 0b00000000}, //&
  {0b00000000, 0b00000000, 0b11100000, 0b00000000, 0b00000000, 0b00000000}, //'

  {0b00111100, 0b01000010, 0b10000001, 0b00000000, 0b00000000, 0b00000000}, //(
  {0b00000000, 0b00000000, 0b10000001, 0b01000010, 0b00111100, 0b00000000}, //)
  {0b01010100, 0b00111000, 0b11111110, 0b00111000, 0b01010100, 0b00000000}, //*
  {0b00001000, 0b00001000, 0b01111111, 0b00001000, 0b00001000, 0b00000000}, //+

  {0b00000000, 0b00000000, 0b00001101, 0b00001110, 0b00000000, 0b00000000}, //,
  {0b00000000, 0b00001000, 0b00001000, 0b00001000, 0b00000000, 0b00000000}, //-
  {0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b00000011, 0b00000000}, //.
  {0b00000001, 0b00000110, 0b00011000, 0b00110000, 0b01000000, 0b00000000}, // /

  {0b01111110, 0b10000101, 0b10011001, 0b10100001, 0b01111110, 0b00000000}, //0
  {0b00100001, 0b01000001, 0b11111111, 0b00000001, 0b00000001, 0b00000000}, //1
  {0b01100011, 0b10000101, 0b10001001, 0b10010001, 0b01100001, 0b00000000}, //2
  {0b10000001, 0b10000001, 0b10001001, 0b10001001, 0b01110110, 0b00000000}, //3

  {0b11111000, 0b00001000, 0b00011111, 0b00001000, 0b00001000, 0b00000000}, //4
  {0b11110001, 0b10010001, 0b10010001, 0b10010010, 0b10001100, 0b00000000}, //5
  {0b01111110, 0b10010001, 0b10010001, 0b10010001, 0b10001110, 0b00000000}, //6
  {0b10000000, 0b10001111, 0b10010000, 0b10100000, 0b11000000, 0b00000000}, //7

  {0b01110110, 0b10001001, 0b10001001, 0b10001001, 0b01110110, 0b00000000}, //8
  {0b01110000, 0b10010000, 0b10010000, 0b10010000, 0b11111111, 0b00000000}, //9
  {0b00000000, 0b00000000, 0b00110011, 0b00000000, 0b00000000, 0b00000000}, //:
  {0b00000000, 0b00000001, 0b00110010, 0b00000000, 0b00000000, 0b00000000}, //;

  {0b00011000, 0b00100100, 0b01000010, 0b10000001, 0b00000000, 0b00000000}, //<
  {0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00000000}, //=
  {0b00000000, 0b10000001, 0b00100100, 0b00011000, 0b00000000, 0b00000000}, //>
  {0b01000000, 0b10000000, 0b10001101, 0b10010000, 0b01100000, 0b00000000}, //?

  {0b01111110, 0b10011001, 0b10100101, 0b10011001, 0b01111010, 0b00000000}, //@

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

  {0b11111111, 0b10000001, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, //[
  {0b10000000, 0b01100000, 0b00011000, 0b00000110, 0b00000001, 0b00000000}, //backslash
  {0b00000000, 0b00000000, 0b00000000, 0b10000001, 0b11111111, 0b00000000}, //]
  {0b00100000, 0b01000000, 0b10000000, 0b01000000, 0b00100000, 0b00000000}, //^

  {0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000000}, //_
  {0b00000000, 0b10000000, 0b01000000, 0b00100000, 0b00000000, 0b00000000}, //`

  {0b00100010, 0b00100101, 0b00100101, 0b00100101, 0b00011111, 0b00000000}, //a
  {0b11111111, 0b00100001, 0b00100001, 0b00100001, 0b00011110, 0b00000000}, //b
  {0b00011110, 0b00100001, 0b00100001, 0b00100001, 0b00100001, 0b00000000}, //c
  {0b00011110, 0b00100001, 0b00100001, 0b00100001, 0b11111111, 0b00000000}, //d

  {0b00011110, 0b00100101, 0b00100101, 0b00100101, 0b00011101, 0b00000000}, //e
  {0b00010000, 0b01111111, 0b10010000, 0b10010000, 0b10000000, 0b00000000}, //f
  {0b00110110, 0b01011001, 0b01010101, 0b01110101, 0b01000110, 0b00000000}, //g
  {0b11111111, 0b00010000, 0b00100000, 0b00100000, 0b00011111, 0b00000000}, //h

  {0b00000001, 0b00010001, 0b01011111, 0b00000001, 0b00000001, 0b00000000}, //i
  {0b00000010, 0b00000001, 0b00000001, 0b00000001, 0b10111110, 0b00000000}, //j
  {0b11111111, 0b00001000, 0b00010100, 0b00100010, 0b00100001, 0b00000000}, //k
  {0b00000001, 0b10000001, 0b11111111, 0b00000001, 0b00000001, 0b00000000}, //l

  {0b00111111, 0b00010000, 0b00001100, 0b00010000, 0b00111111, 0b00000000}, //m
  {0b00111111, 0b00010000, 0b00001100, 0b00000010, 0b00111111, 0b00000000}, //n
  {0b00011110, 0b00100001, 0b00100001, 0b00100001, 0b00011110, 0b00000000}, //o
  {0b00111111, 0b00100100, 0b00100100, 0b00100100, 0b00011000, 0b00000000}, //p

  {0b00011000, 0b00100100, 0b00100100, 0b00111111, 0b00000001, 0b00000000}, //q
  {0b00000001, 0b00111111, 0b00010001, 0b00100000, 0b00010000, 0b00000000}, //r
  {0b00011001, 0b00100101, 0b00100101, 0b00100101, 0b00100010, 0b00000000}, //s
  {0b11111110, 0b00100001, 0b00100001, 0b00000001, 0b00000010, 0b00000000}, //t

  {0b00111110, 0b00000001, 0b00000001, 0b00000010, 0b00111111, 0b00000000}, //u
  {0b00111000, 0b00000110, 0b00000001, 0b00000110, 0b00111000, 0b00000000}, //v
  {0b00111110, 0b00000001, 0b00000110, 0b00000001, 0b00111110, 0b00000000}, //w
  {0b00100001, 0b00010010, 0b00001100, 0b00010010, 0b00100001, 0b00000000}, //x

  {0b00110000, 0b00001001, 0b00001001, 0b00001001, 0b00111110, 0b00000000}, //y
  {0b00100001, 0b00100011, 0b00100101, 0b00101001, 0b00110001, 0b00000000}, //z
  {0b00001000, 0b00110110, 0b01000001, 0b00100010, 0b00000000, 0b00000000}, //{
  {0b00000000, 0b00000000, 0b11111111, 0b00000000, 0b00000000, 0b00000000}, //|

  {0b00000000, 0b00100010, 0b01000001, 0b00110110, 0b00001000, 0b00000000}, //}
  {0b00001100, 0b00010000, 0b00001100, 0b00000100, 0b00011000, 0b00000000}, //~
                           };

//Declare an array holding a display's worth of blank LEDs (screen blank)
byte blank_display[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  Wire.begin();
  Serial.begin(9600);

  //Turn the oscillator to ON
  oscillator(0b00000001);

  //Set the RowInt reg to ROW mode
  rowInt(1);

  //Set the display to ON, NO blinking
  displaySetup(0b10000001);

  //Blank the display
  block_write(0x00, blank_display, 16);

  //Set the first 16 bytes of data[] to be blank
  for(int x = 0; x < 16; x++)
    {
      data[x] = 0b00000000;
    }
}


void loop()
{
  //An int for counting the number of characters to be displayed
  byte numLetters = 0;

  /* If there is serial data, take each char, fetch the corresponding bytes for
  the LED representation and append them to the data[] array */
  if (Serial.available())
  {
    //While there are unprocessed characters
    while (Serial.available())
      {
        //Store the next char in incomingByte
        int incomingByte = Serial.read();
        //If incomingByte is within the ASCII values for meaningful characters
        if (incomingByte >= 32 and incomingByte <= 126)
          {
          Serial.println(incomingByte);
          //Copy the bytes for the LED representation of that char to data
          memcpy(data + (numLetters * 6) + 16, letters[incomingByte - 32], 6);
          //Increase the count of letters processed
          numLetters += 1;
          }
      }

    //Add 16 columns (an entire display) of blank to the end
    memcpy(data + (numLetters * 6) + 16, blank_display, 16);
 
    /* Calculate the number of bytes which need sent. 6 for each letter, 16 for 
    blank screens at both the start and end */
    int numBytes = (numLetters * 6) + 32;
 
    //For each column of data which needs shown (except the last 16)
    for (int i = 0; i < (numBytes - 16); i++)
      {
        //Display that column at column 1 plus the next 15 columns
        block_write(0x00, &data[i], 16);
        delay(pause);
      }

    //All data has been displayed, blank all of the used bytes in data[]
    memset(data,0,numBytes);
  }
  delay(25);
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
/*
  void block_write(byte reg, byte *values, int numValues)
  {
    Serial.println("Prepping bytes");
    Wire.println("Sending data");
    Wire.beginTransmission(device_address);
    Wire.write(reg);
    for (int x = 0; x < numValues; x++)
      {
        Wire.write(values[x]);
      }
    Wire.endTransmission();
  }
*/

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
