#include "no_contact_temp.h"
#include <stdio.h>
#include "TMP006.h"


/* Program Example 11.1: Bluetooth serial test data program
Data is transferred from mbed to PC via Bluetooth. */
#include "mbed.h"
Serial rn41(p9,p10); //name the serial port rn41
BusOut led(LED4,LED3,LED2,LED1);
int main() {
rn41.baud(115200); // set baud for RN41
	while (1) {
		for (char x=0x30;x<=0x39;x++){ // ASCII numerical characters 0-9
		rn41.putc(x); // send test char data on serial to RN41
		led = x & 0x0F; // set LEDs to count in binary
		wait(0.5);
		}
	}
}





/*
#define Address 0x80
using namespace std;

//TMP006 sensor(p9, p10, Address); 

char float_to_string( float f );

// main() runs in its own thread in the OS
int main()
{

    while (true) {
        //float thistemp = sensor.readObjTempC(Address);
        float thistemp = temp_Celcius();
        int i = int(thistemp);
        int d = int((thistemp - float(i)) * 10000.0);
        printf( "Temperature %i.%i\n", i, d);
		ThisThread::sleep_for( 50ms );
    }
}
*/

/*
char float_to_string( float f ) {
    char buff[10];
    int i = int(f);
    float d = f - float(i)
    d = d * 1000.0;
    char format = {"%i.%i"};
    sprintf(&buff[0], format, i, d);
    return buff;   
}
*/
