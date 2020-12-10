#include "no_contact_temp.h"
#include "TMP006.h"

#include <stdio.h>
#include <string.h>

using namespace std;

//for TMP006
#define Address             0x80
#define TMP006_TX_PIN       p9
#define TMP006_RX_PIN       p10

//BufferedSerial
#define RN41_TX_PIN         p28
#define RN41_RX_PIN         p27
#define MAXIMUM_BUFFER_SIZE 30

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

TMP006 sensor(TMP006_TX_PIN, TMP006_RX_PIN, Address);

static BufferedSerial rn41(RN41_TX_PIN, RN41_RX_PIN, 115200);
static BufferedSerial pc(USBTX, USBRX, 9600); 
/*default is 9600 baud, 8 bit data data fram, no parity, one stop bit(1)*/

int main(void){

    //buffer to send the temperature as a string
    char buf[MAXIMUM_BUFFER_SIZE];
    //format of string
    char format[] = {"$%i.%i$\n"};
    while (1) {
        //set all data in buffer equal to easy to use character
        memset(&buf, '\0', sizeof(buf));
        double current_temp = sensor.readObjTempF(Address);
	    int i = int(current_temp);
	    int d = abs(int((current_temp - double(i)) * 10.0));
        sprintf(&buf[0], format, i, d);
        pc.write(buf, sizeof(buf));
        if(current_temp > 70.0) led1 = 1; else led1 = 0;
        if(current_temp > 80.0) led2 = 1; else led2 = 0;
        if(current_temp > 90.0) led3 = 1; else led3 = 0;
        if(current_temp >100.0) led4 = 1; else led4 = 0;
        ThisThread::sleep_for( 150ms );
    }
}