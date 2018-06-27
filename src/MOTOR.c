#include <REGX52.H>

#define IN1 P2_1
#define IN2 P2_2
#define EN1 P2_3
#define IN3 P2_5
#define IN4 P2_6
#define EN2 P2_4

void serialInit(void);
void startUpperMotor(char dir);
void startLowerMotor(char dir);

char LEFT = 0x01;
char RIGHT = 0x10;


sbit sendReq = P3^4;
char oneByte;



void main(void){
	serialInit();
	IN1 = 0;
	IN2 = 1;
	EN1 = 1;
	while(1){
		
	}
}

void serialInit(void){
	SCON = 0x50;
	PCON = 0x80;
	TMOD = 0x20;
	TH1 = 0xFA; //9600
	sendReq = 0;
	ES = 1;
	EA = 1;
	TR1 = 1;
}

void startUpperMotor(char dir){
	IN1 = IN2 = 1;
	if(dir ==  0x01) IN1 = 0;
	else if(dir == 0x10) IN2 = 0;
	EN1 = 1;
}

void startLowerMotor(char dir){
	IN3 = IN4 = 1;
	if(dir == 0x01) IN3 = 0;
	else if(dir == 0x10) IN4 = 0;
	EN2 = 1;
}

void stopUpperMotor(void){
	EN1 = 0;
}

void stopLowerMotor(void){
	EN2 = 0;
}

void receive(void) interrupt 4{
  	if (RI == 1){
	  	RI = 0;
      oneByte = SBUF;
			switch(oneByte){
				case 2:
				startUpperMotor(RIGHT);
				break;
				case 8:
				startUpperMotor(LEFT);
				break;
				
				case 4:
					startLowerMotor(LEFT);
				break;
				
				case 6:
					startLowerMotor(RIGHT);
				break;
				
				case 5:
					stopUpperMotor();
					stopLowerMotor();
				break;
			}
		}
}

