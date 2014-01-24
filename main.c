/* 
  Tetris 3DS Homebrew
  Nop90 - Public domain
*/
#include "main.h"
#include "memory.h"
#include "HID.h"
#include "utils.h"
#include "draw.h"

 
int main(){
  int i;
  clearscreen(TOP_SCREEN | BOTTOM_SCREEN);
  for(i=0; i<400; i=i+3) draw_line(i,0,i,239,0,0,255, TOP_SCREEN);
  draw_rect(19,19,380,220,255,255,0,TOP_SCREEN);  
  for(i=0; i<320; i=i+3) draw_line(i,0,i,239,0,0,255, BOTTOM_SCREEN);
  draw_rect(119,19,300,220,255,255,0,BOTTOM_SCREEN);  
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
    for (j=0; j<12; j++) if(logo[i+j*10]>0) drawsquare(i,14-j,logo[i+j*10]);
}

void initscreens(){
  draw_fillrect(20,20,379,219,128,128,128,TOP_SCREEN); 
  draw_fillrect(120,20,299,219,128,128,128,BOTTOM_SCREEN);  
}

void drawnextpiece(char piece, char state){
  char i,j,color;
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][i+j*4]>0) {
          color=pieces[piece][i+j*4]*state;
          draw_fillrect(((14-j)*20),(i+1)*20,((15-j)*20)-1,((i+2)*20)-1,colors[color][0],colors[color][1],colors[color][2], BOTTOM_SCREEN);
          if(color){ 
            draw_line(((14-j)*20),(i+1)*20,((15-j)*20)-1,(i+1)*20,colors[color][0]>>1,colors[color][1]>>1,colors[color][2]>>1, BOTTOM_SCREEN);
            draw_line(((14-j)*20),(i+1)*20,((14-j)*20),((i+2)*20)-1,colors[color][0]>>1,colors[color][1]>>1,colors[color][2]>>1, BOTTOM_SCREEN);
            draw_line(((15-j)*20)-1,(i+1)*20,((15-j)*20)-1,((i+2)*20)-1,colors[color][0]<<1,colors[color][1]<<1,colors[color][2]<<1, BOTTOM_SCREEN);
            draw_line(((14-j)*20),((i+2)*20)-1,((15-j)*20)-1,((i+2)*20)-1,colors[color][0]<<1,colors[color][1]<<1,colors[color][2]<<1, BOTTOM_SCREEN);
          }
        }
}


