/* bounce.h */

#define BLANK       ' '
#define DFL_SYMBOL  'o'
#define TOP_ROW     0
#define BOT_ROW     LINES
#define LEFT_EDGE   0
#define RIGHT_EDGE  COLS
#define X_INIT      0    /* starting col */
#define Y_INIT      0    /* starting row */
#define TICKS_PER_SEC    100 /* affects speed */
#define X_TTM       5
#define Y_TTM       8

/* the ping pong ball */

struct ppball {
  int y_pos, x_pos,
    y_ttm, x_ttm,
    y_ttg, x_ttg,
    y_dir, x_dir;
  char symbol ;
}
