/**
   Tudor Bertiean, 130318270, bert8270@mylaurier.ca
   Wednesday March 18, 2015
   Mypong Game
 **/


#include <stdio.h>
#include <sys/time.h>
#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
//#include "paddle.h"
//#include "bounce.h"

#define BLANK ' '
#define DFL_SYMBOL 'o'
#define PAD_SYMBOL '|'
#define PAD_SIZE 5

#define TOP_ROW 5
#define BOT_ROW 25
#define LEFT_EDGE 10
#define RIGHT_EDGE 50

#define X_INIT 15
#define Y_INIT 15
#define PADDLE_X_INIT 40
#define PADDLE_Y_INIT 5

#define TICKS_PER_SEC 50
#define X_TTM 7
#define Y_TTM 14


struct ppball {
  int y_pos, x_pos,
    y_ttm, x_ttm,
    y_ttg, x_ttg,
    y_dir, x_dir;
  char symbol ;
} the_ball;

struct paddle
{
  int x_pos, y_pos, pre_y_pos;
  char symbol;
};

void set_up();
void wrap_up();
void ball_move(int);
void draw_rectangle();
void draw_paddle();
int bounce_or_lose(struct ppball *bp);
void game_over();
struct paddle the_paddle1;
struct paddle the_paddle2;

int main()
{
  int c;
  set_up();
  while ( ( c = getchar()) != 'Q' ){
    if ( c == 'f' )     the_ball.x_ttm--;
    else if ( c == 's' ) the_ball.x_ttm++;
    else if ( c == 'F' ) the_ball.y_ttm--;
    else if ( c == 'S' ) the_ball.y_ttm++;
    else if ( c == 'p' ) {
      if(the_paddle1.y_pos > TOP_ROW) {
	the_paddle1.pre_y_pos = the_paddle1.y_pos;
	the_paddle1.y_pos--;
      }
    }
    else if ( c == 'l' )  {
      if(the_paddle1.y_pos < BOT_ROW - PAD_SIZE) {
	the_paddle1.pre_y_pos = the_paddle1.y_pos;
	the_paddle1.y_pos++;
      }
    }
    else if ( c == 'q' ) {
      if(the_paddle2.y_pos > TOP_ROW) {
	the_paddle2.pre_y_pos = the_paddle2.y_pos;
	the_paddle2.y_pos--;
      }
    }
    else if ( c == 'a' )  {
      if(the_paddle2.y_pos < BOT_ROW - PAD_SIZE) {
	the_paddle2.pre_y_pos = the_paddle2.y_pos;
	the_paddle2.y_pos++;
      }
    }
  }
  wrap_up();
}

void set_up()
{
  the_ball.y_pos = Y_INIT;
  the_ball.x_pos = X_INIT;
  the_ball.y_ttg = the_ball.y_ttm = Y_TTM ;
  the_ball.x_ttg = the_ball.x_ttm = X_TTM ;
  the_ball.y_dir = 1  ;
  the_ball.x_dir = 1  ;

  the_ball.symbol = DFL_SYMBOL ;
  the_paddle1.symbol = PAD_SYMBOL;
  the_paddle1.y_pos = TOP_ROW*2;
  the_paddle1.x_pos = RIGHT_EDGE;

  the_paddle1.pre_y_pos = TOP_ROW;

  the_paddle2.symbol = PAD_SYMBOL;
  the_paddle2.y_pos = TOP_ROW*2;
  the_paddle2.x_pos = LEFT_EDGE;

  the_paddle2.pre_y_pos = TOP_ROW;

  initscr();
  noecho();
  crmode();

  signal( SIGINT , SIG_IGN );

  mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
  draw_rectangle();  
  draw_paddle();

  refresh();
  
  signal( SIGALRM, ball_move );
  set_ticker( 1000 / TICKS_PER_SEC ); /* send millisecs per tick */
}

void wrap_up()
{
  set_ticker( 0 );
  endwin();
}

