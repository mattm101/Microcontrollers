#include <REGX52.H>
//P1, P3_7 KEYBOARD -> P1, INT1 7SEG 
//P3_7 7SEG -> INT1 KEYBOARD
//P3_6 7SEG -> INT0 KEYBOARD

void intInit(void);
void timerReload(void);
void setDefault(void);
void timerInit(void);
void count(void);

unsigned char xdata leftPic  _at_ 0xFE00;
unsigned char xdata rightPic _at_ 0xFD00;

volatile char stop = 0;

volatile char leftPos = 2, rightPos = 0;

volatile char leftDigit = 2, rightDigit = 0;

volatile int ticks = 0;

//char pictograms[10] = {0x40, 0xF9, 0xA4, 0xB0, 0x99, 0x92,0x82,0xF8, 0x80, 0x90}; // Common Anode
char pictograms[14] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D,0x7D,0x07, 0x7F, 0x6F, 0x39, 0x76, 0x73, 0x40}; // Common Cathode


void main(void){
	intInit();
	timerInit();
	
	while(1){
		
		leftPic = pictograms[leftPos];
	  rightPic = pictograms[rightPos];
		/* Common Anode
			P2_1 = 0;
			P2_0 = 1;
			P0 = leftPic;
			P0 = 0xFF;
			P2_1 = 1;
			P2_0 = 0;
			P0 = rightPic;
			P0 = 0xFF;
		*/
	}
	
}

void intInit(void){
	IT0 = 1;
	IT1 = 1;
	EX0 = 1;
	EX1 = 1;
	EA = 1;
}

void timerReload(void){
	TR0 = 0; //stop timer 0
	TH0 = 0x4B;
	TL0 = 0xFD;
	if(stop == 0) TR0 = 1; //start timer 0
}

void setDefault(void){
	timerReload();
	leftDigit = 2;
	rightDigit = 1;
	ticks = 0;
}

void timerInit(void){
	TMOD = 0x01; //timer 0 mod 1 16 bit
	ET0 = 1; //timer 0 interrupts enable
	timerReload();
}

void count(void){
	rightDigit--;
	if(rightDigit < 0){
		rightDigit = 9;
		leftDigit--;
	}
	if(leftDigit < 0){
		leftDigit = 13;
		rightDigit = 13;
		stop = 1;
		P3_6 = 0;
		P3_6 = 1;
	}
	
	leftPos = leftDigit;
	rightPos = rightDigit;
}

void timer0Int(void) interrupt 1{
	ticks++;
	if(ticks == 17){
		count();
		ticks = 0;
	}
	timerReload();
}



void reset(void) interrupt 0{
	stop = 0;
	leftPos = 2;
	rightPos = 0;
	setDefault();
	P3_7 = 0;
	P3_7 = 1;
}

void readKeyboard(void) interrupt 2{
	unsigned char nr = P1;
	leftPos = 12;
	if(nr == 12) nr = 11;
	rightPos = nr;
	setDefault();
}

