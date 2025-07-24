#include "main.h"

const uint8_t pwm_curve[11]={
	0,		//	0%	- 0
	2,		// ~10% - 1
	5,		// ~20% - 2
	9,		// ~30% - 3
	16,		// ~40% - 4
	27,		// ~50% - 5
	43,		// ~60% - 6
	65,		// ~70% - 7
	95,		// ~80% - 8
	130,	// ~90% - 9
	170		// ~100% - 10
};

void ports_ini(void){
	/*
	PB0 connected to led B				PC0 connected to X				PD0 connected to RXD
	PB1 connected to led C				PC1 connected to X				PD1 connected to X
	PB2 connected to led D				PC2 connected to X				PD2 connected to INT0/BTN
	PB3 connected to MOSI/OC2			PC3 connected to X				PD3 connected to INT1/Enc_A
	PB4 connected to led E				PC4 connected to X				PD4 connected to Enc_B
	PB5 connected to SCK/led A			PC5 connected to X				PD5 connected to led DP
	PB6 connected to led G				PC6 connected to RESET			PD6 connected to X
	PB7 connected to led F												PD7 connected to X
	*/
	
	DDRB=0b11111111;
	PORTB=0;
	
	DDRC=255;
	PORTC=255;
	
	DDRD= 0b11100010;
	PORTD=0b11011110;
			
	TCCR2|=(1<<WGM21); //p. 115, t. 42 - mode FastPWM
	TCCR2|=(1<<WGM20);
		
	TCCR2|=(1<<COM21); // t. 44
	TCCR2&=~(1<<COM20);
			
	TCCR2|=(1<<CS20);
	TCCR2&=~(1<<CS21);
	TCCR2&=~(1<<CS22);
	
	MCUCR|=(1<<ISC01); 
	MCUCR&=~(1<<ISC00);
	GICR|=(1<<INT0);
	
	MCUCR|=(1<<ISC11); 
	MCUCR&=~(1<<ISC10);
	GICR|=(1<<INT1);
}

void display(char indicator){
/*
		 — A —
		|     |
		F     B
		|     |
		 — G —
		|     |
		E     C
		|     |
		 — D —
*/
	switch(indicator){	
		case '0':
			PORTB|=(1<<led_A)|(1<<led_B)|(1<<led_C)|(1<<led_D)|(1<<led_E)|(1<<led_F);
			PORTB&=~(1<<led_G);
		break;	
		
		case '1':
			PORTB|=(1<<led_B)|(1<<led_C);
			PORTB&=~((1<<led_A)|(1<<led_D)|(1<<led_E)|(1<<led_F)|(1<<led_G));
		break;	
		
		case '2':
			PORTB|=(1<<led_A)|(1<<led_B)|(1<<led_D)|(1<<led_E)|(1<<led_G);
			PORTB&=~((1<<led_C)|(1<<led_F)); 
		break;
				
		case '3':
			PORTB|=(1<<led_A)|(1<<led_B)|(1<<led_C)|(1<<led_D)|(1<<led_G); 
			PORTB&=~((1<<led_F)|(1<<led_E));
		break;
		
		case '4':
			PORTB|=(1<<led_B)|(1<<led_C)|(1<<led_F)|(1<<led_G);
			PORTB&=~((1<<led_A)|(1<<led_D)|(1<<led_E));
		break;
		
		case '5':
			PORTB|=(1<<led_A)|(1<<led_C)|(1<<led_D)|(1<<led_F)|(1<<led_G); 
			PORTB&=~((1<<led_B)|(1<<led_E));
		break;
		
		case '6':
			PORTB|=(1<<led_A)|(1<<led_C)|(1<<led_D)|(1<<led_E)|(1<<led_F)|(1<<led_G);
			PORTB&=~(1<<led_B);
		break;
		
		case '7':
			PORTB|=(1<<led_A)|(1<<led_B)|(1<<led_C);
			PORTB&=~((1<<led_D)|(1<<led_E)|(1<<led_F)|(1<<led_G));
		break;
		
		case '8':
		PORTB|=(1<<led_A)|(1<<led_B)|(1<<led_C)|(1<<led_D)|(1<<led_E)|(1<<led_F)|(1<<led_G);
		break;	
		
		case '9':
			PORTB|=(1<<led_A)|(1<<led_B)|(1<<led_C)|(1<<led_D)|(1<<led_F)|(1<<led_G);
			PORTB&=~(1<<led_E);
		break;
		
		case 'F':
			PORTB|=(1<<led_A)|(1<<led_E)|(1<<led_F)|(1<<led_G);
			PORTB&=~((1<<led_B)|(1<<led_C)|(1<<led_D));
		break;	
		
		case 'd':
			PORTB&=~((1<<led_A)|(1<<led_B)|(1<<led_C)|(1<<led_D)|(1<<led_E)|(1<<led_F)|(1<<led_G));
		break;
	}
}

//encoder press
ISR(INT0_vect){
	_delay_ms(5);
	if (is_enable==1){
		is_enable=0;
		display('d');
		PWM_manual(0);
		return;
	} 
	is_enable = true;
	display('F');
	PWM_manual(10);
}

//encoder rotate
ISR(INT1_vect){
	_delay_ms(2);
	if (~PIND&(1<<4)){	
		if(PWR == 10){
			return;
		}
		PWR++;
	}else{
		if(PWR == 0){
			return;
		}
		PWR--;
	}
	PWM_manual(PWR);
}

void PWM_manual(uint8_t PWR){
	switch(PWR){
		case 0:
			OCR2=pwm_curve[0];
			display('0');
		break;
		
		case 1:
			OCR2=pwm_curve[1];
			display('1');
		break;
		
		case 2:
			OCR2=pwm_curve[2];
			display('2');
		break;
		
		case 3:
			OCR2=pwm_curve[3];
			display('3');
		break;
		
		case 4:
			OCR2=pwm_curve[4];
			display('4');
		break;
		
		case 5:
			OCR2=pwm_curve[5];
			display('5');
		break;
		
		case 6:
			OCR2=pwm_curve[6];
			display('6');
		break;
		
		case 7:
			OCR2=pwm_curve[7];
			display('7');
		break;
		
		case 8:
			OCR2=pwm_curve[8];
			display('8');
		break;

		case 9:
			OCR2=pwm_curve[9];
			display('9');
		break;
		
		case 10:
			OCR2=pwm_curve[10];
			display('F');
		break;
	}
}

int main(void){	
	PWR=0;
	is_enable = false;
	 
	sei();
	ports_ini();
	display('d');
	PWM_manual(0);

	while(1)
	{
		
    }
}

