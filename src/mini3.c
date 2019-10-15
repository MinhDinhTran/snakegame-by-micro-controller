#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include "mini3.h"
//#include "draw.c"

int starttempo=0, gametempo=0,gameovertempo=0;
int i,j=-1;
void TIM3_IRQHandler(void){

if(gametempo==75) gametempo=0;
if(starttempo==75) starttempo=0;
if(gameovertempo==39) gameovertempo=0;
if(eat==1){
	j++;
	TIM6->ARR = eatbread_table[j]-1;
	if(j==5){
		eat=0;
		j=-1;
	}
}
else if(die==1){
	j++;
	TIM6->ARR = die_table[j]-1;
	if(j==9){
		die=0;
		j=-1;
	}
}
else{
	if(flag==0){
		TIM6->ARR = start_table[starttempo]-1;
		starttempo++;
		if(starttempo==75){
			flag++;
		}
	}
	else if (flag==2){
		TIM6->ARR = gameover_table[gameovertempo]-1;
		gameovertempo++;
	}
	else if (flag==4){
		TIM3->ARR = 1000000-1;
		j++;
		TIM6->ARR = eatbread_table[j]-1;
		if(j==5){
			j=-1;
		}
	}
	else if(flag==1){
		TIM6->ARR = game_table[gametempo]-1;
		gametempo++;
		if(gametempo==75){
			flag++;
		}
		if(gametempo==34){
			die=1;
		}
		if(gametempo==47){
			die=1;
		}
		if(gametempo==33){
			eat=1;
		}

	}
}
if((TIM3->SR & TIM_SR_UIF) != 0) TIM3->SR &= ~TIM_SR_UIF;//****important
}

void tim3_init(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
 TIM3->PSC = 1000-1;//
 TIM3->ARR = 4000-1;//change to 4 Hz
 NVIC->ISER[0]=1<<TIM3_IRQn;//**ISER
 TIM3->DIER |= TIM_DIER_UIE;//to update interrupt enable
 TIM3 -> CR1 |= TIM_CR1_CEN;//Counter enable

}

void dac_init(void){
    GPIOA->MODER &= ~(3<<8); //clear bit
    GPIOA->MODER |= 3<<8; //enable to Analog mode
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;//0x29
    DAC->CR |= 1<<12;
    DAC->CR |= 1<<2;
    DAC->CR |= 1;//others
}

void tim6_initspace(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 765-1;
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375
 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}

void dma3_init(void) {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;	// enable the periph clock on DMA
	DMA1_Channel3->CCR |= DMA_CCR_PSIZE_0;	// 16 bit Peripheral size set to "01"
	DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0;	// 16 bit Memory size set to "01"
	DMA1_Channel3->CMAR = (uint32_t)(sine180_table);	// source is the array sine_table
	DMA1_Channel3->CNDTR = sizeof sine180_table /2;	// count of elements is the array size
	DMA1_Channel3->CPAR = (uint32_t) (&(DAC->DHR12R1));// configure increment, read from memory, and circular mode
	DMA1_Channel3->CCR |= DMA_CCR_MINC;
	DMA1_Channel3->CCR |= DMA_CCR_DIR;
	DMA1_Channel3->CCR |= DMA_CCR_CIRC;
	DMA1_Channel3->CCR |= DMA_CCR_EN;	// enable DMA1 channel 3
}
/*
int main(void){
	dac_init();
	tim6_initspace();
	dma3_init();
	tim3_init();

	for(;;) asm("wfi");
}
*/
