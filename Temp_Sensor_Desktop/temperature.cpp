#include "temperature.h"

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <thread>
#include <stdio.h>
#include <chrono>
#include <string>

#define READ_BUFFER_LENGTH       30
#define NUM_BUFFER_LENGTH    12


// Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
int serial_port;
int currentTemp = 0;
char read_buf[READ_BUFFER_LENGTH];

Temperature::Temperature()
{
}
double Temperature::getTemp(){
    serial_port = open("/dev/ttyACM0", O_RDWR);

        // Create new termios struc, we call it 'tty' for convention
        struct termios tty;

        // Read in existing settings, and handle any error
        if(tcgetattr(serial_port, &tty) != 0) {
            return 1; //error with serial port
        }
        tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
        tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
        tty.c_cflag |= CS8; // 8 bits per byte (most common)
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
        tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO; // Disable echo
        tty.c_lflag &= ~ECHOE; // Disable erasure
        tty.c_lflag &= ~ECHONL; // Disable new-line echo
        tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        tty.c_iflag &= ~IXOFF;//(IXON | IXOFF | IXANY);  Turn off s/w flow ctrl
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

        tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        tty.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        tty.c_cc[VMIN] = 30;

        // Set in/out baud rate to be 9600
        cfsetispeed(&tty, B9600);
        cfsetospeed(&tty, B9600);

        // Save tty settings, also checking for error
        if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
            printf("Error # from tcsetattr:" );//%s\n", errno, strerror(errno));
            return 1;
        }

    // Normally you wouldn't do this memset() call, but since we will just receive
      // ASCII data for this example, we'll set everything to 0 so we can
      // call printf() easily.
      //replacement for memset
      for(int i = 0; i < READ_BUFFER_LENGTH; i++){
          read_buf[i] = '_';
      }
      // Read bytes. The behaviour of read() (e.g. does it block?,
      // how long does it block for?) depends on the configuration
      // settings above, specifically VMIN and VTIME
      int no_data = true;
      while(no_data){
     int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

      // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
      if (num_bytes < 0) {
          printf("Error reading bytes\n");
          return 0.0;
      }
      printf("Bytes read : %i\n", num_bytes);
      printf("read_buf (from data check):\n%s\n", read_buf);
      //scan buffer and make sure there are numbers;
      //make sure there are at least two '$' in buffer
      int knt = 0;
      for(int i=0; i<READ_BUFFER_LENGTH; i++){
          if (read_buf[i] == '$') knt++;
          if (knt > 1) no_data = false;
      }
      }



      char num_buf[NUM_BUFFER_LENGTH];
      int buf_index = 0;
      for(int i = 0; i < NUM_BUFFER_LENGTH; i++){
          num_buf[i] = '\0';
      }
      bool start_reading = false;
      //start at index 5 to avoid a number that was split
      for(int i = 0; i<READ_BUFFER_LENGTH; i++){
          if (read_buf[i] == '$'){ // recognize separation char
              if(start_reading){
                  printf("Breaking loop at %i...\ncurrent char: %c\nprevious char: %c\n", i,read_buf[i], read_buf[i - 1]);
                  if (i != READ_BUFFER_LENGTH) printf("next char : %c", read_buf[i + 1]);
                      break;
              }
              else start_reading = true;
          }
          else{
              if(start_reading){
                      num_buf[buf_index] = read_buf[i];
                      buf_index++;
              }
          }
      }
      printf("read_buf:\n%s\n", read_buf);
      printf("num_buf:\n%s\n", num_buf);
      close(serial_port);
      return std::stod(num_buf);//convert string trapped in num_buf to double
}
