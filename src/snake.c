#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <math.h>
#include <stdio.h>
#include "mini3.h"
#include <stdlib.h>
int dir,dir0;
char snakebody[25][2];//Max =24// 24 positions for snake
int snake_len = 5;
char bread_pge[6],bread_y[6],bread_v[6],breadx[6],bready[6];
int clear=0;
int encs1=0,encs2=0;
int scnt=0;//snake count
int win_number=23;

char save_pixel[2][8][64];// per DB (8bits) [0/1 = cs 1/2] [page 0~7]  [y_line 0~63]
int flag=0,eat=0,die=0;
int st=4,scnt2=0;
int d_check=0,w_check=0;//dead/win check
char bread=0;
int starttempo=0, gametempo=0,gameovertempo=0;
int j_=-1;


void snakebody_init(int ini){
	int i;
	for(i=0;i<25;i++){
		snakebody[i][0]=ini;
		snakebody[i][1]=ini;
	}
}
void revive(){

	TIM3->ARR = 4000-1;//change to 4 Hz
	flag=0;
	//tim2_init();
	//init_snake();
	d_check=0;
	w_check=0;
	starttempo=0;
	gametempo=0;
	gameovertempo=0;
	snakebody_init(0);
	save_pixil_init(0b00000000);
	clear_cs(2);

	bread=0;snake_len=5;scnt=0;scnt2=0;
/*
		do{
			dir+=(1+rand()%3);dir=dir%4;
			//dir+=1;
			//dir=dir%4;
		}while(dir==dir0);*/
	dir=1;
	st=4;
	j_=-1;
}
void TIM3_IRQHandler(void){

	if(d_check==1 || w_check==1){
		//check_b();
			//left
			if(GPIOA->IDR & (1<<0)){
				revive();
			}
			else if(GPIOA->IDR & (1<<1)){
				revive();
			}
			else if(GPIOA->IDR & (1<<2)){
				revive();
			}
			else if(GPIOA->IDR & (1<<3)){
				revive();
			}

		}

	/*
    if((GPIOA->IDR & (0b1111)) && die==1) {
        tim2_init();
        init_snake();
    }
    */
	if(((scnt2)%9==0) && (st>=0)){
  		digit(st);st--;
 	}else if(st==-1){
  		tim2_init();
  		init_snake();
  		st--;
	}

 scnt2++;
if(gametempo==75) gametempo=0;
if(starttempo==75) starttempo=0;
if(gameovertempo==39) gameovertempo=0;
if(eat==1){
 j_++;
 TIM6->ARR = eatbread_table[j_]-1;
 if(j_==5){
  eat=0;
  j_=-1;
 }
}
else if(die==1){
 j_++;
 TIM6->ARR = die_table[j_]-1;
 if(j_==5){
  flag=2;
  die=0;
 }
}
else{
 if(flag==0){

  TIM6->ARR = gameover_table[gameovertempo]-1;
    gameovertempo++;
 }
 else if (flag==2){//lose
   //TIM3->ARR = 10000-1;
   j_++;
   TIM6->ARR = eatbread_table[j_]-1;
   if(j_==6){
    j_=-1;
   }
 }
 else if (flag==3){//win
  TIM6->ARR = start_table[starttempo]-1;
  starttempo++;
 }
 else if(flag==1){
  TIM6->ARR = game_table[gametempo]-1;
  gametempo++;
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

void nano_wait(int t) {
    asm("       mov r0,%0\n"
        "repeat:\n"
        "       sub r0,#83\n"
        "       bgt repeat\n"
        : : "r"(t) : "r0", "cc");
}

void transferdata(void){
	nano_wait(8000);
	GPIOA->ODR |= (0x1 << 7);
	nano_wait(8000);
	GPIOA->ODR &= ~(0x1 << 7);
}

void turnon(void){

	//Turn on
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b00 << 11);
	 GPIOC->ODR &= ~(0xff);
	 GPIOC->ODR |= (0b00111111);
	 transferdata();
}
void encs(int cs){
	 GPIOA->ODR &= ~(0b1 << (10-cs)); //Enable cs1
	 GPIOA->ODR |= 0b1 << (7+cs); //disable the other
	 transferdata();
	 if(cs==1) {
		 encs1=1;
		 encs2=0;
	 }
	 else {
		 encs2=1;
		 encs1=0;
	 }
	 //transferdata();

}

void LCD_init(void){

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= (0x555 << 14);
	GPIOA->ODR |= (0b001111 << 7);

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= 0x5555;
}
void save_pixil_init(int ini){
    int i,j,k;
    for(i=0;i<2;i++){
        for(j=0;j<8;j++){
            for(k=0;k<64;k++) save_pixel[i][j][k]=ini;
        }
    }
}

void readdata(char d,int cs,int pge,int y_line){
	  //Read data cs
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b10 << 11);
	 GPIOC->ODR &= ~(0xff);

	//save_pixel[cs][pge][y_line] |= d;
	//GPIOC->ODR |=  save_pixel[cs][pge][y_line];


	GPIOC->ODR |= (save_pixel[cs][pge][y_line] | d);//0: white 1: black
	 //GPIOC->ODR |= (0b00000000 | d);//0: white 1: black
	// GPIOC->ODR |= d;//0: white 1: black
	 transferdata();
}

void data_clear(char d,int cs,int pge,int y_line){
	//Read data cs
	GPIOA->ODR &= ~(0b11 << 11);
	GPIOA->ODR |= (0b10 << 11);


   	GPIOC->ODR &= ~(0xff);
    save_pixel[cs][pge][y_line]&=~d;
	GPIOC->ODR |= save_pixel[cs][pge][y_line];
	//----clear prvious page
	/*
	GPIOC->ODR &= ~(0xff);
    save_pixel[cs][pge][y_line]&=~d;
	GPIOC->ODR |= save_pixel[cs][pge-1][y_line];//only for right:"1"
	*/

	transferdata();
}

void startline(void){
	 //start line
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b00 << 11);
	 GPIOC->ODR &= ~(0xff);
	 GPIOC->ODR |= (0b11000000);
	 transferdata();
}

