/*
 * Read Temperature from TMP0006 sensor
 */
#include "temp_sens.h"
#include "TMP006.h"

I2C i2c(p9, p10);

const int addr = 0x80;

//TMP0006 sends a 16bit (2 byte) signed value
//LSB represents 1/32 Celcius
char buff[2]; 

double temp_Celcius() {
  char reg[1];
  char data[2];
  
  reg[0] = TMP006_TAMB;
  
  i2c.write(addr, reg, 1);
  i2c.read(addr, data, 2);
   
  int16_t raw = (data[0] << 8) | data[1] ;
 
  raw >>= 2;

        buff[0] = 0x01;
        buff[1] = 0x00;
        i2c.write(addr, buff, 2);

        buff[0] = 0x00;
        i2c.write(addr, buff, 1);
        i2c.read(addr, buff, 2); //read two bites into buffer
	
	//shifts top byte left by 8, then inclusive or with bottom
    //then it is forced into a float and divided by 32 (LSB is 1/32 Celcius)
	return (( ( buff[0] << 8 ) | buff[1] ) / 32.0);
}
double temp_Fareinheit() {
    return (9.0 / 5.0) * temp_Celcius() + 32.0;
}
double temp_Kelvin() {
    return temp_Celcius() + 273.15;
}