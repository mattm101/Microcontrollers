#include <REGX52.H>

#define Test 0
#define False    			0  
#define True     			1

#define UP 0
#define DOWN 1
#define LEFT 3
#define RIGHT 4

#define LCD_RS    P2_4         /* p1.4 LCD Register Select line           */
#define LCD_RW    P2_5         /* p1.5 LCD Read / Write line              */
#define LCD_E     P2_6         /* p1.6 LCD Enable line                    */
#define LCD_DB4   P2_0         /* high nibble of port 1 is used for data  */
#define LCD_DB5   P2_1         /* high nibble of port 1 is used for data  */
#define LCD_DB6   P2_2         /* high nibble of port 1 is used for data  */
#define LCD_DB7   P2_3         /* high nibble of port 1 is used for data  */

#define LCD_CONFIG    		0x28
#define LCD_CLEAR     		0x01
#define LCD_HOME      		0x02
#define LCD_ENTRY_MODE		0x06
#define LCD_DISPLAY_OFF		0x08
#define LCD_CURSOR_ON 		0x0A
#define LCD_CURSOR_OFF 		0x0C
#define LCD_DISPLAY_ON		0x0C
#define LCD_CURSOR_BLINK	0x0D
#define LCD_CURSOR_LINE		0x0E
#define LCD_CURSOR_COM		0x0F
#define LCD_CURSOR_LEFT		0x10
#define LCD_CURSOR_RIGHT	0x14
#define LCD_SHIFT_LEFT		0x18
#define LCD_SHIFT_RIGHT		0x1C
#define LCD_SET_CGRAM_ADDR	0x40
#define LCD_SET_DDRAM_ADDR	0x80

extern w0ms();
extern w1ms();
extern w5mS();
extern w50mS();

void placeCoin(void);
void snakeInit(void);
static void Lcd_WriteControl (unsigned char LcdCommand);
static void Lcd_WriteData (unsigned char LcdData);
void Lcd_DisplayCharacter (char a_char);
void Lcd_Cursor (char row, char column);
void Lcd_DisplayRow (char row, char *string);
void Lcd_DisplayScreen (char *ptr);
void LcdInit(void);
void timerReload(void);
void serialInit(void);
void gameLoop(void);

unsigned char data Var1, Var2, Var3;
sbit sendReq = P3^4;
char oneByte;
volatile char write = 0;

static unsigned char code TitleScreen[] =  "***   SNAKE  ***"
                                           "       by       "
                                           "  Mateusz Malec "
																					 "****************";

char ticks = 0;

volatile char direction = LEFT;

struct SpacePoint{
	int x;
	int y;
} snake[10];

char coinPos[4][2] = {{1,1}, {4,16}, {1,16}, {4,1}};

struct SpacePoint coin;

char coinNr = 0;

char dl = 3;

void main(void){
	LcdInit();
	serialInit();
	snakeInit();
	Lcd_DisplayScreen(TitleScreen); 
	while(1){}
}

void placeCoin(void){
	
	coin.x = coinPos[coinNr][1];
	coin.y = coinPos[coinNr][0];
	coinNr++;
	coinNr %= 4;
}

void snakeInit(void){
	char i = 0;
	Lcd_DisplayScreen(TitleScreen);
	for(i=0;i<10;i++){
		w50mS();
	}
	direction = LEFT;
	coinNr = 0;
	placeCoin();
	dl = 3;
	for(i=0;i<10;i++){
		snake[i].x = i;
		snake[i].y = 0;
	}
}

// command bus 4-bit
static void Lcd_WriteControl (unsigned char LcdCommand)
 {
   unsigned char Lcd_Comm = 0;
   static bit LCDReady;
    
   LCD_RS = False;
   LCD_RW = False;

   Lcd_Comm = LcdCommand >> 4;
   P2 &= 0xF0;
   P2 |= Lcd_Comm;

   LCD_E = True;
	 w0ms();
   LCD_E = False;
	 w0ms();
   Lcd_Comm = LcdCommand & 0x0F;  
   P2 &=0xF0;  
   P2 |= Lcd_Comm;
	 w0ms();
   LCD_E = True;
	w0ms();
   LCD_E = False;
	 w0ms();

   P2 |= 0x0F;

   LCD_RW = True;
   LCD_RS = False;
   
   if (Test == 0)
    {
      LCDReady = 1;
      while (LCDReady == 1)
       {
         LCD_E = True;
				
				 w0ms();
         LCDReady = LCD_DB7;
         LCD_E	= False;
				 
				 w0ms();
         LCD_E = True;
				 
				 w0ms();
         LCD_E	= False;
				 
				 w0ms();
       }
    }          
 }

