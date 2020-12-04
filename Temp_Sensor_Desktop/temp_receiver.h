#ifndef TEMP_RECEIVER_H
#define TEMP_RECEIVER_H


class temp_receiver
{
public:
    temp_receiver();
    int getTemp();
    int stopReceiving();
private:
    int serial_port;
    int currentTemp;
    int openSerialPort();
};

#endif // TEMP_RECEIVER_H
