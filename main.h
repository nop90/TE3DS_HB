#ifndef MAIN_H
#define MAIN_H

static char colors[9][3] = {{128,128,128},{0,127,0},{127,0,0},{0,0,127},{0,127,127},{127,0,127},{127,127,0},{63,63,127},{200,200,200}};
static char logo[120] = 
    {1,1,1,2,2,0,0,0,5,5,
     0,1,3,3,2,4,4,0,5,0,
     0,1,3,2,2,4,0,4,5,0,
     0,1,3,3,2,4,0,4,0,5,
     0,1,3,2,2,4,0,4,5,5,
     0,0,3,3,0,4,4,0,0,0,
     0,0,0,0,0,0,0,0,0,0,
     0,0,6,0,6,7,7,0,0,0,
     0,0,6,0,6,7,0,7,0,0,
     0,0,6,6,6,7,7,0,0,0,
     0,0,6,0,6,7,0,7,0,0,
     0,0,6,0,6,7,7,0,0,0}; 

static char pieces[7][16] = 
    {{0,0,0,0,
      0,0,1,1,
      0,1,1,0,
      0,0,0,0},

     {0,0,0,0,
      0,2,2,0,
      0,0,2,2,
      0,0,0,0},

     {0,0,0,0,
      0,3,3,3,
      0,0,3,0,
      0,0,0,0},

     {0,0,0,0,
      0,0,0,4,
      0,4,4,4,
      0,0,0,0},

     {0,0,0,0,
      0,5,0,0,
      0,5,5,5,
      0,0,0,0},

     {0,0,0,0,
      0,6,6,0,
      0,6,6,0,
      0,0,0,0},

     {0,0,0,0,
      0,0,0,0,
      7,7,7,7,
      0,0,0,0}};


int main();
void initscreens();
void drawlogo();
void initboard(char* board);
void drawpiece(int x, int y, char rot, char piece, char state);
void drawnextpiece(char piece, char state);
void drawsquare(int x, int y, char color);
char getlevel();
char playlevel(char level, int* points);
void startgame(char level);
char playpiece(char piece, char* linecompleted, char level, int* points,char* board);
char gluepiece(int x, int y, char rot, char piece,char* board);
int isflying (char piece, int x, int y, char rot, char* board);
void asknewgame();

void getnickname(char* buf);
 
#endif