//data bus 4-bit
static void Lcd_WriteData (unsigned char LcdData){
   unsigned char Lcd_Data = 0;
   static bit LCDReady;
 
   LCD_RS = True;
   LCD_RW = False;

   Lcd_Data = LcdData >> 4;
   P2 &= 0xF0;
   P2 |= Lcd_Data;

   LCD_E = True;
	 
	 w0ms();
   LCD_E = False;
	
	 w0ms();
   Lcd_Data = LcdData & 0x0F;  
   P2 &=0xF0;   
   P2 |= Lcd_Data;
		
	 w0ms();
   LCD_E = True;
	
	w0ms();
   LCD_E = False;
	
		w0ms();
   P2 |= 0x0F;

   LCD_RW = True;
   LCD_RS = False;
   
   if (Test == 0)
    {
      LCDReady = 1;
      while (LCDReady == 1)
       {
         LCD_E = True;
			
				 w0ms();
         LCDReady = LCD_DB7;
         LCD_E	= False;
				 
				 w0ms();
   	     LCD_E	= True;
				
				 w0ms();
   	     LCD_E	= False;
       }
    }             
 }

void Lcd_DisplayCharacter (char a_char)
 {
   Lcd_WriteData(a_char);
   Lcd_WriteControl (LCD_CURSOR_OFF);
 }

/* cursor position values
*     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16
*    ----------------------------------------------------------------
* 1 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
* 2 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
* 3 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
* 4 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
*/
void Lcd_Cursor (char row, char column)
 {
   if (row == 1) Lcd_WriteControl (0x80 + column - 1);
   if (row == 2) Lcd_WriteControl (0xc0 + column - 1);
   if (row == 3) Lcd_WriteControl (0x90 + column - 1);
   if (row == 4) Lcd_WriteControl (0xd0 + column - 1);
 }

void Lcd_DisplayRow (char row, char *string){
   char i;
   Lcd_Cursor (row, 1);
   for (i=0; i<16; i++) Lcd_DisplayCharacter (*string++);
 }

void Lcd_DisplayScreen (char *ptr){
   Lcd_DisplayRow(1,ptr +  0);
   Lcd_DisplayRow(2,ptr + 16);
   Lcd_DisplayRow(3,ptr + 32);
   Lcd_DisplayRow(4,ptr + 48);
 }

 

void LcdInit(void){
   w50mS();
   P2 = 0x83;
   LCD_E   = True;
    
	 w1ms();
   LCD_E   = False;
	
   w5mS();
   LCD_E   = True;
	 
     w1ms();
   LCD_E   = False;
	
   w1ms();
   LCD_E   = True;
	
     w1ms();
   LCD_E   = False;
	 
   w1ms();
   LCD_DB4 = False;
   LCD_E   = True;
	 
   w1ms();
   LCD_E   = False;

   w1ms();
   Lcd_WriteControl(LCD_CONFIG);     
   Lcd_WriteControl(LCD_CLEAR);      
   Lcd_WriteControl(LCD_DISPLAY_OFF);
   Lcd_WriteControl(LCD_DISPLAY_ON); 
   Lcd_WriteControl(LCD_ENTRY_MODE); 
   Lcd_WriteControl(LCD_CURSOR_COM); 
   Lcd_WriteControl(LCD_CLEAR);      
 }

void timerReload(void){
	TR0 = 0; //stop timer 0
	TH0 = 0x00;
	TL0 = 0x00;
	TR0 = 1; //start timer 0
}
 
void serialInit(void){
	SCON = 0x50;
	PCON = 0x80;
	TMOD = 0x21;
	TH1 = 0xFA; //9600 bods
	sendReq = 0;
	ET0 = 1;
	ES = 1;
	EA = 1;
	TR1 = 1;
	timerReload();
}

void gameLoop(void){
	char i;
	for(i=dl-1;i>=1;i--){
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	switch(direction){
		case UP:
			snake[0].y--;
		break;
		
		case DOWN:
			snake[0].y++;
		break;
		
		case LEFT:
			snake[0].x--;
		break;
		
		case RIGHT:
			snake[0].x++;
		break;
	}
	if(snake[0].x < 1) snake[0].x = 16;
	else if(snake[0].x > 16) snake[0].x = 1;
	
	if(snake[0].y < 1) snake[0].y = 4;
	else if(snake[0].y > 4) snake[0].y = 1;
	Lcd_WriteControl(LCD_CLEAR); 
	for(i=0;i<dl;i++){
		Lcd_Cursor (snake[i].y,snake[i].x);
		Lcd_DisplayCharacter(0xDB);
	}
	
	if(snake[0].x == coin.x && snake[0].y == coin.y){
		dl++;
		placeCoin();
	}
	
	Lcd_Cursor (coin.y,coin.x);
	Lcd_DisplayCharacter(0xEC);
}

void timer0Int(void) interrupt 1{
	ticks++;
	if(ticks == 10){
		gameLoop();
		ticks = 0;
	}
	timerReload();
}

void receive(void) interrupt 4{
  	if (RI == 1){
	  	RI = 0;
      oneByte = SBUF;
			switch(oneByte){
				case 2:
					if(direction != DOWN) direction = UP;
				break;
				
				case 8:
					if(direction != UP) direction = DOWN;
				break;
				
				case 4:
					if(direction != RIGHT) direction = LEFT;
				break;
				
				case 6:
					if(direction != LEFT) direction = RIGHT;
				break;
				
				case 16:
					snakeInit();
				break;
			}
	  }
  }


	

	


