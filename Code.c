
#include <reg51.h>
//#include <AT89X51.H>
#define port2 P2

unsigned char *command_AT = "AT\r";
unsigned char *command_CMGF="AT+CMGF=1\r";
unsigned char *command_CMGS;
unsigned char *message="SAVE ME I'M AT THIS PLACE\n--Ramya";
unsigned char CTRLZ =0x1A,ch;
unsigned char LAT[9], LON[10];

int switch_pin; //P3.2

char test[6]={"$GPGGA"};
unsigned int check=0,i;
unsigned char a;
//unsigned char receive_data();

void sendcommandcharbychar(unsigned char ch)
{
	SBUF=ch;
	while(TI==0); //wait until the char is sent and TI is set to 1
	TI=0; // set TI back to 0
}

void sendcommand(unsigned char *p)
{
	unsigned char *temp=p;
	while(*temp!=0x00)
	{
		sendcommandcharbychar(*temp);
		temp++;
	}
}

void GSM_write(unsigned char *command_CMGS)
{
	sendcommand(command_AT);
	sendcommand(command_CMGF);
	sendcommand(command_CMGS);
	sendcommand(message);
	sendcommand(LAT);
	sendcommand(LON);
	sendcommandcharbychar(CTRLZ);
}

void send_messages()
{
	command_CMGS="AT+CMGS=\"9654309726\"\r"; //aakash
	GSM_write(command_CMGS);
	command_CMGS="AT+CMGS=\"9958221803\"\r"; //sarthak
	GSM_write(command_CMGS);
	command_CMGS="AT+CMGS=\"9971408507\"\r"; //akarsha
	GSM_write(command_CMGS);
	command_CMGS="AT+CMGS=\"8527617051\"\r"; //anannya
	GSM_write(command_CMGS);
	command_CMGS="AT+CMGS=\"8375895350\"\r"; //ramya
	GSM_write(command_CMGS);
}

/*
void delay(unsigned int msec)
{
int i,j ;
for(i=0;i<msec;i++)
	for(j=0;j<1275;j++); 
}
*/

void init()
{
	TMOD=0x20; //Timer select mode2 (8 bit auto-reload)
	TH1=0xfd; //the higher byte of timer1 is set for 9600 baud_rate
	SCON=0x50; //mode1 8-bit UART to enable receiving of serial data;
	TR1=1; //enable timer 1
	IT0=0;
	IE=0x91;
}

unsigned char recieve_data()
{
	while(RI == 0);
	ch = SBUF;
	RI=0;
	return ch;
}

void gps () {

unsigned char Temp, i;
if (recieve_data() == '$'){
	if( recieve_data() == 'G'){
		if (recieve_data() == 'P'){
			if (recieve_data() == 'R'){
				if (recieve_data() == 'M'){
					if (recieve_data() == 'C'){
						while (recieve_data() != ',');
						while (recieve_data() != ',');
						/*checking for “A” condition*/

						Temp = recieve_data();
						if (Temp == 'A'||Temp == 'V'){
							while (recieve_data() != ',');
							/*latitude values*/
						
							for (i=0; i<9; i++)
							{
								LAT[i] = recieve_data();
							}
							while (recieve_data() != ',');

							while (recieve_data() != ',');
							/*longitude values*/

							for (i=0; i<10; i++)
							{
								LON[i] = recieve_data();
							}

						}

					}
				}
			}
		}
	}
}
}

void main()
{
	EA=1;
	ES=1;
	init();
	while(1)
	{
		if(switch_pin==0){
			gps();
			send_messages();
		}
	}
}