void drawpiece(char x, char y, char rot, char piece, char state){
  char i,j,i1,j1;

  switch (rot){
    case 0:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][i+j*4]>0) drawsquare(x+i,y-j,pieces[piece][i+j*4]*(state&1));
    break;
    case 1:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=j;
          j1=3-i;
          if(pieces[piece][i1 + j1 *4]>0) drawsquare(x+i,y-j,pieces[piece][i1 + j1 *4]*(state&1));
        } 
    break;
    case 2:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=3-i;
          j1=3-j;
          if(pieces[piece][i1 + j1 *4]>0) drawsquare(x+i,y-j,pieces[piece][i1 + j1 *4]*(state&1));
        } 
    break;
    case 3:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=3-j;
          j1=i;
          if(pieces[piece][i1 + j1 *4]>0) drawsquare(x+i,y-j,pieces[piece][i1 + j1 *4]*(state&1));
        } 
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
  draw_string("Chose a level then Press Select",0,0,255,255,255, BOTTOM_SCREEN);
  while (click==0){
    HID_new = read_word(HID);
    if(HID_new^HID_old){
      HID_old=HID_new;
      if (!(HID_new & BUTTON_SELECT)) click=1; 
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
  while (click==0){
    HID_new = read_word(HID);
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
  char buf[13];
  char linecompleted,i,j,linefull;
  char board[190];
  unsigned char nextpiece;
  initscreens();
  itoa(level,buf);
  draw_string_rot("LEVEL : ",100,24,255,0,0, BOTTOM_SCREEN);
  draw_string_rot(buf,164,24,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("Lines : 25",100,40,0,255,0, BOTTOM_SCREEN);
  draw_string_rot("Points: ",100,56,0,0,255, BOTTOM_SCREEN);
  itoa(points[0],buf);
  draw_string_rot(buf,164,56,255,255,255, BOTTOM_SCREEN);
  initboard(board);
  nextpiece = read_word(TIMER) % 7;  
  do {  
    nextpiece = playpiece(nextpiece,linecompleted,level,points,board);
  } while ((nextpiece < 8)&(linecompleted<25));
  if (linecompleted < 25){
    return 0;
  } else {
    for (j=17;j>0;j--) {
      linefull=1;
      for (i= 0; i<10;i++) if (board[i+j*10]==0) linefull=0;
      if (linefull) {
        // white line
        for (i= 0; i<10;i++) drawsquare(i, j, 8);
        // pause 1sec
        for (i= 0; i<100;i++) draw_fillrect(0,0,320,7,0,0,0,BOTTOM_SCREEN);
        points[0]+= (100+level*10);
      }
    }
    // pause 2sec
    for (i= 0; i<200;i++) draw_fillrect(0,0,320,7,0,0,0,BOTTOM_SCREEN);
    return 1;
  }
}

void initboard(char* board){
  int i,j;
  for (i=0;i<10;i++)
    for (j=0;j<19;j++) board[i+j*10]=0;
}

char playpiece(char piece, char linecompleted, char level, int* points,char* board){
  char buf[13];
  int i, lines, HID_new,HID_old;
  char x,y,x1,y1,rot,rot1,notfalling;
  char nextpiece,state;
  drawnextpiece(piece, 0);
  nextpiece = read_word(TIMER) % 7;  
  drawnextpiece(nextpiece, 1);
  notfalling=1;
  x=3;
  y=19;
  rot=0;
  HID_old = read_word(HID);
  state=isflying(piece,x,y,rot, board);
  if (state){
    drawpiece(x,y,rot,piece,1);
    while (state){
      for (i=0;i<(500-level*5 - linecompleted>>2)*notfalling;i++) {
        draw_fillrect(0,0,320,7,0,0,0,BOTTOM_SCREEN);
        HID_new = read_word(HID);
        x1=x;
        rot1=rot;
        if(HID_new^HID_old){
          HID_old=HID_new;
          if (!(HID_new & BUTTON_UP)) x1--; 
          if (!(HID_new & BUTTON_DOWN)) x1++;
          if (!(HID_new & BUTTON_RIGHT)) rot1 = (rot+1) % 4;
          if (!(HID_new & BUTTON_LEFT)) {
            x1=x;
            rot1=rot;
            notfalling = 0;
          }
          if (((x1!=x)|(rot1!=rot)) & isflying(piece,x1,y,rot1, board)){
            drawpiece(x,y,rot,piece,0);
            x=x1;
            rot=rot1;
            drawpiece(x,y,rot,piece,1);
          }
        }
      }
      y1=y-1;
      if (isflying(piece,x,y1,rot, board)){
        drawpiece(x,y,rot,piece,0);
        y=y1;
        drawpiece(x,y,rot,piece,1);
      } else {
        lines = gluepiece(x,y,rot,piece,board);
        points[0] += (50 * lines) << lines;
        linecompleted += lines;
        state=0;
        draw_fillrect(196,40,299,47,128,128,128,BOTTOM_SCREEN);
        itoa(25 - linecompleted,buf);
        draw_string_rot(buf,164,40,255,255,255, BOTTOM_SCREEN);
        draw_fillrect(164,56,299,63,128,128,128,BOTTOM_SCREEN);
        itoa(points[0],buf);
        draw_string_rot(buf,196,56,255,255,255, BOTTOM_SCREEN);
      }
    }
  } else return 8;
  return nextpiece;
}

char gluepiece(char x, char y,char rot, char piece,char* board){
  char i,j,i1,j1,k, lines,linefull;

  switch (rot){
    case 0:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          if(pieces[piece][i+j*4]>0) board[x+i + (y-j)*10]=pieces[piece][i+j*4];
        } 
    break;
    case 1:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=j;
          j1=3-i;
          if(pieces[piece][i1 + j1 *4]>0) board[x+i + (y-j)*10]=pieces[piece][i1 + j1 *4];
        } 
    break;
    case 2:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=3-i;
          j1=3-j;
          if(pieces[piece][i1 + j1 *4]>0) board[x+i + (y-j)*10]=pieces[piece][i1 + j1 *4];
        } 
    break;
    case 3:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=3-j;
          j1=i;
          if(pieces[piece][i1 + j1 *4]>0) board[x+i + (y-j)*10]=pieces[piece][i1 + j1 *4];
        } 
    break;
  } 
  lines=0;
  for (j= 0; j<19;j++){
    linefull=1;
    for (i= 0; i<10;i++) if (board[i+j*10]==0) linefull=0;
    if (linefull) {
      lines++;
// white line
      for (i= 0; i<10;i++) drawsquare(i, j, 8);
// pause 0.5sec
      for (i= 0; i<50;i++) draw_fillrect(0,0,320,7,0,0,0,BOTTOM_SCREEN);
// traslo in basso
      for (k= j+1; k<18;k++) {
        for (i= 0; i<10;i++) {
          drawsquare(i, k-1, board[i+k*10]);
          board[i+(k-1)*10] = board[i+k*10];
        } 
      }
      y--;  
// pause 0.1 sec  
      for (i= 0; i<10;i++) draw_fillrect(0,0,320,7,0,0,0,BOTTOM_SCREEN);
    } 
  } 
  return lines;
}

int isflying (char piece, char x, char y, char rot, char* board) {

  char i,j,i1,j1, check;
  check=1;
  switch (rot){
    case 0:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if( (pieces[piece][i+j*4]>0) & ((y-j)<18) & ( ((x+i)<0) | ((x+i)>9) | ((y-j)<0) | (board[x+i + (y-j)*10] >0) ) ) check=0;
    break;
    case 1:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=j;
          j1=3-i;
          if((pieces[piece][i1+j1*4]>0) & ((y-j1)<18) & (((x+i1)<0)|((x+i1)>9)|((y-j1)<0)|(board[x+i + (y-j)*10] >0))) check=0;
      }
    break;
    case 2:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){ 
          i1=3-i;
          j1=3-j;
          if((pieces[piece][i1+j1*4]>0) & ((y-j1)<18) & (((x+i1)<0)|((x+i1)>9)|((y-j1)<0)|(board[x+i + (y-j)*10] >0))) check=0;
      }
    break;
    case 3:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=3-j;
          j1=i;
          if((pieces[piece][i1+j1*4]>0) & ((y-j1)<18) & (((x+i1)<0)|((x+i1)>9)|((y-j1)<0)|(board[x+i + (y-j)*10] >0))) check=0;
      }
    break;
  } 


  return check;
}

