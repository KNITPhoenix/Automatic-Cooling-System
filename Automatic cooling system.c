#include <avr/io.h>
#include <util/delay.h>
#include <avr/lcd4.h>
#include <avr/interrupt.h>
unsigned char arr[3];
int i;
void main()
{
	lcdini();
	DDRA=0x00;
	DDRB=0xff;
	DDRC=0xff;
	DDRD=0x00;
	ADCSRA=0x87;
	ADMUX=0xc1;//A0
	lcdcmd(0x01);
	lcdcmd(0x80);
	lcdcmd(0x0C);
	lcdcmd(0x0c);
	unsigned char b[]="Temperature=";
	lcdstring(&b[0]);
	lcdcmd(0x8c);
	GICR|=(1<<INT0);//INTERRUPT 0=PD2
	MCUCR|=(1<<ISC01)|(1<<ISC01);
	sei();
	while(1)
	{
		ADCSRA|=(1<<ADSC);
		while((ADCSRA&(1<<ADIF))==0);
		sprintf(arr,"%d",(ADC/12));
		lcdstring(&arr[0]);
		_delay_ms(1000);
		lcdcmd(0x8c);
		lcdstring("  ");
		i=(ADC/12);
		if(i>=25)
		PORTC|=(1<<PC0);//rising edge to PD2
		PORTC=0x00;
	}
}
ISR(INT0_vect)//opening fan
{
	PORTC|=(1<<PC1);
	PORTC&=~(1<<PC2);
}