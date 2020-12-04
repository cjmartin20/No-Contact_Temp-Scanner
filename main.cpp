#include "no_contact_temp.h"
#include "TMP006.h"

#include <stdio.h>
#include <cstdio>
#include <string.h>
#include <math.h>

using namespace std;

//for TMP006
#define Address             0x80
#define TMP006_TX_PIN       p9
#define TMP006_RX_PIN       p10

//BufferedSerial
#define RN41_TX_PIN         p28
#define RN41_RX_PIN         p27
#define MAXIMUM_BUFFER_SIZE 32

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

TMP006 sensor(TMP006_TX_PIN, TMP006_RX_PIN, Address);

static BufferedSerial rn41(RN41_TX_PIN, RN41_RX_PIN, 115200);
static BufferedSerial pc(USBTX, USBRX, 9600); 
/*default is 9600 baud, 8 bit data data fram, no parity, one stop bit(1)*/

int main(void)
{

    // Application buffer to receive the data
    char buf[MAXIMUM_BUFFER_SIZE];
    char format[] = {"%i.%i"};
    while (1) {
        memset(&buf, '\0', sizeof(buf));
	    float thistemp = sensor.readObjTempC(Address);
	    int i = int(thistemp);
	    int d = abs(int((thistemp - float(i)) * 10000.0));
        sprintf(&buf[0], format, i, d);
        printf( "%s", buf);
        pc.write(buf, sizeof(buf));
        //printf("Test Part 2\n");
        ThisThread::sleep_for( 150ms );
    }
}