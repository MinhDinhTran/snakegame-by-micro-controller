void store_snake(){
 scnt=0;
 int i,j;
 /*
 for(j=0;j<63;j++)
  for(i=0;i<63;i++)
   if(lcd[i][j]==1) coord1(i,j);
 */

 for(i=0;i<snake_len;i++)
  coord1(snakebody[i][0], snakebody[i][1]);

 int tempx = snakebody[0][0];//coordx
 int tempy = snakebody[0][1];//coordy

 switch(dir){
  case 0: snakebody[0][1]++ ;break; //up [][1]=y; [][0]=x
  case 1: snakebody[0][1]-- ;break; //down
  case 2: snakebody[0][0]-- ;break; //left
  case 3: snakebody[0][0]++ ;break; //right [][0]=x ++
 }

 snakebody[1][0] = tempx;//NO.1 =org head -x
 snakebody[1][1] = tempy;//NO.1 =org head -y
 int tailx, taily;
 for(j=1;j<snake_len-2;j++){//move whole snake
   snakebody[j+1][0] = snakebody[j][0];//x next=curr
   snakebody[j+1][1] = snakebody[j][1];//y next=curr
 }
 tailx = snakebody[j][0];
 taily = snakebody[j][1];
 snakebody[j+1][0] = snakebody[j][0];//x next=curr
 snakebody[j+1][1] = snakebody[j][1];//y next=curr
 
 
 
 for(i=0; i<snake_len; i++){
  lcd[snakebody[i][0]][snakebody[i][1]] = 1;
 }
 lcd[tailx][taily] = 0;


 //coord1(32,41);
 display();
}


void display(){
 scnt=0;
 int i;
 /*
 for(i=0; i<snake_len; i++)//print
  coord1(snakebody[i][0], snakebody[i][1]);
 */
 for(i=0; i<64; i++)//print
  for(j=0; j<64; j++)//print 
   if(lcd[i][j] == 1) coord1(i, j);
 

}