void page(int p){
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b00 << 11);
	 GPIOC->ODR &= ~(0xff);
	 GPIOC->ODR |= (0b10111000 | p);
	 transferdata();
}

void Y(int y){

	 //Y address
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b00 << 11);
	 GPIOC->ODR &= ~(0xff);
	 GPIOC->ODR |= ((0b01000000) + y);
	 transferdata();
}

void coord1(int x,int y){//print the coordinate
	int cs,y_line;
	startline();
	//y
	cs=y/64;
	if(cs==0 && encs1==0) {
        encs(1);//select cs1 or cs2
    }
	else if(cs==1 && encs2==0){
        encs(2);
    }

	Y(y%64);

	//x
	page(x/8);//selcet page

	y_line=y%64;
	save_pixel[cs][x/8][y_line]|=(1<<(x%8));
	readdata(1<<(x%8),cs,x/8,y_line);//dot,cs,page,data_line
}

void coord_clear(int x,int y){//clear chosen dot
	int cs,y_line;
	startline();
	//y
	cs=y/64;
	if(cs==0 && encs1==0) {
        encs(1);//select cs1 or cs2

    }
	else if(cs==1 && encs2==0){
        encs(2);
    }
	Y(y%64);

	//x
	page(x/8);//selcet page


  	y_line=y%64;
//	save_pixel[cs][x/8][y_line]&=~(1<<(x%8));
	data_clear(1<<(x%8),cs,x/8,y_line);//dot,cs,page,data_line
}

void clear_cs(int cs){
	int i=0,j=0;

	encs(cs);

	startline();
	for(i=0;i<8;i++){
		page(i);
		for(j=0;j<64;j++){
			Y(j);
			 GPIOA->ODR &= ~(0b11 << 11);
			 GPIOA->ODR |= (0b10 << 11);
			 GPIOC->ODR &= ~(0xff);
			 GPIOC->ODR |= (0b00000000);//0: white 1: black
			 //save_pixel=0b00000000;
			 transferdata();
		}
	}

	clear=1;
}

void init_b(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~(0b11111111 << 0);//input
	GPIOA->IDR &= ~(0b1111 << 0);
	GPIOA->PUPDR &=~ (0b11111111 << 0); // iF NOT PRESS= 0
	GPIOA->PUPDR |= (0b10101010 << 0); // iF PRESSed=1 -> pull down
}

void check_b(){
	if(GPIOA->IDR & (1<<0) ) dir = 0;
	if(GPIOA->IDR & (1<<1) ) dir = 1;
	if(GPIOA->IDR & (1<<2) ) dir = 2;
	if(GPIOA->IDR & (1<<3) ) dir = 3;

}

void snake1(){
	clear_cs(1);

}

void TIM2_IRQHandler(void){

		check_b();

		check_dead();
		check_win();
		dir0=dir;
		/*if((scnt%20==0) && (flag==3)) digit(snake_len);
		else if((scnt%20!=0) && (flag==3)) clear_cs(1);*/
		if(scnt% (9-(snake_len)/3)==0){

			if(bread==0) generate_bread();
			eat_bread();
			store_snake();
			display();
			drawwalls();
		}
	 	if((TIM2->SR & TIM_SR_UIF) != 0) TIM2->SR &= ~TIM_SR_UIF;//****important

		scnt++;//snake count



}

