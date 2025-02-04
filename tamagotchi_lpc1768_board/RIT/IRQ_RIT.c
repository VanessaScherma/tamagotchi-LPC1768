/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h" 
#include "../timer/timer.h"
#include "../TouchPanel/TouchPanel.h"
#include "../adc/adc.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int J_left;
extern int J_right;
extern int J_select;

extern int reset;
extern int action;
extern int manage_satiety;
extern int manage_happiness;
extern int i;
extern int sound;
extern int sound_reset;
extern int cuddles_flag;

extern int volume;

void RIT_IRQHandler (void)
{		
		enable_timer(3);
		ADC_start_conversion();
		if(volume == 1249) {
			volume_max(2,2);
		} else if(volume < 1249 && volume >= 800) {
			volume_2(2,2);
		} else if(volume < 800 && volume != 0) {
			volume_1(2,2);
		} else if(volume == 0) {
			volume_0(2,2);
		}
		
	/* Joytick Select pressed p1.25*/
	/* Joytick Left pressed p1.27 --> using J_left due to emulator issues*/
	/* Joytick Right pressed p1.28 --> using J_right due to emulator issues*/
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		J_left++;
		J_right = 0;
		sound = 1;
		LCD_DrawLine(0, 280, 120, 280, Red);
		LCD_DrawLine(120, 280, 120, 320, Red);
		LCD_DrawLine(120, 320, 0, 320, Red);
		LCD_DrawLine(0, 320, 0, 280, Red);
		LCD_DrawLine(120, 280, 240, 280, Black);
		LCD_DrawLine(240, 280, 240, 320, Black);
		LCD_DrawLine(240, 320, 120, 320, Black);
	}
	else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		J_right++;
		J_left = 0;
		sound = 1;
		LCD_DrawLine(120, 280, 240, 280, Red);
		LCD_DrawLine(240, 280, 240, 320, Red);
		LCD_DrawLine(240, 320, 120, 320, Red);
		LCD_DrawLine(120, 320, 120, 280, Red);
		LCD_DrawLine(0, 280, 120, 280, Black);
		LCD_DrawLine(120, 320, 0, 320, Black);
		LCD_DrawLine(0, 320, 0, 280, Black);
	} 
	else if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		J_select++;
		enable_timer(3);
		if(reset == 1) {
			LCD_Clear(White);
			start_game();
			enable_timer(0);
			reset = 0;
			sound = 0;
			sound_reset = 0;
			i=0;
			reset_timer(1);
		} else if(J_left != 0) {
			init_timer(1, 0, 3, 0x2FAF080);				//2s
			disable_RIT();
			action = 1;
			sound = 1;
			manage_satiety = 1;
			draw_Meal(5, 200);
			LCD_DrawRectangle(50, 120, 150, 150, White);
			totoro_eating(55, 130);
			LCD_DrawRectangle(40, 120, 150, 150, White);
			totoro_eating(45, 130);
			LCD_DrawRectangle(40, 120, 150, 150, White);
			totoro_eating(35, 130);
			enable_timer(1);
		} else if(J_right != 0) {
			init_timer(1, 0, 3, 0x2FAF080);				//2s
			disable_RIT();
			action = 1;
			sound = 1;
			manage_happiness = 1;
			draw_Snack(210, 208);
			LCD_DrawRectangle(50, 120, 150, 150, White);
			totoro_eating(80, 130);
			LCD_DrawRectangle(65, 120, 140, 150, White);
			totoro_eating(95, 130);
			LCD_DrawRectangle(75, 120, 130, 150, White);
			totoro_eating(110, 130);
			enable_timer(1);
		}
	}
	
	if(getDisplayPoint(&display, Read_Ads7846(), &matrix )) {
		if(display.x > 60 && display.x < 180 && display.y > 125 && display.y < 250) {
		init_timer(1, 0, 3, 0x2FAF080);				//2s
		disable_RIT();
		action = 1;
		manage_happiness = 1;
		totoro_cuddles(65,121);
		enable_timer(1);
		sound = 1;
		cuddles_flag = 1;
		} else {
			// do nothing
		}
	}
		
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
