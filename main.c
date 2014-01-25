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
  draw_string_rot("Tetris 3DS Homebrew v0.1",20,210,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("NOP90",199,311,255,255,255, BOTTOM_SCREEN);
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
          draw_fillrect(((14-j)*20),(i+1)*20+3,((15-j)*20)-1,((i+2)*20)+2,colors[color][0],colors[color][1],colors[color][2], BOTTOM_SCREEN);
          if(color){ 
            draw_line(((14-j)*20),(i+1)*20+3,((15-j)*20)-1,(i+1)*20+3,colors[color][0]>>1,colors[color][1]>>1,colors[color][2]>>1, BOTTOM_SCREEN);
            draw_line(((14-j)*20),(i+1)*20+3,((14-j)*20),((i+2)*20)+2,colors[color][0]>>1,colors[color][1]>>1,colors[color][2]>>1, BOTTOM_SCREEN);
            draw_line(((15-j)*20)-1,(i+1)*20+3,((15-j)*20)-1,((i+2)*20)+2,colors[color][0]<<1,colors[color][1]<<1,colors[color][2]<<1, BOTTOM_SCREEN);
            draw_line(((14-j)*20),((i+2)*20)+2,((15-j)*20)-1,((i+2)*20)+2,colors[color][0]<<1,colors[color][1]<<1,colors[color][2]<<1, BOTTOM_SCREEN);
          }
        }
}

void drawpiece(int x, int y, char rot, char piece, char state){
  int i,j,i1,j1;

  switch (rot){
    case 0:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if(pieces[piece][i+j*4]>0) drawsquare(x+i,y-j,pieces[piece][i+j*4]*(state));
    break;
    case 1:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=j;
          j1=3-i;
          if(pieces[piece][i1 + j1 *4]>0) drawsquare(x+i,y-j,pieces[piece][i1 + j1 *4]*(state));
        } 
    break;
    case 2:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=3-i;
          j1=3-j;
          if(pieces[piece][i1 + j1 *4]>0) drawsquare(x+i,y-j,pieces[piece][i1 + j1 *4]*(state));
        } 
    break;
    case 3:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=3-j;
          j1=i;
          if(pieces[piece][i1 + j1 *4]>0) drawsquare(x+i,y-j,pieces[piece][i1 + j1 *4]*(state));
        } 
    break;
  } 
}

