/*
 * main.c
 *
 *  Created on: Sep 25, 2022
 *      Author: General
 */




#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "util/delay.h"
#include "DIO_Interface.h"
#include "UART_Interface.h"
#include "TIMER1_int.h"
#include "TWI_int.h"
#include "EXT_EEPROM_int.h"
#include "LCD_Interface.h"
#include"string.h"

#ifndef F_CPU
#define F_CPU 8000000UL
#endif


void System_Init(void);
void Array_Zero_Init(u8* data);
void BUZZ(void);


int main()
{

	u8 NAME[][8]={{"Ahmed"},{"Mohamed"}};
	u8 UART_DATA[10] ={0} ;
	u8 DATA2[10] ={0} ;
	u8 NAME_INDX[2] ={0,20} ;
	u8 PASS_INDX[2] ={10,30} ;
	u8 i=0 ;
	u8 PASS_COUNT=3 ;


	System_Init();

	while(1)
	{
		LCD_voidClear();
		LCD_voidGoToXY(0,2);
		LCD_voidWriteString((u8*)"Door closed []");
		LCD_voidGoToXY(1,2);
		LCD_voidWriteString((u8*)"Enter Your Name");

		/*Receive Name*/
		UART_VoidReceiveString(UART_DATA);

		/*check Name if in list in eeprom - we have 2 user ahmed - mohamed */
		for(i=0;i<2;i++)/* start loop for(i=0;i<2;i++)*/
		{

			/* go inside eeprom and Read name in list ->Iterates over the number of users until matching  */
			H_EXT_EEPROM_void_Read_Str(NAME_INDX[i] , DATA2 ) ;


			if(strcmp((char*)UART_DATA ,(char*) DATA2)==0)/* startof-> if(strcmp(UART_DATA , DATA2)==0)*/
			{/* go inside here if name = any name in list */

				Array_Zero_Init(DATA2);
				LCD_voidClear();
				LCD_voidGoToXY(0,3);
				LCD_voidWriteString((u8*)"Welcome ");
				LCD_voidWriteString(NAME[i]);

				/*  if name is in list  ENTER PASSWORD at max 3 times*/
				PASS_COUNT =3 ;
				while(PASS_COUNT!=0)/* start of-> while(PASS_C!=0)*/
				{
					LCD_voidGoToXY(1,3);
					LCD_voidWriteString((u8*)"Enter Password");
					LCD_voidGoToXY(2,3);
					LCD_voidWriteString((u8*)"you have: ");
					LCD_voidWriteNumber(PASS_COUNT);
					LCD_voidWriteString((u8*)" times");

					/*Receive Password */
					UART_VoidReceiveString(UART_DATA);
					H_EXT_EEPROM_void_Read_Str(PASS_INDX[i] , DATA2 ) ;

					if(strcmp((char*)UART_DATA,(char*)DATA2)==0)
					{
						LCD_voidClear();
						LCD_voidGoToXY(0,0);
						LCD_voidWriteString((u8*)"Correct Password");
						LCD_voidGoToXY(1,0);
						LCD_voidWriteString((u8*)"Door is Open [ ] ^_^");

						/*open servo motor (Door) and after 5 sec close*/
						M_TIMER1_voidPwm1Start();
						M_TIMER1_voidPwm1SetDutyCycle(65);
						_delay_ms(5000);
						M_TIMER1_voidPwm1SetDutyCycle(25);

						break ;
					}

					else
					{
						LCD_voidClear();
						LCD_voidGoToXY(0,3);
						LCD_voidWriteString((u8*)"WRONG TRAY AGAIN");
						BUZZ();

						PASS_COUNT -- ;
					}

				}/* End of -> while(PASS_C!=0)*/

				break ;
			}/* End of -> if(strcmp(UART_DATA , DATA2)==0)*/

			Array_Zero_Init(DATA2) ;

		}/* End loop for(i=0;i<2;i++)*/


		if(i==2)
		{
			LCD_voidClear();
			LCD_voidWriteString((u8*)"WRONG Name");
			BUZZ();
			_delay_ms(300);

		}






	}



	return 0 ;
}


void System_Init(void)
{
	LCD_voidInit();
	UART_VoidInit();

	H_EXT_EEPROM_void_Init();

	M_TIMER1_voidPwm1Init();
	M_TIMER1_voidPwm1SetTop();
	M_TIMER1_voidPwm1Start();
	M_TIMER1_voidPwm1SetDutyCycle(25);

	DIO_voidSetPinDirection(PORTD_ID,PIN3,PIN_OUTPUT);
	DIO_voidSetPinValue(PORTD_ID,PIN3,PIN_LOW);


	H_EXT_EEPROM_void_Write_Str(0  , (u8*)"ahmed#" )  ;
	H_EXT_EEPROM_void_Write_Str(10 , (u8*)"1234#" )   ;
	H_EXT_EEPROM_void_Write_Str(20 , (u8*)"mohamed#") ;
	H_EXT_EEPROM_void_Write_Str(30 , (u8*)"1234#" )   ;
}

void Array_Zero_Init(u8* data)
{
	u8 C =0 ;
	for( ; C!=10 ; C++)
	{
		data[C]=0 ;
	}
}

void BUZZ(void)
{
	DIO_voidSetPinValue(PORTD_ID,PIN3,PIN_HIGH);
	_delay_ms(100);
	DIO_voidSetPinValue(PORTD_ID,PIN3,PIN_LOW);
	_delay_ms(100);
	DIO_voidSetPinValue(PORTD_ID,PIN3,PIN_HIGH);
	_delay_ms(100);
	DIO_voidSetPinValue(PORTD_ID,PIN3,PIN_LOW);
	_delay_ms(100);
	DIO_voidSetPinValue(PORTD_ID,PIN3,PIN_HIGH);
	_delay_ms(100);
	DIO_voidSetPinValue(PORTD_ID,PIN3,PIN_LOW);
}
