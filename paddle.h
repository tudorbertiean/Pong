//Paddle.h

#define PAD_SYMBOL '|'
#define PAD_SIZE 5

#define PADDLE_X_INIT 40
#define PADDLE_Y_INIT 50


struct paddle
{
  int x_pos, y_pos, pre_y_pos;
  char symbol;
} the_paddle;
