#include <reg51.h>
#include <AT89X51.H>
#define port2 P2


unsigned char *command_AT = "AT\r";
unsigned char *command_CMGF="AT+CMGF=1\r";
unsigned char *command_CMGS;
unsigned char *message="For heavens sake thou good soul, save me ! I am at : \n";
unsigned char CTRLZ =0x1A;
unsigned int LAT[9], LON[10];

int switch_pin; //P3.2


char info[70];
char test[6]={"$GPGGA"};
char comma_position[15];
unsigned int check=0,i;
unsigned char a;
void receive_data();


void delay(unsigned int msec)
{
int i,j ;
for(i=0;i<msec;i++)
	for(j=0;j<1275;j++); 
}


void find_comma()
{
	unsigned int i,count=0;
	for(i=0;i<70;i++)
	{ 
		if(info[i]==',')
		{
		comma_position[count++]=i;
		}
	}
}


void init()
{
	TMOD=0x20; //Timer select mode2 (8 bit auto-reload)
	TH1=0xfd; //the higher byte of timer1 is set for 9600 baud_rate
	SCON=0x50; //mode1 8-bit UART to enable receiving of serial data;
	TR1=1; //enable timer 1
	IT0=0;
	IE=0x91;
}


void compare()
{ 
	IE.4=0; //Interrupt disable 
	find_comma(); //Function to detect position of comma in the string
	//lcd_latitude(); //Function to show Latitude
	//lcd_longitude(); //Function to show Longitude
	check=0;
	IE.4=1; //Interrupt enable
}


void receive_data() interrupt 4 
{
	info[check++]=SBUF; //Read SBUF
	if(check<7) //Condition to check the required data
	{
		if(info[check-1]!=test[check-1])
			check=0;
	}
	RI=0;
}


void ex0_isr (void) interrupt 0
{
	//GPS_location_receive();
	while(1)
	{
		if(check==69)
			compare();
	}


	GSM_init();
	send_messages();
}


void receive_data()
{
	info[check++]=SBUF;
}




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
	sendcommand(CTRLZ);
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

void gps () {

unsigned char Temp, i;
if (recieve_data() == ‘$’){
	if( recieve_data() == ‘G’){
		if (recieve_data() == ‘P’){
			if (recieve_data() == ‘R’){
				if (recieve_data() == ‘M’){
					if (recieve_data() == ‘C’){
						while (recieve_data() != ‘,’);
						while (recieve_data() != ‘,’);
						/*checking for “A” condition*/

						Temp = recieve_data();
						if (Temp == ‘A’||Temp == ‘V’){
							while (recieve_data() != ‘,’);
							/*latitude values*/
							//LCDCmd (0x80);

							for (i=0; i<9; i++)
							{
								LAT[i] = recieve_data();
								//LCDData (LAT[i]);
							}
							while (recieve_data() != ‘,’);

							while (recieve_data() != ‘,’);
							/*longitude values*/

							//LCDCmd (0xc0);
							for (i=0; i<10; i++)
							{
								LON[i] = recieve_data();
							//	LCDData (LON[i]);
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
	while(1)
	{
		init();
		gps();
		if(switch_pin==1){
			send_messages();
		}
	}
}