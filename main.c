#include "main.h"
#include "memory.h"
#include "HID.h"
#include "utils.h"
#include "draw.h"

 
int main(){
  char buf[34];
  do {
    initscreens();
    drawlogo();
    startgame(getlevel());
    asknewgame();
  } while(1);
  return 0;
}

void drawlogo(){
  char i,j;
  for (i=0; i<10; i++)
    for (j=0; j<12; j++) if(logo[i+j*10]>0) drawsquare(i,15-j,logo[i+j*10]);
}

void initscreens(){
  int i;
  clearscreen(TOP_SCREEN | BOTTOM_SCREEN);
  for(i=0; i<400; i=i+3) draw_line(i,0,i,239,0,0,255, TOP_SCREEN);
  draw_rect(18,18,380,220,255,255,0,TOP_SCREEN);  
  draw_fillrect(19,19,379,219,128,128,128,TOP_SCREEN); 
}

void drawpiece(char x, char y, char rot, char piece, char state){
  char i,j;

  switch (rot){
    case 0:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][i+j*4]>0) drawsquare(x+i,y-j,pieces[piece][i+j*4]*(state&1));
    break;
    case 1:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][(i+1)*4-j-1]>0) drawsquare(x+i,y-j,pieces[piece][(i+1)*4-j-1]*(state&1));
    break;
    case 2:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][15 - i -(j*4)]>0) drawsquare(x+i,y-j,pieces[piece][15 - i -(j*4)]*(state&1));
    break;
    case 3:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][(3-i)+j]>0) drawsquare(x+i,y-j,pieces[piece][(3-i)+j]*(state&1));
    break;
  } 
}

void drawsquare(char x, char y, char color){
  if ((x>=0)&(x<10)&(y>=0)&(y<18)){
    draw_fillrect(((y+1)*20),(x+1)*20,((y+2)*20)-1,((x+2)*20)-1,colors[color][0],colors[color][1],colors[color][2], TOP_SCREEN);
    if(color){ 
      draw_line(((y+1)*20),(x+1)*20,((y+2)*20)-1,(x+1)*20,colors[color][0]>>1,colors[color][1]>>1,colors[color][2]>>1, TOP_SCREEN);
      draw_line(((y+1)*20),(x+1)*20,((y+1)*20),((x+2)*20)-1,colors[color][0]>>1,colors[color][1]>>1,colors[color][2]>>1, TOP_SCREEN);
      draw_line(((y+2)*20)-1,(x+1)*20,((y+2)*20)-1,((x+2)*20)-1,colors[color][0]<<1,colors[color][1]<<1,colors[color][2]<<1, TOP_SCREEN);
      draw_line(((y+1)*20),((x+2)*20)-1,((y+2)*20)-1,((x+2)*20)-1,colors[color][0]<<1,colors[color][1]<<1,colors[color][2]<<1, TOP_SCREEN);
    }
  }
}

char getlevel(){
  int HID_new,HID_old;
  char click;
  HID_old=0;
  click=0;
  draw_string("Use arrows to chose a level then Press start",0,0,255,255,255, BOTTOM_SCREEN);
  HID_new = read_word(HID);
  while (click==0){
    if(HID_new^HID_old){
      HID_old=HID_new;
      if (!(HID_new & BUTTON_START)) click=1; 
    }
  }
  return 1;
}

void asknewgame(){
  int HID_new,HID_old;
  char click;
  HID_old=0;
  click=0;
  draw_string("Press start for a new play",0,24,255,255,255, BOTTOM_SCREEN);
  HID_new = read_word(HID);
  while (click==0){
    if(HID_new^HID_old){
      HID_old=HID_new;
      if (!(HID_new & BUTTON_START)) click=1; 
    }
  }
}

void startgame(char level){
  int points[1];
  points[0]=0;
  while (playlevel(level++,points));
}

char playlevel(char level, int* points){
  char board[180];
  char nextpiece;
  initscreens();
  initboard(board);
  nextpiece = (read_word(TIMER) % 7) +1;  
  do {  
    nextpiece = playpiece(nextpiece,level,points,board);
  } while (nextpiece < 8);
  return (nextpiece > 8 ? 0 : 1);
}