void tim2_init(void) {
 	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
 	TIM2->PSC = 70000-1;//
 	TIM2->ARR = 20-1;//change to 40 Hz
 	NVIC->ISER[0]=1<<TIM2_IRQn;//**ISER
 	TIM2->DIER |= TIM_DIER_UIE;//to update interrupt enable
 	TIM2 -> CR1 |= TIM_CR1_CEN;//Counter enable.
 	flag=1;//start game
}

void init_snake(){//AT MIDDLE

	int i;
	for(i=0; i<snake_len; i++){
		snakebody[i][0] = 32;
		snakebody[i][1] = i+25+64;
	}
}

void generate_bread(){


	int i;


	digit(snake_len);
	encs(2);
	for(i=0;i<1;i++) {

		/*bread_pge[i]=rand()%8;
		bread_y[i]=rand()%60;//64
		bread_v[i]=rand()%248;//255*/
		srand(abs(scnt2)*snake_len);
		breadx[i]=rand()%58+2;
		bready[i]=rand()%58+2+64;

		bread_pge[i]=breadx[i]/8;
		bread_y[i]= bready[i]/64;
		bread_v[i]=breadx[i]%8;
		save_pixel[1][bread_pge[i]][bread_y[i]]=bread_v[i];
		/*
		save_pixel[1][bread_pge[i]][bread_y[i]]=bread_v[i];
		breadx[i]=bread_pge[i]*8+bread_v[i];//x
		bready[i]=bread_y[i]+64;//y*/

		coord1(breadx[i],bready[i]);
		bread=1;
	}


}
void eat_bread(){//fake_bread bug might be here
	int i;
	for (i=0;i<1;i++) if(snakebody[0][0]==breadx[i] && snakebody[0][1]==bready[i]) {
		snake_len+=3;
		generate_bread();
		eat=1;//eat flag
		bread=0;
		data_clear(1<<(breadx[i]%8),1,breadx[i]/8,bready[i]%64);
	}

}

int check_dead(){
 int i;

 if((snakebody[0][0] == 62) || (snakebody[0][0] == 2) || (snakebody[0][1] == 65) || (snakebody[0][1] == 126)){//touch wall->dead
 	d_check = 1;
 }
 for(i=1; i<snake_len; i++){//touch self->dead
 	if((snakebody[0][0] == snakebody[i][0]) && (snakebody[0][1] == snakebody[i][1])) d_check = 1;
 }
 //die=0;
 if(d_check ==1){

  	clear_cs(1);
  	//flag = 2;
  	die=1;

  	draw_stop_sign();
  	TIM2 -> ARR = 1-1;//Counter stop



 }

 else{
 	die=0;

 }
}

void readdata_norm(char d){
	  //Read data cs
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b10 << 11);
	 GPIOC->ODR &= ~(0xff);

	//save_pixel[cs][pge][y_line] |= d;
	//GPIOC->ODR |=  save_pixel[cs][pge][y_line];


	GPIOC->ODR |= (0b00000000 | d);//0: white 1: black
	 //GPIOC->ODR |= (0b00000000 | d);//0: white 1: black
	// GPIOC->ODR |= d;//0: white 1: black
	 transferdata();
}


void Short(int p, int y, int r){

 startline();
 page(p);//0~7
 Y(y);//0~63
 readdata_norm(r);//0~63
}


void draw9(){
    int i;
    for(i=16; i<20; i++) Short(1, i, (0b11111111));
    for(i=20; i<32; i++) Short(1, i, (0b00001111));
    for(i=32; i<36; i++) Short(1, i, (0b11111111));
    for(i=36; i<49; i++) Short(1, i, (0b00001111));
    for(i=49; i<52; i++) Short(1, i, (0b11111111));

    for(i=16; i<20; i++) Short(2, i, (0b11111111));
    for(i=20; i<32; i++) Short(2, i, (0b11110000));
    for(i=32; i<36; i++) Short(2, i, (0b11111111));
    for(i=36; i<49; i++) Short(2, i, (0b00000000));
    for(i=49; i<52; i++) Short(2, i, (0b11111111));
}

