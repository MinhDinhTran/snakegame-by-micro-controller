
int main(){
	clear = 0;
	LCD_init();
	turnon();
	int i,j,k;
	//int time;
	int time_red_light,time;
	//clear_cs(2);
	//clear_cs(1);


	//for(i=4;i<63;i++) coord3(13,i);

/*
	encs(2);//1 2
	startline();
	page(1);//0~7
	Y(32);//0~63
	readdata(0b01010101);//0~63
*/

	clear_cs(1);
/*
	for(j=11;j>=0;j--){
		digit(j);
		for(i=0;i<1000;i++) nano_wait(1000000);
		clear_cs(1);
	}
*/
	//digit(7);
	clear=0;
	clear_cs(2);
/*
	time=1000000;
	time=10*time/12;
	//while(1){
	for(j=0,k=11;j<3;j++){

		clear_cs(1);digit(k--);clear_cs(2);
		left_leg0();//pose 0
		right_leg0();//pose 1
		draw_head();
		draw_body1();
		draw_shoulder1();
		for(i=0;i<1000;i++) nano_wait(time);

		//---------------------------------------/
		clear_cs(1);digit(k--);clear_cs(2);
		right_leg1();//pose 1
		left_leg1();
		draw_head();
		draw_body1();
		draw_shoulder2();
		for(i=0;i<1000;i++) nano_wait(time);
		//---------------------------------------/
		clear_cs(1);digit(k--);clear_cs(2);
		merge_leg1();
		draw_head();

		draw_body1();
		draw_shoulder3();
		for(i=0;i<1000;i++) nano_wait(time);
		//--------------------------------------/
		clear_cs(1);digit(k--);clear_cs(2);
		merge_leg0();
		draw_head();
		draw_body1();
		draw_shoulder2();

		for(i=0;i<1000;i++) nano_wait(time);
		//---------------------------------------/

	}
*/

	//
	step(1);step(2);step(3);step(4);step(1);step(2);step(3);step(4);step(1);step(2);step(3);step(4);step(5);
	//stop_leg();
	//left_leg0();
	//right_leg0();
	//right_leg1();//pose 1
	//left_leg1();
	merge_leg0();
	//merge_leg1();
	draw_head();
	//move_body();

	draw_body1();
	//draw_body2();
	//draw_shoulder1();
	//draw_shoulder3();
	draw_shoulder2();
	time_red_light=6000000;
	for(i=0;i<1000;i++) nano_wait(time_red_light);
	//clear_cs(2);



	//encs(1);
	//coord1(23,12);
	clear_cs(1);
	//shoe(32,56);
	draw_stop_sign();
//	}



}


