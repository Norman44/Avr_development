/*
 * v7.c
 *
 * Created: 04-Apr-17 14:49:45
 * Author : maticpi
 */ 

//#include <util/delay.h>
#include <avr/io.h>
#include "LCD_Ili9341.h"
#include "systime.h"
#include "kbd.h"
#include "BackgroundServices.h"

void Init_IO();
void Game();
int WaitForStart();
int PlayTheGame();
int EndOfGame();
char GetRandomMoleNumber();

int main(void)
{
	Init_IO();
	LCD_Init();
	Systime_Init();
	sei();
	
    /* Replace with your application code */
    while (1)
    {
		if (HasOneMillisecondPassed())
		{
			Game();
		}
		BackgroundServices();
    }
}

void Game()
{
	static int state;
	int result;

	switch (state)
	{
		case 1:	//waiting for game start
			result = WaitForStart();
			if (result == 1) state++;
			break;
		case 2: //Play the game
			result = PlayTheGame();
			if (result == 1) state++;
			break;
		case 3: //Display the score
			result = EndOfGame();
			if (result == 1) state=0;
			break;
		default: //0 or unknown value: reset the game
			state = 1;
			break;
	}
}

int WaitForStart()
{
	static int state;
	char key;
	static char krt;
	static uint32_t zaznamekCasa;
	
	int result=0;
	switch (state)
	{
		case 0: //stanje inicializacije
			display_msg = DISP_START_SCREEN;
			KBD_flush(); //sprazni buffer za tipke
			krt=1;
			zaznamekCasa = GetSysTick();
			state++;
			break;
		case 1:
			key = KBD_GetKey(); //kbd read dela background services
			if (key == BTN_OK){
				state++;
				break;
			}
			if (Has_X_MillisecondsPassed(500, &zaznamekCasa)){
				krt++;//krt++
				if (krt>4) krt=1; //èe je krt veèji od 4, krt = 1
				display_msg=krt;//display_msg = krt;
			}
			break;
		case 2: //konec, testiraj avtomat stanj
			result = 1;
			state = 0;
			break; 
	}
	//TODO: write the program
	
	return result;
}

int PlayTheGame()
{
	
	char key;
	static char krt;
	static uint32_t zaznamekCasa;
	static int state;
	
	
	int result = 0;
	
	switch (state)
	{
		case 0: //init
			score = 0;
			level = 1;
			lives = 3;
		
			display_msg = DISP_GAME_PLAY_SCREEN;
			state++;
			break;
			
		case 1: //doloèi krta
			krt = GetRandomMoleNumber();//get the random value
			display_msg = krt;
			zaznamekCasa = GetSysTick();
			state++;
			break;
			
		case 2: // glej tipke in èas
			key = KBD_GetKey();
			if (key != 0){
				switch (key)
				{
					case BTN_OK:
						if (krt == 1) state = 10;
						else state = 20;
						break;
						
					case BTN_ESC:
						if (krt==2) state =10;
						else state = 20;
						break;
						
					case BTN_A:
						if (krt == 3) state = 10;
						else state = 20;
						break;
					
					case BTN_B:
						if (krt == 4) state = 10;
						else state = 20;
						break;
					
					default:
						state = 20;
						break;		
				}
				break;
			}
			
			while (level == 1){
				
				if (Has_X_MillisecondsPassed(2000, &zaznamekCasa)){ //ali je èas potekel
			
					state = 20;
					break;
				} 
				break;
				}
				
			while (level == 2){
				
				if (Has_X_MillisecondsPassed(1000, &zaznamekCasa)){ //ali je èas potekel
					
					state = 20;
					break;
				}
				break;
			}
			
			while (level == 3){
				
				if (Has_X_MillisecondsPassed(900, &zaznamekCasa)){ //ali je èas potekel
					
					state = 20;
					break;
				}
				break;
			}
			
			while (level == 4){
				
				if (Has_X_MillisecondsPassed(800, &zaznamekCasa)){ //ali je èas potekel
					
					state = 20;
					break;
				}
				break;
			}
			
			while (level == 5){
				
				if (Has_X_MillisecondsPassed(700, &zaznamekCasa)){ //ali je èas potekel
					
					state = 20;
					break;
				}
				break;
			}
			break;
			
		
		case 10: //krt je praviln
			score = score + (level * 2);
			switch (score)
				{
					case 0:
						//display_msg = DISP_LEVEL;
						break;
					case 20:
						level++;
						//display_msg = DISP_LEVEL;
						break;
					case 60:
						level++;
						//display_msg = DISP_LEVEL;
						break;
					case 120:
						level++;
						//display_msg = DISP_LEVEL;
						break;
					case 200:
						level++;
						break;
					case 300:
						level++;
						break;
				}
				
			display_msg = DISP_REFRESH_ALL;
			state++;
			//poveèaj levl za domaèo nalogo
			break;
			/*if (level != 0)
				switch (level)
				{
					case 1:
						score++;
						if (score > 10) level++;
						//display_msg = DISP_REFRESH_ALL;
						break;
					case 2:
						score = level * 2;
						if (score > 40) level++;
						//display_msg = DISP_REFRESH_ALL;
						break;
					case 3:
						score*/ 
				
			 //povèaj toèke
			/*if (score != 0){
				switch (score)
				{
					case 10;
					
						level++;
						
					case 20:
				}
				 level++;*/
			
		case 11: //briši krta
			display_msg = DISP_MOLE_NONE;
			state=1;
			break;

		case 20: //krt je napaèn ali timeout
			lives--;//pomanjšaj live
			if (lives > 0) {
				state = 1;
				display_msg = DISP_REFRESH_LIVES;
				break;
			} else {
				display_msg = DISP_END_SCREEN;	
				state = 0; 
				result = 1;
				break;
			}
			break;
		default: state = 0; result = 1; break;
			}
	return result;
	
}