void draw8(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b11110000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b11110000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw7(){
	int i;
	 for(i=16; i<20; i++) Short(1, i, (0b11111111));
	 for(i=20; i<32; i++) Short(1, i, (0b00001111));
	 for(i=32; i<36; i++) Short(1, i, (0b00001111));
	 for(i=36; i<48; i++) Short(1, i, (0b00001111));
	 for(i=48; i<52; i++) Short(1, i, (0b00001111));

	 for(i=16; i<20; i++) Short(2, i, (0b11111111));
	 for(i=20; i<32; i++) Short(2, i, (0b00000000));
	 for(i=32; i<36; i++) Short(2, i, (0b00000000));
	 for(i=36; i<48; i++) Short(2, i, (0b00000000));
	 for(i=48; i<52; i++) Short(2, i, (0b00000000));
}

void draw6(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00000000));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b11110000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b11110000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw5(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00000000));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b11110000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b00000000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw4(){
	int i;
	 for(i=16; i<20; i++) Short(1, i, (0b00001111));
	 for(i=20; i<32; i++) Short(1, i, (0b00001111));
	 for(i=32; i<36; i++) Short(1, i, (0b11111111));
	 for(i=36; i<48; i++) Short(1, i, (0b00001111));
	 for(i=48; i<52; i++) Short(1, i, (0b00001111));

	 for(i=16; i<20; i++) Short(2, i, (0b11110000));
	 for(i=20; i<32; i++) Short(2, i, (0b11110000));
	 for(i=32; i<36; i++) Short(2, i, (0b11111111));
	 for(i=36; i<48; i++) Short(2, i, (0b00000000));
	 for(i=48; i<52; i++) Short(2, i, (0b00000000));
}

void draw3(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b00000000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b00000000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw2(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b11111111));
 for(i=36; i<48; i++) Short(1, i, (0b00000000));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b00000000));
 for(i=32; i<36; i++) Short(2, i, (0b11111111));
 for(i=36; i<48; i++) Short(2, i, (0b11110000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}

void draw1(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b00001111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b00001111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b00001111));

 for(i=16; i<20; i++) Short(2, i, (0b00000000));
 for(i=20; i<32; i++) Short(2, i, (0b00000000));
 for(i=32; i<36; i++) Short(2, i, (0b00000000));
 for(i=36; i<48; i++) Short(2, i, (0b00000000));
 for(i=48; i<52; i++) Short(2, i, (0b00000000));
}

void draw0(){
 int i;
 for(i=16; i<20; i++) Short(1, i, (0b11111111));
 for(i=20; i<32; i++) Short(1, i, (0b00001111));
 for(i=32; i<36; i++) Short(1, i, (0b00001111));
 for(i=36; i<48; i++) Short(1, i, (0b00001111));
 for(i=48; i<52; i++) Short(1, i, (0b11111111));

 for(i=16; i<20; i++) Short(2, i, (0b11111111));
 for(i=20; i<32; i++) Short(2, i, (0b11110000));
 for(i=32; i<36; i++) Short(2, i, (0b11110000));
 for(i=36; i<48; i++) Short(2, i, (0b11110000));
 for(i=48; i<52; i++) Short(2, i, (0b11111111));
}



void draw9X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<49; i++) Short(5, i, (0b00001111));
    for(i=49; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<49; i++) Short(6, i, (0b00000000));
    for(i=49; i<52; i++) Short(6, i, (0b11111111));
}

void draw8X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b11110000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw7X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b00001111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b00001111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b00000000));
    for(i=32; i<36; i++) Short(6, i, (0b00000000));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b00000000));
}

void draw6X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00000000));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b11110000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw5X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00000000));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw4X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b00001111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b00001111));

    for(i=16; i<20; i++) Short(6, i, (0b11110000));
    for(i=20; i<32; i++) Short(6, i, (0b11110000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b00000000));
}

void draw3X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b00000000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw2X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b11111111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b11111111));
    for(i=36; i<48; i++) Short(5, i, (0b00000000));
    for(i=48; i<52; i++) Short(5, i, (0b11111111));

    for(i=16; i<20; i++) Short(6, i, (0b11111111));
    for(i=20; i<32; i++) Short(6, i, (0b00000000));
    for(i=32; i<36; i++) Short(6, i, (0b11111111));
    for(i=36; i<48; i++) Short(6, i, (0b11110000));
    for(i=48; i<52; i++) Short(6, i, (0b11111111));
}

void draw1X(){
    int i;
    for(i=16; i<20; i++) Short(5, i, (0b00001111));
    for(i=20; i<32; i++) Short(5, i, (0b00001111));
    for(i=32; i<36; i++) Short(5, i, (0b00001111));
    for(i=36; i<48; i++) Short(5, i, (0b00001111));
    for(i=48; i<52; i++) Short(5, i, (0b00001111));

    for(i=16; i<20; i++) Short(6, i, (0b00000000));
    for(i=20; i<32; i++) Short(6, i, (0b00000000));
    for(i=32; i<36; i++) Short(6, i, (0b00000000));
    for(i=36; i<48; i++) Short(6, i, (0b00000000));
    for(i=48; i<52; i++) Short(6, i, (0b00000000));
}


