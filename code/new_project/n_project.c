/*
 * project.c
 *
 *  Created on: Apr 9, 2019
 *      Author: Metro
 */

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char num_sec=0;
unsigned char num_minuts=0;
unsigned char num_hours=0;
volatile unsigned char chick=0;

void timer1_oco(void)
{
	/* FOC1A to timer1_cTc and not PWM*/
	TCCR1A =(1<<FOC1A);

	/* WGM12 to work CTC immediate */
	/* CS12 and CS10 to N = 1024 */
	TCCR1B =(1<<WGM12)|(1<<CS12)|(1<<CS10);

	TCNT1 = 0;
	/* compare when 1 SEC */
	OCR1A =1000;

	/* enable CTC*/
	TIMSK |=(1<<OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	/* if press button stop timer and press again rework timer ( INTERRUPT 0)*/
	if(chick %2==0)
	{
		num_sec++;

	}


}


// External INT0 Interrupt Service Routine
ISR(INT0_vect)
{
	chick++;
}

// External INT0 enable and configuration function
void INT0_Init(void)
{
	// Enable external interrupt pin INT0
	GICR  |= (1<<INT0);
	// Trigger INT0 with the falling edge
	MCUCR &= ~(1<<ISC00);
	MCUCR |= (1<<ISC01);
}


int main()
{
	DDRC |=0x0F;
	PORTC &=0xF0;
	SREG |=(1<<7);

	DDRA |=0x3F;
	PORTA &=(0xc0);

	// configure INT0/PD2 as input pin
	DDRD  &= (~(1<<PD2));
	// enable internal pull up resistor at INT0/PD2 pin
	PORTD |= (1<<PD2);


	timer1_oco();

	INT0_Init();



	unsigned char num_sec1=0;
	unsigned char num_sec2=0;

	unsigned char num_minut1=0;
	unsigned char num_minut2=0;

	unsigned char num_hour1=0;
	unsigned char num_hour2=0;

	//unsigned char count=0;
	//unsigned char arr[6]={0};  //{num_hour2 , num_hour1 ,num_minut2 ,num_minut1,num_sec2,num_sec1};

	for(;;)
	{
		/*
		_delay_ms(2);
		PORTD =(PORTD & 0xC0) | (1<<count);
		PORTC =(PORTC & 0xF0)|(arr[5] & 0x0F);
		count++;
		if(count==6)
		{
			count=0;
		}
		 */




		/* display 7_segments */
		PORTA =(PORTA & 0xC0) | 0x01;
		PORTC =(PORTC & 0xF0)|(num_sec1 & 0x0F);
		_delay_ms(2);

		PORTA =(PORTA & 0xC0) | 0x02;
		PORTC =(PORTC & 0xF0)|(num_sec2 & 0x0F);
		_delay_ms(2);

		PORTA =(PORTA & 0xC0) | 0x04;
		PORTC =(PORTC & 0xF0)|(num_minut1 & 0x0F);
		_delay_ms(2);

		PORTA =(PORTA & 0xC0) | 0x08;
		PORTC =(PORTC & 0xF0)|(num_minut2 & 0x0F);
		_delay_ms(2);


		PORTA =(PORTA & 0xC0) | 0x10;
		PORTC =(PORTC & 0xF0)|(num_hour1 & 0x0F);
		_delay_ms(2);

		PORTA =(PORTA & 0xC0) | 0x20;
		PORTC =(PORTC & 0xF0)|(num_hour1 & 0x0F);
		_delay_ms(2);




		if(num_sec < 59)
		{
			num_sec1=(num_sec % 10);
			num_sec2=(num_sec /10);



		}
		else if(num_minuts < 59)
		{
			/*when num_sec=59 */
			num_sec=0;
			num_sec1=0;
			num_sec2=0;

			num_minuts++;
			num_minut1=(num_minuts % 10);
			num_minut2=(num_minuts /10);

		}
		else if(num_hours < 23)
		{
			/*when num_sec=59 */
			num_sec=0;
			num_sec1=0;
			num_sec2=0;

			/*when num_minuts=59 */
			num_minuts=0;
			num_minut1=0;
			num_minut2=0;

			num_hours++;
			num_hour1=(num_hours % 10);
			num_hour2=(num_hours /10);

		}
		else  /*when the hour is 24 (reset)*/
		{
			num_sec=0;
			num_sec1=0;
			num_sec2=0;

			num_minuts=0;
			num_minut1=0;
			num_minut2=0;

			num_hours=0;
			num_hour1=0;
			num_hour2=0;


		}


	}
}
