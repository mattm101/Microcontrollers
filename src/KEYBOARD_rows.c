#include <REGX52.H>

void serialInit(void);
void intInit(void);
void send(char oneByte);

volatile char enabled = 1;
char tab[4] = {0xEF, 0xDF, 0xBF, 0x7F};
char row = 0;	
char col = 0;
char i = 0;
char j = 0;
sbit int1 = P3^7;
char digit = 0;
char test = 0;
char tmp = 0;
sbit diod = P0^2;
sbit sendReq = P3^4;



void main(void){
	diod = 0;
	intInit();
	serialInit();
	while(1){
		P2 = tab[row];
		tmp = P2;
		if(tmp != tab[row]){
			if(P2_1 == 0){
				col = 3;
			}
			else if(P2_2 == 0){
				col	= 2;
			}
			else if(P2_3 == 0){
				col = 1;
			}
			if(test == 4){
				digit = (col + row * 3);
				if(digit == 11) digit = 0;
				P1 = digit;
				if(enabled == 1){
					int1 = 0;
					int1 = 1;
					send(digit);
				}
				
			}
			test = 0;
		}
		else if(test < 4) test++;
			row++;
			if(row == 4) row = 0;
			
	}
}

void serialInit(void){
	SCON = 0x40;
	PCON = 0x80;
	TMOD = 0x20;
	TH1 = 0xFA; //9600 bodow
	sendReq = 0;
	ES = 1;
	TR1 = 1;
}

void intInit(void){
	IT0 = 1;
	IT1 = 1;
	EX0 = 1;
	EX1 = 1;
	EA = 1;
}

void send(char oneByte){
	sendReq = 1;
	TI = 0;
	SBUF = oneByte;
	while(TI == 0) {;}
  TI = 0;
	sendReq = 0;
}

void receive(void) interrupt 4{
   if (RI == 1) {RI = 0;}
}

void disable(void) interrupt 0{
	enabled = 0;
	diod = 1;
	send(0x10);
}

void enable(void) interrupt 2{
	enabled = 1;
	diod = 0;
}