void digit(int x){
	clear_cs(1);
	//ones
	switch(x%10){
		case 0:draw0();break;
		case 1:draw1();break;
		case 2:draw2();break;
		case 3:draw3();break;
		case 4:draw4();break;
		case 5:draw5();break;
		case 6:draw6();break;
		case 7:draw7();break;
		case 8:draw8();break;
		case 9:draw9();break;
	}

	//tens
	switch(x/10){
		//case 0:draw0X();break;
		case 1:draw1X();break;
		case 2:draw2X();break;
		case 3:draw3X();break;
		case 4:draw4X();break;
		case 5:draw5X();break;
		case 6:draw6X();break;
		case 7:draw7X();break;
		case 8:draw8X();break;
		case 9:draw9X();break;
	}
}
void draw_win(){
	int i,j;
	for(i=0;i<64;i++){
		for(j=0;j<6;j++) coord1(64-i/2+j-3,i+64);
	}
	for(i=0;i<64;i++){
 		for(j=0;j<6;j++) coord1(32-i/2+j-3,64-i+64);
 }

}

void draw_stop_sign(){
 int i,j;
 for(i=0;i<63;i++){
  for(j=0;j<6;j++) coord1(i+j,i+64);
 }
 for(i=0;i<63;i++){
 	for(j=0;j<6;j++) coord1(63-i-j,i+64);
 }

}
void check_win(){

 if(snake_len == win_number){
	 w_check=1;
	 clear_cs(2);
	 flag = 3;
	 draw_win();
	 drawwalls();
	 TIM2 -> ARR = 1-1;//Counter enable
	 //nano_wait(80000000);

 }

}


void drawwalls(){
 int i, j;
 for(i=0; i<2; i++)
  for(j=64; j<128; j++)
   coord1(i, j);

 for(i=2; i<62; i++)
  for(j=64; j<66; j++)
   coord1(i, j);

 for(i=2; i<62; i++)
  for(j=126; j<128; j++)
   coord1(i, j);

 for(i=62; i<64; i++)
  for(j=64; j<128; j++)
   coord1(i, j);
}
void store_snake(){

 int i,j;

 	int tempx = snakebody[0][0];//coordx
 	int tempy = snakebody[0][1];//coordy

 	coord_clear(snakebody[snake_len-1][0],snakebody[snake_len-1][1]);



 	for(j=snake_len-1;j>0;j--){//move whole snake
 		snakebody[j][0] = snakebody[j-1][0];//x next=curr
 		snakebody[j][1] = snakebody[j-1][1];//x next=curr
 	}


 switch(dir){//only head directsq
	case 3: snakebody[0][1]++ ;break; //up [][1]=y; [][0]=x
  	case 2: snakebody[0][1]-- ;break; //down
  	case 0: snakebody[0][0]-- ;break;//left
 	case 1: snakebody[0][0]++ ;break; //right [][0]=x ++
 }
 	coord_clear(snakebody[snake_len-1][0],snakebody[snake_len-1][1]);

}




void display(){

 int i,j;

 for(i=0; i<snake_len; i++)//print
  	coord1(snakebody[i][0], snakebody[i][1]);
 /*
    for(i=0; i<64; i++)//print
        for(j=0; j<64; j++)//print
            if(lcd[i][j] == 1) coord1(i, j);
*/
}



int main(){

	dac_init();
	tim6_initspace();
	dma3_init();
	tim3_init();
	clear_cs(1);
	//digit(snake_len);

    save_pixil_init(0);
    clear_cs(2);

	LCD_init();
	turnon();


	init_b();

	//tim2_init();
	clear_cs(1);
	//init_snake();

	//dir=rand()%4;
	dir=0;

	clear_cs(2);

/*	for(i=0;i<19;i++){
		clear_cs(1);
		coord1(2+i,69);
	}*/
//	coord1(7,39);coord1(24,39);coord1(23,39);coord1(21,30);coord1(20,30);
//	for(i=0;i<12;i++) coord1(9+i,39);
//	coord_clear(10,39);coord_clear(12,39);coord_clear(15,39);
	//coord_clear(14,39);
	//display();
	//after display's idr=effective
//    coord_clear(7,39);
	//TIM2_IRQHandler();
	return 0;
}
