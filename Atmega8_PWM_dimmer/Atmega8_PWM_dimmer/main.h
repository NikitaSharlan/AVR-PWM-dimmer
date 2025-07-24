#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define led_A PB5
#define led_B PB0
#define led_C PB1
#define led_D PB2
#define led_E PB4
#define led_F PB7
#define led_G PB6
#define led_DP PD5

const uint8_t pwm_curve[11];
uint8_t PWR;
char indicator;
bool is_enable;

void ports_ini(void);
void display(char indicator);
void PWM_manual(uint8_t PWR);
ISR(INT0_vect);
ISR(INT1_vect);