void initboard(char* board){
  int i,j;
  for (i=0;i<10;i++)
    for (j=0;j<18;j++) board[i+j*10]=0;
}

char playpiece(char piece, char level, int* points,char* board){
  int i, lines, HID_new,HID_old;
  char x,y,x1, rot,rot1;
  char nextpiece;
  nextpiece = (read_word(TIMER) % 7) +1;  
  x=4;
  y=20;
  rot=0;
  HID_old = 0;

  if (isflying(piece,x,y,rot, board)){
    drawpiece(x,y,rot,piece,1);
    while (isflying (piece,x,y,rot, board)){
      for (i=0;i<6000;i++) {
        HID_new = read_word(HID);
        x1=x;
        rot1=rot;
        if(HID_new^HID_old){
          HID_old=HID_new;
          if (!(HID_new & BUTTON_UP) & (x>0)) x1--; 
          if (!(HID_new & BUTTON_DOWN) & (x<10)) x1++;
          if (!(HID_new & BUTTON_LEFT)) ;
          if (!(HID_new & BUTTON_RIGHT)) rot = (rot+1) & 3;
          if (((x1!=x)|(rot1!=rot))&isflying(piece,x1,y,rot1, board)){
            drawpiece(x,y,rot,piece,0);
            x=x1;
            drawpiece(x,y,rot,piece,1);
          } else {
            lines = gluepiece(x,y,rot,piece,board);
            if (lines) points[0] += (50 * lines) << lines;
          }
        }
      }
      drawpiece(x,y,rot,piece,0);
      y=y-1;
      drawpiece(x,y,rot,piece,1);
    }
  } else return 8;
  return nextpiece;
}

char gluepiece(char x, char y,char rot, char piece,char* board){
  char i,j,k, lines,linefull;

  switch (rot){
    case 0:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][i+j*4]>0) board[x+i + (y-j)*18]=pieces[piece][i+j*4];
    break;
    case 1:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][(i+1)*4-j-1]>0) board[x+i + (y-j)*18]=pieces[piece][(i+1)*4-j-1];
    break;
    case 2:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][15 - i -(j*4)]>0) board[x+i +(y-j)*18]=pieces[piece][15 - i -(j*4)];
    break;
    case 3:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][(3-i)*4+j]>0) board[x+i +(y-j)*18]=pieces[piece][(3-i)*4+j]; 
    break;
  } 
  lines=0;
  for (j= 0; j<18;j++){
    linefull=1;
    for (i= 0; i<10;i++) if (board[i+j*18]==0) linefull=0;
    if (linefull) {
      lines++;
// evidenzio
      for (i= 0; i<10;i++) drawsquare(i, j, 8);
// pausa 1sec
      for (i= 0; i<1000;i++);
// traslo in basso
      for (k= j+1; j<18;j++) {
        for (i= 0; i<10;i++) {
          drawsquare(i, j, board[i+k*18]);
          board[i+(k-1)*18] = board[i+k*18];
        } 
      } 
// pausa 1sec   
      for (i= 0; i<1000;i++);
     j--;
    } 
  } 
  return lines;
}

int isflying (char piece, char x, char y, char rot, char* board) {
  char i,j, check;
  check=0;
  switch (rot){
    case 0:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if((pieces[piece][i+j*4]>0) & (((x+i)<0)|((x+i)>9)|((y+j)<0)|(board[x+i + (y-j)*18] !=0))) check=1;
    break;
    case 1:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if((pieces[piece][(i+1)*4-j-1]>0) & (((x+i)<0)|((x+i)>9)|((y+j)<0)|(board[x+i + (y-j)*18] !=0))) check=1;
    break;
    case 2:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if((pieces[piece][15 - i -(j*4)]>0) & (((x+i)<0)|((x+i)>9)|((y+j)<0)|(board[x+i + (y-j)*18] !=0))) check=1;
    break;
    case 3:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if((pieces[piece][(3-i)*4+j]>0) & (((x+i)<0)|((x+i)>9)|((y+j)<0)|(board[x+i + (y-j)*18] !=0))) check=1; 
    break;
  } 


  return check;
}

