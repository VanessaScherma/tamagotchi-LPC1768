/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../RIT/RIT.h"
#include "../TouchPanel/TouchPanel.h"

 
extern uint8_t hr, min, sec;
extern uint8_t MatchReg;
extern int reset;
extern int action;
extern int manage_satiety;
extern int manage_happiness;
extern int J_left;
extern int J_right;
extern int J_select;

volatile int flag_animation = 0;
volatile int happiness = 4;
volatile int satiety = 4;
volatile int count_happiness = 0;
volatile int count_satiety = 0;

extern int i;
extern int sound;
extern int sound_reset;
extern int cuddles_flag;

extern int volume;

// gestione dei suoni
#define	C	1062
#define B	1125
#define BB 1192
#define A 1263
#define AB 1339
#define	G 1417
#define	GB 1502
#define F 1592
#define E 1684
#define EB 1786
#define D 1890
#define DB 2006

#define	c	1062*2
#define b	1125*2
#define bb 1192*2
#define a 1263*2
#define ab 1339*2
#define	g 1417*2
#define	gb 1502*2
#define f 1592*2
#define e 1684*2
#define eb 1786*2
#define d 1890*2
#define db 2006*2

const int freqss[40]={1502,0, 1502,0, 1502,0, 2006 ,0, 1786 ,0, 1786 ,0, 2006,0, 2006,0, 1192,0, 1192,0, 1339,0, 1339,0, 1502, 0,0,0 };

const int click_sound = G;	
const int eating_sound[3] = {A,B,C};
const int cuddles_sound[8] = {F,E,F,E,D,D,E,A};
const int run_away_sound[5] = {B,D,B,D,C};
	
uint16_t SinTable[45] =                                       /*                      */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
//gestisce lo scorrere del tempo e le animazioni
void TIMER0_IRQHandler (void)
{		if(sec == 60 && min == 60) {
		hr++;
		min = 0;
		sec = 0;
		count_happiness++;
		count_satiety++;
	}
		else if(sec == 60) {
		min++;
		sec = 0;
		count_happiness++;
		count_satiety++;
	} else if(min == 60) {
		hr++;
		min = 0;
		count_happiness++;
		count_satiety++;
	} else {
		sec++;
		count_happiness++;
		count_satiety++;
	}
	refresh_LCD(hr, min, sec);
	
	if(count_happiness == 5) {
		happiness--;
		decrease_happiness(happiness);
		count_happiness = 0;
	}
	if(count_satiety == 5) {
		satiety--;
		decrease_satiety(satiety);
		count_satiety = 0;
	}
		
	//reset functionality
	if(happiness == 0 || satiety == 0) {
		reset = 1;
		sound_reset = 1;
		disable_timer(0);
		disable_timer(1);
		enable_timer(3);
		disable_RIT();
		LCD_DrawRectangle(0, 100, 240, 170, White);
		run_away(100, 130);
		LCD_DrawRectangle(80, 120, 150, 150, White);
		run_away(180, 130);
		LCD_DrawRectangle(100, 120, 140, 150, White);
		balloon(50, 130);
		LCD_DrawRectangle(0, 280, 240, 320, White);
		LCD_DrawLine(0, 280, 239, 280, Red);
		LCD_DrawLine(239, 280, 239, 319, Red);
		LCD_DrawLine(239, 319, 0, 319, Red);
		LCD_DrawLine(0, 319, 0, 280, Red);
		GUI_Text(100, 293,(uint8_t *) "Reset", Black, White);
		action = 0;
		happiness = 4;
		satiety = 4;
		hr = 0;
		min = 0;
		sec = 0;
		count_happiness = 0;
		count_satiety = 0;
		manage_happiness = 0;
		manage_satiety = 0;
		cuddles_flag = 0;
		J_left = 0;
		J_right = 0;
		J_select = 0;
		enable_RIT();
	}
	
	if (reset == 1) {
		//skip animation
	} else if (action == 1) {
		//skip animation
	} else if(flag_animation == 0) {
		draw_Totoro_Animation(65, 130);
		flag_animation = 1;
	} else if(flag_animation == 1) {
		draw_Totoro(65, 130);
		flag_animation = 0;
	}
	LPC_TIM0->IR = 1;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
//gestisce la sazietà e la felicità
void TIMER1_IRQHandler (void) {
	action = 1;
	disable_timer(1);
	if (manage_satiety == 1) {
		if(satiety < 4 && reset == 0) {
			satiety++;
			increase_satiety(satiety);
		}
		if(reset == 0) {
			LCD_DrawRectangle(5, 100, 150, 170, White);
			enable_RIT();
			reset_timer(1);
			count_satiety = 0;
		} else {
			LCD_DrawRectangle(0, 100, 50, 170, White);
		}
	}	else if (manage_happiness == 1) {
		if(happiness < 4 && reset == 0) {
			happiness++;
			increase_happiness(happiness);
		}
		if(reset == 0) {
			LCD_DrawRectangle(60, 91, 170, 179, White);
			enable_RIT();
			reset_timer(1);
			count_happiness = 0;
		} else {
			LCD_DrawRectangle(100, 100, 140, 170, White);
		}
	}
	action = 0;
	manage_satiety = 0;
	manage_happiness = 0;
	sound = 0;
	LPC_TIM1->IR = 1;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = (uint16_t)(volume*SinTable[ticks]/1000)<<6;
	ticks++;
	if(ticks==45) ticks=0;
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
}


/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void)
{
	if(LPC_TIM3->IR & 1){
		disable_timer(2);
		reset_timer(2);
		
		if(reset == 1) {
			if(cuddles_flag == 1 || manage_happiness == 1 || manage_satiety == 1 || J_left != 0 || J_right != 0) {
				cuddles_flag = 0;
				manage_happiness = 0;
				manage_satiety = 0;
				J_left = 0;
				J_right = 0;
				disable_timer(2);
				reset_timer(2);
				i=0;
				sound = 0;
			} else if(i==5) {
				i=0;
				disable_timer(2);
				reset_timer(2);
				sound_reset = 0;
				sound = 0;
			} 
		} else if((manage_happiness == 1 || manage_satiety == 1) && cuddles_flag == 0) {
			if(i==3) {
				i=0;
				disable_timer(2);
				reset_timer(2);
				sound = 0;
			} 
		} else if(cuddles_flag == 1) {
			if(i==8) {
				i=0;
				disable_timer(2);
				reset_timer(2);
				sound = 0;
				cuddles_flag = 0;
			} 
		} else if(J_left != 0 || J_right != 0) {
			if(i==1) {
				i=0;
				disable_timer(2);
				reset_timer(2);
				sound = 0;
			}
		}
		
		if(run_away_sound[i]!=0  && reset == 1 && sound_reset == 1) {
			init_timer(2, 0, 3, run_away_sound[i]);
			enable_timer(2);
			i++;
		} else if(cuddles_sound[i]!=0  && cuddles_flag == 1 && sound == 1) {
			init_timer(2, 0, 3, cuddles_sound[i]);
			enable_timer(2);
			i++;
		} else if(eating_sound[i]!=0 && (manage_happiness == 1 || manage_satiety == 1) && sound == 1) {
			init_timer(2, 0, 3, eating_sound[i]);
			enable_timer(2);
			i++;
		} else if(click_sound!=0 && (J_left != 0 || J_right != 0) && sound == 1){
			init_timer(2, 0, 3, click_sound);
			enable_timer(2);
			i++;
		}
			
			
			LPC_TIM3->IR = 1;
		}
}
/******************************************************************************
**                            End Of File
******************************************************************************/
