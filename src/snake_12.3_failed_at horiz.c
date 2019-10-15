#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>
//bool lcd[64][64];

int dir;
int snakebody[20][2];//Max = 20// 20 positions for snake
int snake_len = 8;

int clear=0;
int encs1=0,encs2=0;
int scnt=0;//snake count
int save_pixel[2][8][64];// per DB (8bits) [0/1 = cs 1/2] [page 0~7] [y_line 0~63]
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

void readdata(int d,int cs,int pge,int y_line){
	  //Read data cs
	 GPIOA->ODR &= ~(0b11 << 11);
	 GPIOA->ODR |= (0b10 << 11);
	 GPIOC->ODR &= ~(0xff);


	 GPIOC->ODR |= (save_pixel[cs][pge][y_line] | d);//0: white 1: black
	 //GPIOC->ODR |= (0b00000000 | d);//0: white 1: black
	 GPIOC->ODR |= d;//0: white 1: black
	 transferdata();
}

void data_clear(int d,int cs,int pge,int y_line){
	//Read data cs
	GPIOA->ODR &= ~(0b11 << 11);
	GPIOA->ODR |= (0b10 << 11);
	GPIOC->ODR &= ~(0xff);

	GPIOC->ODR |= save_pixel[cs][pge][y_line];//0: white 1: black
    GPIOC->ODR &=~d;
	//GPIOC->ODR |= (0b00000000 | d);//0: white 1: black
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

void coord1(int x,int y){
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

void coord_clear(int x,int y){
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
	save_pixel[cs][x/8][y_line]&=~(1<<(x%8));
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

	if(scnt==8) snake1();
	if(scnt == 10){
		store_snake();
		display();
		scnt=0;
	}
 	if((TIM2->SR & TIM_SR_UIF) != 0) TIM2->SR &= ~TIM_SR_UIF;//****important
	
	scnt++;//snake count
	
}

void tim2_init(void) {
 	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
 	TIM2->PSC = 60000-1;//
 	TIM2->ARR = 20-1;//change to 40 Hz
 	NVIC->ISER[0]=1<<TIM2_IRQn;//**ISER
 	TIM2->DIER |= TIM_DIER_UIE;//to update interrupt enable
 	TIM2 -> CR1 |= TIM_CR1_CEN;//Counter enable
}

void init_snake(){
	int i;
	for(i=0; i<snake_len; i++){
		snakebody[i][0] = 20;
		snakebody[i][1] = i+20;
	}
}
void store_snake(){
 
 int i,j;
 
 /*
 for(j=0;j<63;j++)
  for(i=0;i<63;i++)
   if(lcd[i][j]==1) coord1(i,j);
 */

 /*for(i=0;i<snake_len;i++)
  coord1(snakebody[i][0], snakebody[i][1]);*/

 	int tempx = snakebody[0][0];//coordx
 	int tempy = snakebody[0][1];//coordy

 switch(dir){//only head directs
	case 3: snakebody[0][1]++ ;break; //up [][1]=y; [][0]=x
  	case 2: snakebody[0][1]-- ;break; //down
  	case 0: snakebody[0][0]-- ;break;//left
 	case 1: snakebody[0][0]++ ;break; //right [][0]=x ++
 }
 
 
 for(j=1;j<snake_len-1;j++){//move whole snake
 	snakebody[j+1][0] = snakebody[j][0];//x next=curr
     snakebody[j+1][1] = snakebody[j][1];//y next=curr 
     snakebody[j][0] = tempx;
     snakebody[j][1] = tempy;
     tempx = snakebody[j+1][0];//current<-next//next head
     tempy = snakebody[j+1][1];
 }
 
  // coord_clear(tempx,tempy);


/*
	int tailx,taily; 
	//clear_snake_tail
	tailx = snakebody[j][0];
	taily = snakebody[j][1];
	snakebody[j+1][0] = snakebody[j][0];//x next=curr
	snakebody[j+1][1] = snakebody[j][1];//y next=curr*/
	

	/*
	snakebody[1][0] = tempx;//NO.1 =org head -x
	snakebody[1][1] = tempy;//NO.1 =org head -y
	for(j=1;j<snake_len-2;j++){//move whole snake
			snakebody[j+1][0] = snakebody[j][0];//x next=curr
			snakebody[j+1][1] = snakebody[j][1];//y next=curr
	}
	
*/
	/*
	snakebody[1][0] = tempx;//NO.1 =org head -x
 	snakebody[1][1] = tempy;//NO.1 =org head -y
 	for(j=2;j<snake_len;j++){//move whole snake
   		tempx = snakebody[j][0];
   		tempy = snakebody[j][1];
   		snakebody[j][0] = tempx;//x next=curr
   		snakebody[j][1] = tempy;//y next=curr
 }
*/
/*
	 for(i=0; i<snake_len; i++){
		 lcd[snakebody[i][0]][snakebody[i][1]] = 1;
	 }

	 lcd[tailx][taily] = 0;
*/

	
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
    save_pixil_init(0);
	LCD_init();
	turnon();
	int i;

	init_b();
	tim2_init();
	clear_cs(2);
	dir=0;
	init_snake();
/*	for(i=0;i<19;i++){
		clear_cs(1);
		coord1(2+i,69);
	}*/
//	coord1(7,39);coord1(24,39);coord1(23,39);coord1(21,30);coord1(20,30);
//	for(i=0;i<12;i++) coord1(9+i,39);
//	coord_clear(10,39);coord_clear(12,39);coord_clear(15,39);
	//coord_clear(14,39);
	//display();
	dir=0;//after display's idr=effective
//    coord_clear(7,39);
	//TIM2_IRQHandler();

}
