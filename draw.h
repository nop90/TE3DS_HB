#ifndef DRAW_H
#define DRAW_H

#define TOP_SCREEN 1
#define BOTTOM_SCREEN 2

void draw_color(int offset, int address, char r, char g, char b);
void draw_pixel(int x, int y, char r, char g, char b, int screen);
void draw_char(char letter,int x,int y, char r, char g, char b, int screen);
void draw_string(char* word, int x,int y, char r, char g, char b, int screen);
void draw_line( int x1, int y1, int x2, int y2, char r, char g, char b, int screen);
void draw_rect( int x1, int y1, int x2, int y2, char r, char g, char b, int screen);
void draw_fillrect( int x1, int y1, int x2, int y2, char r, char g, char b, int screen);
#endif
