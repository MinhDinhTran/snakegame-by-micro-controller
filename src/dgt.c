void digit0(){
	int i;
	int j;
	//zero-right
	for(i=4;i<63-4;i++)	coord4(1,i);
	//zero-left
	for(i=4;i<63-4;i++) coord4(9,i);
	//zero-top//need fix:x
	for(j=0;j<4;j++)
		for(i=0;i<32;i+=8)shoe(i,j);
	//zero-bottom//need fix:x
	for(j=0;j<4;j++)
		for(i=0;i<32;i+=8) shoe(i,j+63-4);
}

void digit1(){
	int i;
	int j;
	//one-right(straight)
	for(i=0;i<63-4;i++) coord4(1,i);
	//one-slope//need add more pixils
	for(i=0;i<12;i++) coord4(2+i,i);
	//one-bottom(horizontal)//need fix:x
	for(j=0;j<4;j++)
		for(i=0;i<32;i+=8) shoe(i,j+63-4);
}

void digit2(){
	int i,j;
	//-------------------  two-top  --------------------------------//
	for(i=4;i<32/2;i+=4) 
		for(j=0;j<4;j++) coord4(i,j);
		
	//----------------  two-right(straight)  -----------------------//
	for(i=0;i<31;i++) coord4(1,i);
	
	//------------------  two-middle  ------------------------------//
	for(i=4;i<32;i+=4)
		for(j=0;j<4;j++) coord4(i,j+31);
		
	//-------------------  two-left  -------------------------------//
	for(i=0;i<31;i++) coord4(27,i+31);
	
	//-------------------  two-down  -------------------------------//
	for(i=4;i<32/2;i+=4)
		for(j=0;j<4;j++) coord4(i,j+63-4);
}

void digit10(){
	int i, j;
	//-------------------  one-right(straight)  -------------------//
	for(i=0;i<63-4;i++) coord4(1+32,i);
	//-------------  one-slope//need add more pixils  -------------//
	for(i=0;i<12;i++) coord4(2+i+32,i);
	//------------  one-bottom(horizontal) need_fix:x  ------------//
	for(j=0;j<4;j++) 
		for(i=0;i<32;i+=8) shoe(i+32,j+63-4);
	//--------------------------  zero  ---------------------------//
	digit0();
}

void digit11(){
	int i, j;
	//-------------------  one-right(straight)  -------------------//
	for(i=0;i<63-4;i++) coord4(1+32,i);
	//-------------  one-slope//need add more pixils  -------------//
	for(i=0;i<12;i++) coord4(2+i+32,i);
	//------------  one-bottom(horizontal) need_fix:x  ------------//
	for(j=0;j<4;j++) 
		for(i=0;i<32;i+=8) shoe(i+32,j+63-4);
	//--------------------------  one  ---------------------------//
	digit1();
}

void digit(int x){
	switch(x){
		case 0:digit0();break;
		case 1:digit1();break;
		case 2:digit2();break;
		case 3:draw3();break;
		case 4:draw4();break;
		case 5:draw5();break;
		case 6:draw6();break;
		case 7:draw7();break;
		case 8:draw8();break;
		case 9:draw9();break;
		case 10:digit10();break;
		case 11:digit11();break;
	}
}
