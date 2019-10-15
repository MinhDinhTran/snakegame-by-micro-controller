#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>

void nano_wait(int);
void init_lcd(void);
void display1(const char *s);
void display2(const char *s);


int main(void) {
//  6.1
/*
    init_lcd();
    display1("Hello, World3.");
    for(;;);
*/
//  6.2
/*
    float f = 0.0;
    init_lcd();
    for(;;){
        char line[17];
        snprintf(line, sizeof line, "Number: %.3f", f);
        display1(line);
        f += 1.234;
        nano_wait(250*1000*1000);
    }
 */
// 6.3
	//first line

	  float f = 0.0;

	 /*   for(;;){
	        char line1[17];
	        snprintf(line1, sizeof line1, "Number: %.3f", f);
	        display1(line1);
	        f += 1.234;
	        nano_wait(250*1000*1000);*/

	init_lcd();
	char line1[17];
	char line2[21];
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= 0x30000;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CR2 |= RCC_CR2_HSI14ON;
	while(!(RCC->CR2 &ADC_ISR_ADRDY));
	ADC1->CR |= ADC_CR_ADEN;
	while(!(ADC1->ISR & ADC_ISR_ADRDY));
	while((ADC1->CR & ADC_CR_ADSTART));
	while(1){
		 //snprintf(line1, sizeof line1, "Number: %.3f", f);
         //display1(line1);
	     //f += 1.234;
	     //nano_wait(250*1000*1000);
		ADC1->CHSELR = 0;
		ADC1->CHSELR |= 1<< 8;
		while (!(ADC1->ISR & ADC_ISR_ADRDY));
		ADC1->CR |= ADC_CR_ADSTART;
		while(!(ADC1->ISR & ADC_ISR_EOC));
		sprintf(line1, "Integer: 0x%x", ADC1->DR);
		display1(line1);
		sprintf(line2, "Voltage: %2.2f", ADC1->DR * 3 / 4095.0);
		display2(line2);
	 }

}
