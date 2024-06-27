/********************************************************************************************
*Filename      : voltemeter.c
*Description   : make a voltemeter
*Author        : Alan
*Website       : www.osoyoo.com
*Update        : 2017/07/04
********************************************************************************************/
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>

#define LCDADDR             0x3F //IIC LCD address
#define BLEN                1   //1--open backlight,0--close backlight

#define CHAN_CONFIG_SINGLE  8   //setup channel 0 as Single-ended input
#define SPICHANNEL          0   //MCP3008 connect to SPI0
#define ANALOGCHANNEL       0   //Potentiometer connect MCP3008 analog channel 0

static int spifd ;
static int i2cfd;

void spiSetup (int spiChannel)
{
    if ((spifd = wiringPiSPISetup (spiChannel, 10000)) < 0)
    {
        fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
        exit (EXIT_FAILURE) ;
    }
}

int myAnalogRead(int spiChannel,int channelConfig,int analogChannel)
{
    if(analogChannel<0 || analogChannel>7)
        return -1;
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (channelConfig+analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
    return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
}

void print_info()
{
    printf("\n");
    printf("|************************************|\n");
    printf("|                 Voltemter          |\n");
    printf("|       -------------------------    |\n");
    printf("|       | ADC |           | Pi  |    |\n");
    printf("|       |-----|-----------|-----|    |\n");
    printf("|       | CS  | connect to| CE0 |    |\n");
    printf("|       | Din | connect to| MOSI|    |\n");
    printf("|       | Dout| connect to| MISO|    |\n");
    printf("|       | CLK | connect to| SCLK|    |\n");
    printf("|       | CH0 | connect to| 3.3V|    |\n");
    printf("|       | CH1 | connect to| GND |    |\n");
    printf("|************************************|\n");
    printf("|   Potentiometer connect to ADC CH0 |\n");
    printf("|                              OSOYOO|\n");
    printf("|************************************|\n");
    printf("\n");

}

//write a word to lcd
void write_word(int data){
    int temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;
    wiringPiI2CWrite(i2cfd, temp);
}

//send command to lcd
void send_command(int comm){
    int buf;
    // Send bit7-4 firstly
    buf = comm & 0xF0;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);

    // Send bit3-0 secondly
    buf = (comm & 0x0F) << 4;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

//send data to lcd
void send_data(int data){
    int buf;
    // Send bit7-4 firstly
    buf = data & 0xF0;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);

    // Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

//initialize the lcd
void init(){
    send_command(0x33); // Must initialize to 8-line mode at first
    delay(5);
    send_command(0x32); // Then initialize to 4-line mode
    delay(5);
    send_command(0x28); // 2 Lines & 5*7 dots
    delay(5);
    send_command(0x0C); // Enable display without cursor
    delay(5);
    send_command(0x01); // Clear Screen
    wiringPiI2CWrite(i2cfd, 0x08);
}

//clear screen
void clear(){
    send_command(0x01); //clear Screen
}

//Print the message on the lcd
void write(int x, int y, char data[]){
    int addr, i;
    int tmp;
    if (x < 0)  x = 0;
    if (x > 15) x = 15;
    if (y < 0)  y = 0;
    if (y > 1)  y = 1;

    // Move cursor
    addr = 0x80 + 0x40 * y + x;
    send_command(addr);
    
    tmp = strlen(data);
    for (i = 0; i < tmp; i++){
        send_data(data[i]);
    }
}

int main()
{
    int adc;
    float voltage;
    char buf[5];
    if(wiringPiSetup() < 0)
    {   fprintf(stderr,"Can't init wiringPi: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }

    spiSetup(SPICHANNEL);//init spi

    i2cfd = wiringPiI2CSetup(LCDADDR);//init i2c
    init();//init LCD
    clear();//clear screen

    print_info();

    while(1)
    {
        adc = myAnalogRead(SPICHANNEL,CHAN_CONFIG_SINGLE,ANALOGCHANNEL);
        voltage = adc/1024.*3.3;
        write(0,0,"Voltage:");
        sprintf(buf,"%1.2f",voltage);//float change to string
        write(8,0,buf);//print voltage on lcd
        write(12,0,"V");//print unit

        write(8,1,"--OSOYOO");
        delay(1000);
    }
    return 0;
}