int EndOfGame()
{
	char key;
	//static int state;
	int result = 1;
	key = KBD_GetKey();
	
	display_msg = DSIP_END_SCORE;
	if (key == BTN_OK){
		//display_msg = DISP_REFRESH_ALL;
		//display_msg = DSIP_END_SCORE;
		return result;
	} 
	return result;
}
	/*
	display_msg = DSIP_END_SCORE;
	if (key != 0){
		switch (key)
		{
			case BTN_OK:
			return result;
			break;
		}
	} 
	*/
	
	/*
	switch (state)
	{
		case 0:
		display_msg = DSIP_END_SCORE;
		state++;
		break;
		case 1: 
		key = KBD_GetKey();
			if (key != 0){
			switch (key)
			{
				case BTN_OK:
				return result;
				break;
			}
			break;
			}
		
	}
	return result;
	*/
	
	//display_msg = DISP_REFRESH_ALL;
	//display_msg = DSIP_END_SCORE;
	
	//return result;
	
	/*
	char key;
	int result=0;
	//static int state;
	key = KBD_GetKey();
	if (KBD_isKeyStatePressed(BTN_OK))
	{
		display_msg = DSIP_END_SCORE;
		result = 1;
	} else {
		result = 1;
	}
	//return result;
	*/
	/*
	switch (state){
		case 0:
			KBD_flush(); //empty buffer 
			//display_msg = DISP_REFRESH_ALL;
			display_msg = DSIP_END_SCORE;
			return result;
			state++;
			break;
		case 1:
			key = KBD_GetKey(); //kbd read dela background services
			if (key == BTN_OK){
				//display_msg = DISP_REFRESH_ALL;
				display_msg = DSIP_END_SCORE;
				return result;
				break;
			}
			break;
				
		
	}
	return result;
	*/
	/*
	key = KBD_GetKey();
	if (key == BTN_OK) {
		//display_msg = DISP_REFRESH_ALL; 
		return result;
	}else{
	display_msg = DISP_REFRESH_ALL;
	display_msg = DSIP_END_SCORE;
	
	
	//TODO: write the program
	
	return result;
	}*/

char GetRandomMoleNumber()
{
	char rand_mole;
	//since there is no telling when the user will actually start the game,
	//the first two bits of system time are completely random
	rand_mole = GetSysTick();
	rand_mole = (rand_mole & 0x03)+1;
	return rand_mole;
}

void Init_IO()
{
	//DDR: 1-out, 0-in
	PORTB = 0x12;	//0-LCD_DC, 1-buzzer, 2-LCD_CS,
	DDRB = 0x2D;	//3-MOSI, 4-MISO, 5-SCK, 6,7-not implemented
	
	PORTC = 0xF0;	//0..3-analog inputs
	DDRC = 0x00;	//4-SDA(not connected), 5-SCL(not connected), 6,7-not implemented
	
	PORTD = 0xBF;	//0-Rx, 1-Tx, 2..5-buttons
	DDRD = 0xC2;	//6-LCD_BACKLIGHT, 7-LCD_RESET
	
	PORTE = 0x00;	//LEDs
	DDRE = 0xFF;
}