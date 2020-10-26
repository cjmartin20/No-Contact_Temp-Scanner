/*
 * Read Temperature from TMP0006 sensor
 */
#include "temp_sens.h"

I2C i2c(p9, p10);

const int addr = 0x80;

//TMP0006 sends a 16bit (2 byte) signed value
//LSB represents 1/32 Celcius
char buff[2]; 

float temp_Celcius() {
  char reg[1];
  char data[2];
  
  reg[0] = TMP006_TAMB;
  
  m_i2c.write(addr, reg, 1);
  m_i2c.read(addr, data, 2);
   
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
	return float( ( buff[0] << 8 ) | buff[1] ) / 32.0;
}
float temp_Fareinheit() {
    return 6.9;
}
float temp_Kelvin() {
    return 6.9;
}


/*
 * Copyright (c) 2014-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
/*
#include "mbed.h"

// Read temperature from LM75BD

I2C i2c(I2C_SDA, I2C_SCL);

const int addr7bit = 0x48;      // 7 bit I2C address
const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90

int main()
{
    char cmd[2];
    while (1) {
        cmd[0] = 0x01;
        cmd[1] = 0x00;
        i2c.write(addr8bit, cmd, 2);

        ThisThread::sleep_for(500);

        cmd[0] = 0x00;
        i2c.write(addr8bit, cmd, 1);
        i2c.read(addr8bit, cmd, 2);

        float tmp = (float((cmd[0] << 8) | cmd[1]) / 256.0);
        printf("Temp = %.2f\n", tmp);
    }
}
 */
