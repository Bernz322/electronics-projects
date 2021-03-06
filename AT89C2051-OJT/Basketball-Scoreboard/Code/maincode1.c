/* ----HEADERS---- */
#include<stdio.h>
#include <reg51.h>						
#define SEVENSEGMENT P1	// All Port1 is for the 7-segment display called "SEVENSEGMENT" (A,B,C,D,E,F,G)

// JEFFREY BERNADAS
/* This project uses an Encoder IC called 74LS148 to cater the needed number of buttons as the MCU AT89C2051 only has limited number of pins */
/* This project uses a Decoder IC called 74154 to cater the needed number of 7-segment displays as the MCU AT89C2051 only has limited number of pins */
/* In short */
/* Encoder 74LS148 is for the buttons */
/* 74LS154 Truth Table for each pins can be found in "https://makeyourownchip.tripod.com/74154.html" */
/* 74LS148 Truth Table for each pins can be found in "https://www.electronics-tutorials.ws/combination/comb_4.html" */

/* ----PINS CONFIG---- */

// 74LS154 IC
sbit dataA = P3^2;	// 74154 A Pin
sbit dataB = P3^3;	// 74154 B Pin
sbit dataC = P3^4;	// 74154 C Pin
sbit dataD = P3^5;	// 74154 D Pin

// 74LS148 IC
sbit A0 = P3^7;	//	74LS148 A0 Pin
sbit A1 = P3^1;   // 74LS148 A1 Pin 
sbit A2= P3^0;   // 74LS148 A2 Pin 

