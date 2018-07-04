#include <Wire.h>

const byte device_address = 0x70;

const byte letters[3][6] = {
  {0b00111111, 0b01001000, 0b01001000, 0b01001000, 0b00111111, 0b00000000},
  {0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110110, 0b00000000},
  {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b01000001, 0b00000000}
                           };

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
  byte data[6] = {0,0,0,0,0,0};

  byte numLetters = 0;

  //If there is serial data, append the LED values for each char to "data"
  while (Serial.available() > 0)
    {
      int incomingByte = Serial.read();
      Serial.print("Byte = ");Serial.println(incomingByte);
      Serial.println(char(incomingByte - 65));
      memcpy(data + (numLetters * 6), letters[incomingByte - 65], 6);
      numLetters += 1;
    }

  //Add 16 columns (an entire display) of blank to the end
  memcpy(data + (numLetters * 6), blank_display, 16);

  //byte data2[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
  //                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  //Calculate the number of bytes which need sent
  int numBytes = (numLetters * 6) + 16;
  //int numBytes2 = 32;

    
  //For each column of data which needs shown (except the last 16)
  for (int i = 0; i < (numBytes - 16); i++)
    {
      //Display that column at column 1 plus the next 15 columns
      block_write(0x00, &data[i], 16);
      delay(100);
    }


  block_write(0x00, blank_display, 16);
  delay(1000);

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
