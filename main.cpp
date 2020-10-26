#include "no_contact_temp.h"
#include <stdio.h>
#include "TMP006.h"

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