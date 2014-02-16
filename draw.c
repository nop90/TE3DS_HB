/* 
  2d drawing primitives for 3DS - simplified for Te3ds 3DS Homebrew
  Nop90 - Public domain
*/

#include "draw.h"
#include "memory.h"
#include "utils.h"
#include "ascii64.h"

void clearscreen(int screen){
int i;
  if(screen & TOP_SCREEN) for (i=0;i<TOP_WIDTH * TOP_HEIGHT * 3; i++){
     write_byte(TOP_LEFT_FRAME0+i, 0);
     write_byte(TOP_LEFT_FRAME1+i, 0);
     write_byte(TOP_RIGHT_FRAME0+i, 0);
     write_byte(TOP_RIGHT_FRAME1+i, 0);
  }
  if(screen & BOTTOM_SCREEN) for (i=0;i<BOTTOM_WIDTH * BOTTOM_HEIGHT * 3; i++){
     write_byte(BOTTOM_FRAME0+i, 0);
     write_byte(BOTTOM_FRAME1+i, 0);
  }
}

void draw_color(int offset, int screen, char r, char g, char b){
  if(screen & TOP_SCREEN){
    write_byte(offset+TOP_LEFT_FRAME0, b);
    write_byte(offset+TOP_LEFT_FRAME0+1, g);
    write_byte(offset+TOP_LEFT_FRAME0+2, r);
    write_byte(offset+TOP_LEFT_FRAME1, b);
    write_byte(offset+TOP_LEFT_FRAME1+1, g);
    write_byte(offset+TOP_LEFT_FRAME1+2, r);
    write_byte(offset+TOP_RIGHT_FRAME0, b);
    write_byte(offset+TOP_RIGHT_FRAME0+1, g);
    write_byte(offset+TOP_RIGHT_FRAME0+2, r);
    write_byte(offset+TOP_RIGHT_FRAME1, b);
    write_byte(offset+TOP_RIGHT_FRAME1+1, g);
    write_byte(offset+TOP_RIGHT_FRAME1+2, r);
  }
  if(screen & BOTTOM_SCREEN){
    write_byte(offset+BOTTOM_FRAME0, b);
    write_byte(offset+BOTTOM_FRAME0+1, g);
    write_byte(offset+BOTTOM_FRAME0+2, r);
    write_byte(offset+BOTTOM_FRAME1, b);
    write_byte(offset+BOTTOM_FRAME1+1, g);
    write_byte(offset+BOTTOM_FRAME1+2, r);
  }
}

void draw_pixel(int x, int y, char r, char g, char b, int screen){
    int offset = 720*x+720-(y*3)-3;
    draw_color(offset,screen,r,g,b);
}

void draw_char(char letter,int x,int y, char r, char g, char b, int screen){
  int i, k;
  unsigned char mask;
  unsigned char l;

  for (i = 0; i < 8; i++){
    mask = 0b10000000;
    l = ascii_data[letter][i];
    for (k = 0; k < 8; k++){
      if ((mask >> k) & l){
        draw_pixel(k+x,i+y,r,g,b,screen);
      }     
    }
  }
}

void draw_string(char* word, int x,int y, char r, char g, char b, int screen){
    int tmp_x =x;
    int i;
    int line = 0;

    int width;

  switch(screen){
    case BOTTOM_SCREEN:
      width=BOTTOM_WIDTH;
      break;
    default:
      width=TOP_WIDTH;
      break;
  }

    for (i = 0; i <strlen(word); i++){
     
      if (tmp_x+8 > width) {
        line++;
        tmp_x = x;
      }
      draw_char(word[i],tmp_x,y+(line*8),r,g,b, screen);

      tmp_x = tmp_x+8;
    }
}

void draw_string_rot(char* word, int x,int y, char r, char g, char b, int screen){
    int j, k;
    unsigned char mask;
    unsigned char l;
    int tmp_x =x;
    int i;
    int line = 0;

    int width;

  switch(screen){
    case BOTTOM_SCREEN:
      width=BOTTOM_WIDTH;
      break;
    default:
      width=TOP_WIDTH;
      break;
  }

    for (i = 0; i <strlen(word); i++){
     
      if (tmp_x+8 > TOP_HEIGHT) {
        line++;
        tmp_x = x;
      }
      for (j = 0; j < 8; j++){
        mask = 0b10000000;
        l = ascii_data[word[i]][j];
        for (k = 0; k < 8; k++){
          if ((mask >> k) & l){
            draw_pixel(width-j-y-(line*8),k+tmp_x,r,g,b,screen);
          }     
        }
      }

      tmp_x = tmp_x+8;
    }
}

/*
  draw_line - H_line and V_line only
*/

void draw_line( int x1, int y1, int x2, int y2, char r, char g, char b, int screen){

int x, y;
  if (x1 == x2){
    if (y1<y2) for (y = y1; y < y2; y++) draw_pixel(x1,y,r,g,b,screen);
    else for (y = y2; y < y1; y++) draw_pixel(x1,y,r,g,b,screen);
  } else {
    if (x1<x2) for (x = x1; x < x2; x++) draw_pixel(x,y1,r,g,b,screen);
    else for (x = x2; x < x1; x++) draw_pixel(x,y1,r,g,b,screen);
  }
}

void draw_rect( int x1, int y1, int x2, int y2, char r, char g, char b, int screen){
  draw_line( x1, y1, x2, y1, r, g, b, screen);
  draw_line( x2, y1, x2, y2, r, g, b, screen);
  draw_line( x1, y2, x2, y2, r, g, b, screen);
  draw_line( x1, y1, x1, y2, r, g, b, screen);
}

void draw_fillrect( int x1, int y1, int x2, int y2, char r, char g, char b, int screen){
int X1,X2,Y1,Y2,i,j;

  if (x1<x2){ 
    X1=x1;
    X2=x2;
  } else { 
    X1=x2;
    X2=x1;
  } 

  if (y1<y2){ 
    Y1=y1;
    Y2=y2;
  } else { 
    Y1=y2;
    Y2=y1;
  } 

  for(i=X1;i<=X2;i++){
    for(j=Y1;j<=Y2;j++){
      draw_pixel(i,j, r, g, b, screen);
    }
  }
}

