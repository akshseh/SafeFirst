/*Authors:Anannya uberoi
	Sarthak Jindal
	Akarsha Sehwag
	AakashDeep Chhonkar
	Y S Ramya
*/
#include <reg51.h>

unsigned char *command_CMGF="AT+CMGF=1\r",*command_CMGS;
unsigned char *message="SAFETY BUTTON PRESSED!!!\n\n";
unsigned char CTRLZ =0x1A,ch,LAT[82];

sbit led_pin=P2^0;

void delay(unsigned int msec)
{
int i,j ;
for(i=0;i<msec;i++)
	for(j=0;j<1200;j++); 
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
	sendcommand(command_CMGF);
	delay(1000);
	
	sendcommand(command_CMGS);
	delay(1000);

	sendcommand(message);
	sendcommand("GPS Location in NMEA Sentence:\n\n ");
	sendcommand(LAT);
	delay(100);

	sendcommandcharbychar(CTRLZ);
	delay(1000);
	
}

void send_messages()
{
	command_CMGS="AT+CMGS=\"+919654309726\"\r"; //aakash
	GSM_write(command_CMGS);
	command_CMGS="AT+CMGS=\"+919958221803\"\r"; //sarthak
	GSM_write(command_CMGS);
	command_CMGS="AT+CMGS=\"9971408507\"\r"; //akarsha
	GSM_write(command_CMGS);
	command_CMGS="AT+CMGS=\"+918375895350\"\r"; //ramya
	GSM_write(command_CMGS);
}

void init_gps()
{
	TMOD=0x20; //Timer select mode2 (8 bit auto-reload)
	TH1=0xfd; //the higher byte of timer1 is set for 9600 baud_rate
	SCON=0x50; //mode1 8-bit UART to enable receiving of serial data;
	TR1=1; //enable timer 1
	IE=0x91;
	EA=1;
}

void init_reset(){
		TMOD = 0x00;
		TH1 =0x00;
		SCON= 0x00;
		TR1 = 0;
		IE=0x00;
		EA = 1;  //Enable Interrupt
    EX0 = 1;  //Enable External Hardware 0 Interrupt
    IE0 = 0;  //Clear ExHW0 Flag
    IT0 = 1;  //Choose Interrupt Type 0 for ExHW0
}

unsigned char recieve_data()
{
	while(RI == 0);
	ch = SBUF;
	RI=0;
	return ch;
}

void gps () {
	unsigned char  i,Temp;
	Temp=recieve_data();
	for(i=0;i<81 && Temp!=0x00 ;i++ ){
			LAT[i]=Temp;
			Temp=recieve_data();
	}
	LAT[i]=0x00;
}
	
void Interrupt_Service_Routine(void) interrupt 0
{
	led_pin = ~led_pin;

	init_gps();	
	gps();
	
	TH1=0xf4;
	send_messages();
	
	init_reset();
}

void main(void)
{
	init_reset();
   while(1);
}