void delay(int n) {	// Delay Function
	while(n--);
}
void main (){

	char SevenSegDigit[]={0xc0,0xf9 ,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9] Array HEX for Common Anode 7 Seg Display

	/* ----Declaring of Variables---- */
	
	// For HOME SCORE
	int homeScore = 0;
	int homeScoring = 0;	// Will serve as a button debounce
	
	// For GUEST SCORE	
	int guestScore = 0;
	int guestScoring = 0;	// Will serve as a button debounce
	
	// For the Shot Clock
	float shotClock = 24;
	
	// For the Game Clock
	float gameClock = 1000;
	
	// For the Quarter
	int quarter = 1;	// Default should be 1
	
	// For Pausing both the Game and Shot Clocks
	int pause = 0;
	int pausing = 0;
	int adding = 0;	// Will serve as a button debounce to avoid doubling the function
	
		/* ----Main Functionalities in a loop ---- */
		while(1){
			/* ----Calculations for every required function---- */
			
			/* ----Game Clock and Shot Clock Digits Calculations---- */
			
			int shotClockTimer = (int)shotClock;	// 24
			int gameClockTimer = (int)gameClock;	// 1000
			
			/*-Shot Clock- [24]*/
			int shotClockDigitOne = shotClockTimer%10;	// [24%10 = 4]	= Digit 1	
			int v = shotClockTimer/10; // [24/10 = 2]
			int shotClockDigitTwo = v%10;	// [2%10 = 2]	= Digit 2
			
			/*-Game Clock- [10:00]*/
			int gameClockDigitOne = gameClockTimer%10;	// [1000 % 10 = 0]	= Digit 1
			int o = gameClockTimer/10;	// [1000/10 = 100]
			int gameClockDigitTwo = o%10;	// [100%10 = 0]	= Digit 2
			int p = o/10;	// [100/10 = 10]
			int gameClockDigitThree = p%10;	// [10%10 = 0]	= Digit 3
			int gameClockDigitFour = p/10; // [10/10 = 1]	= Digit 4
			
			/* ----Home and Guest Scores Digits Calculations---- */	
			
			/*-Default Score - [X000]*/
			/*-Increment Score by 1 should result to [X001]*/
			/*-Incrementing Score to 11 should result to [X011]*/
			int homeScoreDigitOne = homeScore%10;	// [0%10 = 0]	: [1%10 = 1] : [11%10 = 1]	= Digit 1
			int n = homeScore/10;	// [0/10 = 0]	:	[1/10 = 0] : [11/10 = 1]
			int homeScoreDigitTwo = n%10;	// [0%10 = 0]	:	[0%10 = 0] : [1%10 = 1]	= Digit 2
			int homeScoreDigitThree = n/10;	// [0/10 = 0]	:	[0/10 = 0] : [1/10 = 0]	= Digit 3
			
			/* -Guest Score- */
			int guestScoreDigitOne = guestScore%10;	// [0%10 = 0]
			int y = guestScore/10;	// [0/10 = 0]
			int guestScoreDigitTwo = y%10;	// [0%10 = 0]
			int guestScoreDigitThree = y/10;	// [0/10 = 0]
			
			
			
			/********************************/
			/* ----BUTTONS from 74LS148---- */
			/********************************/
			
			// HOME SCORE
			// If corresponding btn is pressed, increment Home Score
			if(A0==0 && A1==1 && A2==1 && homeScoring ==0){	// If 74LS148 Pin 1 is LOW, increment homeScore variable
				homeScore = homeScore + 1;
				homeScoring = 1;	// This is where debounce comes in just to avoid doubling this "if block" of incrementing home score. This applies to all debounce as well. Same idea.
			}
			
			// GUEST SCORE
			// If corresponding btn is pressed, increment Guest Score 
			if(A0==1 && A1==0 && A2==1 && guestScoring ==0){	// If 74LS148 Pin 2 is LOW, increment guestScore variable
				guestScore = guestScore + 1;	
				guestScoring = 1;	// Another debounce. 
			}
			
			// SHOT CLOCK
			// If corresponding btn is pressed, reset Shot Clock
			if(A0==0 && A1==0 && A2==1){	// If 74LS148 Pin 3 is LOW, set shotClock variable to default w/c is 24
				shotClock = 24;
			}
			
			// GAME CLOCK
			// If corresponding btn is pressed, reset Game Clock
			if(A0==1 && A1==1 && A2==0){	// If 74LS148 Pin 4 is LOW, set gameClock variable to 960
				gameClock = 960;
			}
			
			// GAME QUARTER
			// If corresponding btn is pressed, increment quarter by 1
			if(A0==1 && A1==0 && A2==0 && adding==0){	// Pin 6
				if(quarter<4){	// Increment if value is less than 4 else, reset quarter to 1
					quarter = quarter + 1;
					adding = 1;	// Another debounce
				}else{
					quarter = 1;
					adding = 1;
				}
			}
			
			// PAUSE BOTH SHOT and GAME CLOCKS
			// If corresponding btn is pressed, and pause is false, then increment pause to return true (1).
			// The new returned pause variable will enable pausing of the clock ( Function for it is in the bottom part of the code)
			if(A0==0 && A1==1 && A2==0 && pausing ==0){	// Pin 5
				if(pause==0){
					pause = pause +1;
					pausing = 1;
				}else{
					pause = pause -1;
					//shotClock=25;
					pausing = 1;
				}
			}
			
			// BUTTON DEBOUNCE
			/* To ensure that a btn is only pressed ONCE, this is used to every specific "if conditions" of every buttons which means that this "if" will 
			 always execute last and ensure to reset all values back to 0 which is required to all if statements above */
			if(A0==1 && A1==1 && A2==1){
				homeScoring = 0;	// reset this variable to 0
				guestScoring = 0;	// reset this variable to 0
				pausing = 0;	// reset this variable to 0
				adding = 0;	// reset this variable to 0
			}			
			
			
			
			/********************************************/
			/* --DISPLAY Digits to 7seg using 74LS154-- */
			/********************************************/
			
			/*------------------HOME SCORE------------------*/
			
			// 1st DIGIT
			/*Start of Blank*/
			// This blank is the pin 6 of the IC which means that the MCU will connect to this empty pin for 10 ms and then show the real digit to the certain pin for 100 ms
			// This will run always to avoid having similar digit to a 4-digit Seven Segment Display
			/* Note that this is very vital to the whole project. This will give an effect to our naked eye that the 4 digit 7 segment display is displaying the correct digit
			 as it runs very fast creating an illusion. (Same idea to the flourescent lamp or any other lights blinking too fast that our naked eyes can't notice.) */
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[homeScoreDigitOne];	// What to display in the 7 seg (from the Array) If homeScoreDigitOne value is "1" then SevenSegDigit[1]
			/* 74154 Pin 0*/
			dataA = 0;
			dataB = 0;
			dataC = 0;
			dataD = 0;
			delay(100);
			/*end*/
			
			// 2nd DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[homeScoreDigitTwo];	// What to display in the 7 seg (Array based)
			/* 74154 Pin 1*/
			dataA = 1;
			dataB = 0;
			dataC = 0;
			dataD = 0;
			delay(100);
			/*end*/
			
			// 3rd DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			/* 74154 Pin 2*/
			SEVENSEGMENT = SevenSegDigit[homeScoreDigitThree];	// What to display in the 7 seg (Array based)
			dataA = 0;
			dataB = 1;
			dataC = 0;
			dataD = 0;
			delay(100);
			/*end*/
			
			/*------------------END of HOME SCORE------------------*/
			
			/*------------------GUEST SCORE------------------*/
			
			// 1st DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[guestScoreDigitOne];	// What to display in the 7 seg (Array based)
			/* 74154 Pin 3*/
			dataA = 1;
			dataB = 1;
			dataC = 0;
			dataD = 0;
			delay(100);
			/*end*/
			
			// 2nd DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[guestScoreDigitTwo];	// What to display in the 7 seg (Array based)
			/* 74154 Pin 4*/
			dataA = 0;
			dataB = 0;
			dataC = 1;
			dataD = 0;
			delay(100);
			/*end*/
			
			// 3rd DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[guestScoreDigitThree];	// What to display in the 7 seg (Array based)
			/* 74154 Pin 5*/
			dataA = 1;
			dataB = 0;
			dataC = 1;
			dataD = 0;
			delay(100);
			/*end*/
			
			/*------------------END of GUEST SCORE------------------*/
			
			/*----------------------SHOT CLOCK----------------------*/
		
			// 1st DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[shotClockDigitOne];	// 4
			/* 74154 Pin 7*/
			dataA = 1;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(100);
			/*end*/
			
			// 2nd DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[shotClockDigitTwo];	//	2
			/* 74154 Pin 8*/
			dataA = 0;
			dataB = 0;
			dataC = 0;
			dataD = 1;
			delay(100);
			/*end*/
			
			/*--------------------END of SHOT CLOCK---------------------*/		
			
			/*----------------------GAME CLOCK----------------------*/
			
			// 1st DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[gameClockDigitOne];
			/* 74154 Pin 9*/
			dataA = 1;
			dataB = 0;
			dataC = 0;
			dataD = 1;
			delay(100);
			
			// 2nd DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[gameClockDigitTwo];
			/* 74154 Pin 10*/
			dataA = 0;
			dataB = 1;
			dataC = 0;
			dataD = 1;
			delay(100);
			/*End of Blank*/
			
			// 3rd DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[gameClockDigitThree];
			/* 74154 Pin 11*/
			dataA = 1;
			dataB = 1;
			dataC = 0;
			dataD = 1;
			delay(100);
			/*End of Blank*/
			
			// 4th DIGIT
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[gameClockDigitFour];
			/* 74154 Pin 12*/
			dataA = 0;
			dataB = 0;
			dataC = 1;
			dataD = 1;
			delay(100);
			/*End of Blank*/
			
			/*--------------------END of GAME CLOCK---------------------*/	
			
			/*------------------QUARTER------------------*/
			
			/*Start of Blank*/
			dataA = 0;
			dataB = 1;
			dataC = 1;
			dataD = 0;
			delay(10);
			/*End of Blank*/
			SEVENSEGMENT = SevenSegDigit[quarter];
			/* 74154 Pin 13*/
			dataA = 1;
			dataB = 0;
			dataC = 1;
			dataD = 1;
			delay(100);
			/*End of Blank*/
			
			/*------------------END of QUARTER------------------*/	
			
			/*----------------COUNTDOWN/ TIMER DECREMENT----------------*/
			
			if(pause==0){	// if not paused, decrement shot clock by 0.09
				if(shotClock>0){
					shotClock = shotClock - 0.09; 
				 }
			 }
			
			if(pause==0){	// if not paused, decrement game clock by 0.09
				if(gameClock>0){
					if(gameClockDigitTwo==0 && gameClockDigitOne==0){
						gameClock = gameClock - 40;
					}
				gameClock = gameClock - 0.09;
				}
			}
			
			/*-------------END of COUNTDOWN/ TIMER DECREMENT-------------*/		
		}
}
 
