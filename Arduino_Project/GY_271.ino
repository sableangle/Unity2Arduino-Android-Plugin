
/* FILE:    ARD_GY291_ADXL345_Example
   DATE:    02/04/14
   VERSION: 0.1
   
REVISIONS:

02/04/14 Created version 0.1

This is an example of how to use the Hobby Components GY-291 accelerometer module
(HCMODU0060). This module is based on the Analog Devices ADXL345 triple axis
accelerometer device.

This example sketch will demonstrate how perform a basic initialisation and then
will continually read each of the 3 axis registers and output them to the serial port.

PINOUT:

MODULE`                Arduino
GND                    GND
VCC                    +3.3V
CS                     +3.3V*
INT1                   N/A
INT2                   N/A
SD0                    N/A
SDA                    A4*
SCL                    A5*

*Please note that the ADXL345 opperates at 3.3V (via a 3.3V regulator) and these
pins should not be driven above 3.6V therefore you may require level shifters to
ensure safe opperation.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used for the purpose of promoting or selling products
that directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/

/* Include the standard wire library */
#include <Wire.h>

/* Alternate I2C address of the module */
#define I2C_Add 0x53

/* ADXL345 register addresses */
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define X_Axis 0x32
#define Y_Axis 0x34
#define Z_Axis 0x36

/* Accelerometer range modes */
#define RANGE_2g 0
#define RANGE_4g 1
#define RANGE_8g 2
#define RANGE_16g 3

void setup()
{
  /* Initialise the I2C bus */
  Wire.begin(); 
 
  /* Initialise the serial interface */
  Serial.begin(9600);
 
  /* Initialise the ADXL345 */ 
  Init_ADXL345(RANGE_2g);
}

/* Main program */
void loop()
{
  /* Continually read and output all 3 axis to the serial port */
  Serial.print("X: ");
  Serial.print(Read_Axis(X_Axis));
 
  Serial.print(" Y: ");
  Serial.print(Read_Axis(Y_Axis));
 
  Serial.print(" Z: ");
  Serial.println(Read_Axis(Z_Axis));
  delay(1000);
}

/* Read one of the 3 axis via the I2C interface */
int Read_Axis(byte axis)
{
  int Data;
   
  Wire.beginTransmission(I2C_Add);
  Wire.write(axis);
  Wire.endTransmission();
 
  Wire.beginTransmission(I2C_Add);
  Wire.requestFrom(I2C_Add, 2);
 
  /* If data is available then read it (2 bytes) */
  if(Wire.available())     
  {
    Data = (int)Wire.read();
    Data = Data  | (Wire.read() << 8);
  }else
  {
    Data = 0;
  }
   
  Wire.endTransmission(); 
  return Data;
}


/* Initialise the ADXL345 */
void Init_ADXL345(byte range)
{
  Wire.beginTransmission(I2C_Add);
 
  /* Set the sensitivity of the module */
  Wire.write(DATA_FORMAT);
  Wire.write(range);
  Wire.endTransmission();
 
  /* Put the module into measurement mode to start taking measurements */
  Wire.beginTransmission(I2C_Add);
  Wire.write(POWER_CTL);
  Wire.write(0x08);
 
  Wire.endTransmission();
}