void ball_move(int signum)
{
  int y_cur, x_cur, moved;
  signal( SIGALRM , SIG_IGN ); /* dont get caught now */
  y_cur = the_ball.y_pos ; /* old spot*/
  x_cur = the_ball.x_pos ;
  moved = 0 ;

  if ( the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1 ){
    the_ball.y_pos += the_ball.y_dir ;/* move*/
    the_ball.y_ttg = the_ball.y_ttm  ;/* reset*/
    moved = 1;
  }

  if ( the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1 ){
    the_ball.x_pos += the_ball.x_dir ;/* move*/
    the_ball.x_ttg = the_ball.x_ttm  ;/* reset*/
    moved = 1;
  }

  if ( moved ){
    mvaddch( y_cur, x_cur, BLANK );
    mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol );

    bounce_or_lose( &the_ball );  
   
    if ( the_ball.x_pos > RIGHT_EDGE || the_ball.x_pos < LEFT_EDGE){
      moved = 0;
      mvaddch( the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
      game_over();
      }
  }

  draw_paddle();
  move(LINES-1,COLS-1);
  refresh();
  signal( SIGALRM, ball_move);
}

void game_over(){
  standout();
  move((TOP_ROW+BOT_ROW)/2,(RIGHT_EDGE+LEFT_EDGE)/2);
  addstr("GAME OVER");
  refresh();
  set_ticker(0);
  endwin();
  exit(1);
}

int bounce_or_lose(struct ppball *bp)
{
  int return_val = 0;
  if (bp->y_pos == TOP_ROW+1){
    bp->y_dir = 1;
    return_val = 1;
  }
  else if(bp->y_pos == BOT_ROW-1){
    bp->y_dir = -1;
    return_val = 1;
  }
  else if(bp->x_pos == RIGHT_EDGE-1){
    if(bp->y_pos == the_paddle1.y_pos)
      bp->x_dir = -1;
    if(bp->y_pos == the_paddle1.y_pos+1)
      bp->x_dir = -1;
    if(bp->y_pos == the_paddle1.y_pos+2)
      bp->x_dir = -1;
    if(bp->y_pos == the_paddle1.y_pos+3)
      bp->x_dir = -1;
    if(bp->y_pos == the_paddle1.y_pos+4)
      bp->x_dir = -1;
  }
  if(bp->x_pos == LEFT_EDGE+1){
    if(bp->y_pos == the_paddle2.y_pos)
      bp->x_dir = 1;
    if(bp->y_pos == the_paddle2.y_pos+1)
      bp->x_dir = 1;
    if(bp->y_pos == the_paddle2.y_pos+2)
      bp->x_dir = 1;
    if(bp->y_pos == the_paddle2.y_pos+3)
      bp->x_dir = 1;
    if(bp->y_pos == the_paddle2.y_pos+4)
      bp->x_dir = 1;
  }
  return return_val;

}


void draw_rectangle()
{
  int i,j;
  // draw the top boundary 
  for( i = LEFT_EDGE; i <= RIGHT_EDGE; i++)
    {
      move(TOP_ROW,i);
      addstr("-");
    }

  // draw the left and bottom side
  
  for( i = LEFT_EDGE; i <= RIGHT_EDGE; i++)
    {
      move(BOT_ROW,i);
      addstr("-");
    }

}

void draw_paddle() {
 
  int i;
  for( i = the_paddle1.pre_y_pos; i < the_paddle1.pre_y_pos + 5; i++){
    mvaddch( i , the_paddle1.x_pos, BLANK);
  } 

  for(i = the_paddle1.y_pos; i < the_paddle1.y_pos + 5; i++){
    mvaddch( i, the_paddle1.x_pos, the_paddle1.symbol);
  }
  
  for( i = the_paddle2.pre_y_pos; i < the_paddle2.pre_y_pos + 5; i++){
    mvaddch( i , the_paddle2.x_pos, BLANK);
  } 

  for(i = the_paddle2.y_pos; i < the_paddle2.y_pos + 5; i++){
    mvaddch( i, the_paddle2.x_pos, the_paddle2.symbol);
  }

}

int set_ticker( int n_msecs ) {
  struct itimerval new_timeset;
  long n_sec, n_usecs;
  n_sec = n_msecs / 1000; /* int part */
  n_usecs = ( n_msecs % 1000 ) * 1000L; /* remainder */
  new_timeset.it_interval.tv_sec = n_sec; /* set reload */
  new_timeset.it_interval.tv_usec = n_usecs; /* new ticker value */
  new_timeset.it_value.tv_sec = n_sec; /* store this */
  new_timeset.it_value.tv_usec = n_usecs; /* and this */
  return setitimer( ITIMER_REAL, &new_timeset, NULL );
} /* set_ timer */ 
