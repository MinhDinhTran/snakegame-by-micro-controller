#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "sinewave.h"
//#include "draw.c"
int tot=5000000;
int cnt=0;
void mIcro_wait(int t){

    //static int tot=0;
    tot+=t;

   if((tot / 5000000)>=1){
	  step(cnt%4+1,(11-cnt));
	  if(cnt==12) step(5);
	   tot-=5000000;
	   cnt++;
   }
    micro_wait(t);
    //cnt++;//cnt=cnt%4;

}


void dac_init(void){
 //RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
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
 TIM6->ARR = 4400-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}

void tim6_initlowerfa(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 765-1;
 // lower se 533 la 553
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initlowerso(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 685-1;
 // lower se 533 la 553
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initlowerla(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 610-1;
 // lower se 533 la 553
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initlowerse(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 540-1;
 // lower se 540 la
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initdo(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 510-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initre(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 455-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 455 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initme(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 405-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initfa(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 385-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initso(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 340-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initla(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 305-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}

void tim6_initse(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 270-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}
void tim6_initsharpdo(void) {
 RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 TIM6->PSC = 1-1;// 1
 TIM6->ARR = 255-1;//
 // 1 400 4 me, 2 330 la 4 key,3 re mid 330 , 4 la down 330
 // 1: 505 do 470 minner re 450 re 425 minner me 400 me 380 fa 360 sharp fa 340 so 320 sharp so 300 la 285 sharp la 270 si 250 230 do2: 320 minner la 330 sharp so 340 so 360 minner so 375

 TIM6->CR1|=TIM_CR1_ARPE;//auto-reload
 TIM6->CR2 |=TIM_CR2_MMS_1;// the Counter enable signa_TRGO
 TIM6->CR1 |=TIM_CR1_CEN;//Counter enabled
}

void dma3_init(void) {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;	// enable the periph clock on DMA
	DMA1_Channel3->CCR |= DMA_CCR_PSIZE_0;	// 16 bit Peripheral size set to "01"
	DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0;	// 16 bit Memory size set to "01"
	DMA1_Channel3->CMAR = (uint32_t)(sine_table);	// source is the array sine_table
	DMA1_Channel3->CNDTR = sizeof sine_table /2;	// count of elements is the array size
	DMA1_Channel3->CPAR = (uint32_t) (&(DAC->DHR12R1));// configure increment, read from memory, and circular mode
	DMA1_Channel3->CCR |= DMA_CCR_MINC;
	DMA1_Channel3->CCR |= DMA_CCR_DIR;
	DMA1_Channel3->CCR |= DMA_CCR_CIRC;
	DMA1_Channel3->CCR |= DMA_CCR_EN;	// enable DMA1 channel 3
}

// These variables are used below.
int t6_dir = 1;
int t6_offset = 0;
int t6_max = 20;
int t6_min = -20;

void TIM3_IRQHandler(void) {
	TIM3->SR &= ~TIM_SR_UIF;// clear the bits
	t6_offset = t6_dir + t6_offset;	// add t6_dir to t6_offset
	if (t6_offset <= t6_min){
		t6_dir = 1;
		t6_offset = t6_min + 1;
	}
	if (t6_offset >= t6_max){
		t6_dir = -1;
		t6_offset = t6_max - 1;
	}
	TIM6->ARR = t6_dir + TIM6->ARR;
}


void tim3_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;// enable timer3
	TIM3->PSC = 960- 1;
	TIM3->ARR = 760-1;
	TIM3->CR1 |= TIM_CR1_CEN;
	TIM3->DIER |= TIM_DIER_UIE;	// configure the DIER register
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn,0);
}

void open_LED(void){

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= 0xffffffc3;
	GPIOA->MODER |= 0x00000014;//output: 01
	//GPIOA->ODR &= 0xfff9;
	//GPIOA->ODR |= 0x0002;//green light on
	GPIOA->ODR &= ~(0b11 << 1 );
	GPIOA->ODR |=(0b01 << 1 );//green light on
}

int main(void){
    /*test
	int i;
    dac_init();
    tim6_initsharpdo();
    dma3_init();
    mIcro_wait(2000000);*/



int i,j=1;

/* heart and soul*/

//tim3_init();


while(1){
	cnt=0;tot=5000000;
	LCD_init();
	turnon();
	//dgt_cnt=11;
open_LED();
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(500000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(1000000);
dac_init();
tim6_initme();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initme();
dma3_init();
mIcro_wait(1000000);
dac_init();
tim6_initlowerla();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initlowerla();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initre();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initre();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initfa();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initfa();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerso();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initlowerso();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerse();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initlowerse();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initme();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initme();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerla();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initlowerla();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerfa();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initlowerfa();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerla();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerla();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerso();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initlowerso();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerse();
dma3_init();
mIcro_wait(1000000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(500000);
dac_init();
tim6_initlowerse();
dma3_init();
mIcro_wait(500000);
  dac_init();
  tim6_initspace();
  dma3_init();
  mIcro_wait(250000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(1000000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(1000000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(1000000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(1000000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(1000000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(5500000);//step(1);mIcro_wait(1000000);step(2);mIcro_wait(1000000);step(3);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(400000);
dac_init();
tim6_initlowerse();
dma3_init();
mIcro_wait(600000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(350000);
dac_init();
tim6_initlowerla();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(450000);
dac_init();
tim6_initlowerse();
dma3_init();
mIcro_wait(900000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initdo();
dma3_init();
mIcro_wait(700000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(300000);
dac_init();
tim6_initre();
dma3_init();
mIcro_wait(1350000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(500000);
dac_init();
tim6_initlowerse();
dma3_init();
mIcro_wait(750000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(600000);
dac_init();
tim6_initme();
dma3_init();
mIcro_wait(1700000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(850000);
dac_init();
tim6_initme();
dma3_init();
mIcro_wait(1700000);
dac_init();
tim6_initspace();
dma3_init();
mIcro_wait(850000);
dac_init();
tim6_initme();
dma3_init();
mIcro_wait(1700000);
dac_init();
tim6_initspace();
dma3_init();
micro_wait(2500000);


//GPIOA->ODR &= 0xfffd;//green light off
GPIOA->ODR &=~ (0b11<<1);
GPIOA->ODR |= (0b10<<1);//red light on
step(5);
	for(i=0;i<5;i++){
		//for(i=0;i<1;i++){
		dac_init();
		tim6_initla();

		dma3_init();
		micro_wait(5000000);
		dac_init();
		tim6_initspace();
		dma3_init();
		micro_wait(1000000);
	}
//GPIOA->ODR &= 0xfffb;
GPIOA->ODR |= (0B00<<1);
//micro_wait(800000);
clear_cs(2);
clear_cs(1);
}

//tot=5000000;
//micro_wait(1000000);
    //tim3_init();
    //for(;;) asm("wfi");
}
