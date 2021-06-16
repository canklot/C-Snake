//To compile and run please do the following
//sudo apt-get install libncurses5-dev libncursesw5-dev
//To compile gcc snake.c -lncurses -o snake
//To run ./snake
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/select.h>
#include <stdlib.h>

#define MAXHEIGHT 23
#define MAXWIDTH 79

int Collision(int *MySnake, int SnakeSize)
{ /* returns 1 if snake hit the walls */

  /* Get x and y coordinates of head*/
  int x = *MySnake;
  int y = *(MySnake + 1);

  /* if snake head X index is more than MAXWIDTH so that means it hit the walls */
  if (x < 0 || x > MAXWIDTH)
  {
    return 1;
  }
  /* if snake head Y index is more than MAXHEIGHT so that means it hit the walls */
  else if (y < 0 || y > MAXHEIGHT)
  {
    return 1;
  }
  /* Checks if snake hit itself */
  for (int i = 2; i < SnakeSize * 2 + 2; i += 2)
  {
    if (x == *(MySnake + i) && y == *(MySnake + i + 1))
    {
      return 1;
    }
  }
  /* if there is no collisions return 0 */
  return 0;
}

void CreateFood(int *FoodX, int *FoodY, int *FoodEaten)
{
  /* if food is eaten creates new one, if not prints old food */
  if (*FoodEaten)
  {
    srand(time(0));                    //currnet time as seed
    *FoodX = (rand() % MAXWIDTH) + 1;  //creates a random number between 0 and MAXWIDTH
    *FoodY = (rand() % MAXHEIGHT) + 1; //creates a random number between 0 and MAXHEIGHT
    *FoodEaten = 0;                    //Turns FoodEaten to 0 when a new food created
  }
  /* prints food at the specified coordinates */
  mvprintw(*FoodY, *FoodX, "*");
}

void CheckEatFood(int *MySnake, int FoodX, int FoodY, int *FoodEaten, int *SnakeSize)
{

  /* Snake head position */
  int x = *MySnake;
  int y = *(MySnake + 1);

  /* if snake head is same place with food */
  if (x == FoodX && y == FoodY)
  {
    *FoodEaten = 1;
    *SnakeSize += 1;

    /* Makes snake longer */
    int snakeLen = *SnakeSize;
    int lastX = *(MySnake + snakeLen * 2 - 2);
    int lastY = *(MySnake + snakeLen * 2 - 2 + 1);
    *(MySnake + snakeLen * 2) = lastX;
    *(MySnake + snakeLen * 2 + 1) = lastY;
  }
  /* Print score, SnakeSize to top left */
  mvprintw(0, 0, "SnakeSize : %d", *SnakeSize);
}

int KeyPressed(void)
{
  /* checks key pressed or not */
  /* Returns True if pressed */
  struct timeval tv;
  fd_set read_fd;

  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&read_fd);
  FD_SET(0, &read_fd);

  if (select(1, &read_fd, NULL, NULL, &tv) == -1)
    return 0;

  if (FD_ISSET(0, &read_fd))
    return 1;

  return 0;
}

void MoveSnake(int *MySnake, int DirectionX, int DirectionY, int Speed, int SnakeSize, char *HeadIn)
{
  /* Create and initialize 2 vars to hold old coordinates */
  int Xold = 0;
  int Yold = 0;

  /* Get current head coordinates */
  int Xcurrent = *MySnake;
  int Ycur = *(MySnake + 1);

  /* Find new head coordinates */
  int Xnew = Xcurrent + DirectionX;
  int Ynew = Ycur + DirectionY;

  /* Assign new values to the snake */
  *(MySnake) = Xnew;
  *(MySnake + 1) = Ynew;

  /* Print head at new coordinates */
  mvprintw(Ynew, Xnew, HeadIn);

  for (int i = 2; i < SnakeSize * 2; i += 2)
  {
    /* Back up old coordinates */
    Xold = Xcurrent;
    Yold = Ycur;

    /* Get current coordinates */
    Xcurrent = *(MySnake + i);
    Ycur = *(MySnake + i + 1);

    Xnew = Xold;
    Ynew = Yold;

    *(MySnake + i) = Xnew;
    *(MySnake + i + 1) = Ynew;

    /* prints the snake on specified coordinates */
    mvprintw(Ynew, Xnew, "#");

    /* My debugger */
    /* char debugger[255];
    sprintf(debugger,"%d", Xcurrent);
    mvprintw(10, 10, debugger); */
  }

  /* Sleep according to Speed parameter */
  /* Horizontal and Vertical Speed diffirent because  */
  /* The font size on the terminal is diffirent for horizontal and vertical lines */
  if (DirectionY != 0)
  {
    usleep(500000 / Speed);
  }

  usleep(1000000 / Speed);
}

/* ---------------------------MAIN---------------------------------- */

int main()
{
  /* Create a 2 dimension matrix to store snake coordinates. */
  /* It needs to be 2 dimensional because snake can go both horizontally and vertically */
  int MySnake[100][2];

  /* Initialize the snake matrix with all zeros */
  for (int i = 0; i < 100; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      MySnake[i][j] = 0;
    }
  }
  /* Inıtıalize starting position */
  MySnake[0][0] = 10;
  MySnake[0][1] = 10;

  /* Initialize key pressed as zero because the game just started */
  int keyPressed = 0;

  /* Inıtıalize starting position to the right */
  int DirectionX = 1;
  int DirectionY = 0;

  /* Speed and size of snake */
  int Speed = 15;
  int SnakeSize = 10;

  /* Create 2 vars to hold food coordinates */
  int FoodX = 0;
  int FoodY = 0;

  /* Inıtıalize FoodEaten */
  int FoodEaten = 1;

  /* Head variable for diffirent directions */
  char *HeadCur = ">";

  /* Start curses  */
  initscr();

  /* Set cursor invisible */
  curs_set(false);

  /* Dont display typed charecters */
  noecho();

  /* MAIN LOOP */
  /* Loop if there is no collision */
  while (!Collision(&MySnake[0][0], SnakeSize))
  {
    /* Clean the terminal */
    erase();

    CreateFood(&FoodX, &FoodY, &FoodEaten);
    MoveSnake(&MySnake[0][0], DirectionX, DirectionY, Speed, SnakeSize, HeadCur);
    CheckEatFood(&MySnake[0][0], FoodX, FoodY, &FoodEaten, &SnakeSize);

    /* Prints virtual screen to real screen */
    /* Turns out if you have getch it also cause refresh */
    refresh();

    /* Change direction with wasd */
    if (KeyPressed())
    {
      keyPressed = getch();

      if (keyPressed == 'w' && !(DirectionY == 1 && DirectionX == 0))
      {
        DirectionY = -1;
        DirectionX = 0;
        HeadCur = "^";
      }
      if (keyPressed == 's' && !(DirectionY == -1 && DirectionX == 0))
      {
        DirectionY = 1;
        DirectionX = 0;
        HeadCur = "v";
      }
      if (keyPressed == 'a' && !(DirectionY == 0 && DirectionX == 1))
      {
        DirectionY = 0;
        DirectionX = -1;
        HeadCur = "<";
      }
      if (keyPressed == 'd' && !(DirectionY == 0 && DirectionX == -1))
      {
        DirectionY = 0;
        DirectionX = 1;
        HeadCur = ">";
      }
    }
  }

  /* Print score when you die */
  erase();
  mvprintw(MAXHEIGHT / 2, MAXWIDTH / 4, "You Died! -  Score : %d", SnakeSize);
  refresh();
  getch();  /* Wait  */
  endwin(); /* End curses */

  return 0;
}