void drawsquare(int x, int y, char color){
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
  char click, sel,i;
  HID_old=0;
  click=0;
  sel=0;
  draw_string_rot("Choose a level",68,74,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("then Press Start",52,82,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("Easy   (Lev. 1)",68,106,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("Medium (Lev. 5)",68,122,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("Hard   (Lev. 9)",68,138,255,255,255, BOTTOM_SCREEN);
  while (click==0){
    for (i=0;i<3;i++) draw_string_rot(">",52,106+(16*i),255,(i==sel) ? 0 : 255,(i==sel) ? 0 : 255, BOTTOM_SCREEN);
    HID_new = read_word(HID);
    if(HID_new^HID_old){
      HID_old=HID_new;
      if (!(HID_new & BUTTON_RIGHT)) sel = (sel+5)%3; 
      if (!(HID_new & BUTTON_LEFT)) sel = (sel+1)%3; 
      if (!(HID_new & BUTTON_START)) click = 1; 
    }
  }
  return (sel * 4)+1;
}

void asknewgame(){
  int HID_new,HID_old;
  char click;
  HID_old=0;
  click=0;
  draw_string_rot("Press start",74,130,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("for a new game",60,138,255,255,255, BOTTOM_SCREEN);
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
  char linecompleted[1];
  int i,j;
  char lineempty;
  char board[190];
  char nextpiece;
  initscreens();
  itoa(level,buf);
  draw_string_rot("LEVEL: ",116,40,255,0,0, BOTTOM_SCREEN);
  draw_string_rot(buf,172,40,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("Lines: ",116,56,0,255,0, BOTTOM_SCREEN);
  draw_string_rot("25",172,56,255,255,255, BOTTOM_SCREEN);
  draw_string_rot("Score: ",116,72,0,0,255, BOTTOM_SCREEN);
  itoa(points[0],buf);
  draw_string_rot(buf,172,72,255,255,255, BOTTOM_SCREEN);
  initboard(board);
  linecompleted[0] = 0;
  nextpiece = read_word(TIMER) % 7;  
  do {  
    nextpiece = playpiece(nextpiece,linecompleted,level,points,board);
  } while ((nextpiece < 8)&(linecompleted[0]<25));
  if (linecompleted[0] < 25){
    return 0;
  } else {
    for (j=17;j>0;j--) {
      lineempty=1;
      for (i= 0; i<10;i++) if (board[i+j*10]>0) lineempty=0;
      if (lineempty) {
        // white line
        for (i= 0; i<10;i++) drawsquare(i, j, 8);
        // pause 
        for (i= 0; i<300;i++) draw_fillrect(0,0,319,0,0,0,0,BOTTOM_SCREEN);
        points[0]+= (100+level*10);
        draw_fillrect(241,164,248,219,128,128,128,BOTTOM_SCREEN);
        itoa(points[0],buf);
        draw_string_rot(buf,172,72,255,255,255, BOTTOM_SCREEN);
      }
    }
    // pause 
    for (i= 0; i<2000;i++) draw_fillrect(0,0,319,0,0,0,0,BOTTOM_SCREEN);
    return 1;
  }
}

void initboard(char* board){
  int i,j;
  for (i=0;i<10;i++)
    for (j=0;j<19;j++) board[i+j*10]=0;
}

char playpiece(char piece, char* linecompleted, char level, int* points,char* board){
  char buf[13];
  int i,j, lines, HID_new,HID_old,delay;
  int x,y,x1,y1;
  char rot,rot1,notfalling;
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
      delay= 500 - level*20 - linecompleted[0];
      if (delay<1) delay=1;
      for (i=0;i<delay*notfalling;i++) {
        draw_fillrect(0,0,319,0,0,0,0,BOTTOM_SCREEN); //slow down counter
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
          if (((x1!=x)||(rot1!=rot)) && isflying(piece,x1,y,rot1, board)){
            drawpiece(x,y,rot,piece,0);
            x=x1;
            rot=rot1;
            drawpiece(x,y,rot,piece,1);
          }
        }
      }
      if (isflying(piece,x,y-1,rot, board)){
        drawpiece(x,y,rot,piece,0);
        y--;
        drawpiece(x,y,rot,piece,1);
      } else {
        lines = gluepiece(x,y,rot,piece,board);
        points[0] += (50 * lines) << lines;
        linecompleted[0] += lines;
        state=0;
        draw_fillrect(241,164,264,219,128,128,128,BOTTOM_SCREEN);
        itoa(25-linecompleted[0],buf);
        draw_string_rot(buf,172,56,255,255,255, BOTTOM_SCREEN);
        itoa(points[0],buf);
        draw_string_rot(buf,172,72,255,255,255, BOTTOM_SCREEN);
      }
    }
  } else return 8;
  return nextpiece;
}

char gluepiece(int x, int y,char rot, char piece,char* board){
  int i,j,i1,j1,k, lines;
  char linefull;

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
// traslo in basso
      for (k= j+1; k<18;k++) {
        for (i= 0; i<10;i++) {
          drawsquare(i, k-1, board[i+k*10]);
          board[i+(k-1)*10] = board[i+k*10];
        } 
      }
      j--;  
    } 
  } 
  return lines;
}

int isflying (char piece, int x, int y, char rot, char* board) {

  int i,j,i1,j1;
  char check;
  check=1;
  switch (rot){
    case 0:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++) if( (pieces[piece][i+j*4]>0) && ((y-j)<19) && ( ((x+i)<0) || ((x+i)>9) || ((y-j)<0) || (board[x+i + (y-j)*10] >0) ) ) check=0;
    break;
    case 1:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=j;
          j1=3-i;
          if((pieces[piece][i1+j1*4]>0) && ((y-j)<19) && ( ((x+i)<0) || ((x+i)>9) || ((y-j)<0) || (board[x+i + (y-j)*10] >0) ) ) check=0;
      }
    break;
    case 2:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){ 
          i1=3-i;
          j1=3-j;
          if((pieces[piece][i1+j1*4]>0) && ((y-j)<19) && (((x+i)<0)||((x+i)>9)||((y-j)<0)||(board[x+i + (y-j)*10] >0))) check=0;
      }
    break;
    case 3:
      for (i=0; i<4; i++)
        for (j=0; j<4; j++){
          i1=3-j;
          j1=i;
          if((pieces[piece][i1+j1*4]>0) && ((y-j)<19) && (((x+i)<0)||((x+i)>9)||((y-j)<0)||(board[x+i + (y-j)*10] >0))) check=0;
      }
    break;
  } 

  return check;
}

