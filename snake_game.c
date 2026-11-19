#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <stdio.h>
#include "snake_game.h"
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

const int WIDTH = 31; // one extra for \n
const int HEIGHT = 18;
int snake_axis_x1  = 8;
int snake_axis_x2 = 0;
int snake_axis_y1 = 14;
int snake_axis_y2 = 0;
int fruit_axis_x = 0;
int fruit_axis_y = 0;
int switch_pos = 1;
int right_bound = WIDTH - 2; 
int score = 0;
int timer = 0;
bool is_start  = false;
bool is_boundary = false;



char **grid_board(void) {
    int counter = 0;

    char **grid = (char **)malloc(HEIGHT * sizeof(char *));

    for (int rows = 0; rows < HEIGHT; rows++)
    {
        grid[rows] = (char *)malloc(WIDTH * sizeof(char));
    }

    for (int rows = 0; rows < HEIGHT; rows++) {

        for(int colums = 0; colums < (WIDTH - 1); colums++) {

            if ( (colums == (WIDTH - 2) || colums == 0) && (rows == 0 || rows == (HEIGHT - 1)) ) {
                grid[rows][colums] = '@';
            } // add @ at the edges

            else if ((rows != 0 && rows != (HEIGHT - 1)) && (colums != 0 && colums != (WIDTH - 2))) {
                grid[rows][colums] = ' ';
                // add whites spaces in the middle.
            }
            else {
                grid[rows][colums] = '*';
            }

            counter = colums;
            //track of columns for outside use to add the new line.
        } 
        grid[rows][counter + 1] = '\n';
    }

    return grid;
}

char **add_snake() {
    char **grid = grid_board();


    for (int y = 14; y < 16; y++) {
        grid[snake_axis_x1][y] = '@';
        snake_axis_y2 = y;
    }

    return grid;
}

void add_fruits () 
{
    srand(time(NULL));
    fruit_axis_x = (rand() % 29) + 1;
    fruit_axis_y = (rand() % 16) + 1;

    while (fruit_axis_y == snake_axis_x1 && (fruit_axis_x >= snake_axis_y1 && fruit_axis_x >= snake_axis_y2))
    {
        srand(time(NULL));
        fruit_axis_x = (rand() % 29) + 1;
        fruit_axis_y = (rand() % 16) + 1;
    }

    while((fruit_axis_y == 0 || fruit_axis_y == (HEIGHT - 1)) || (fruit_axis_x == 0 || fruit_axis_x == (WIDTH - 2)))
    {
        srand(time(NULL));
        fruit_axis_x = (rand() % 29) + 1;
        fruit_axis_y = (rand() % 16) + 1;
    }

}

void game() 
{

    initscr();

    keypad(stdscr, TRUE);
    cbreak();

    refresh(); 
    //reload screen 
    char **grid = add_snake();
    add_fruits();
    grid[fruit_axis_y][fruit_axis_x] = '+';
    for (int rows = 0; rows < HEIGHT; rows++) {
                
        for(int colums = 0; colums < WIDTH; colums++) {
            printw("%c", grid[rows][colums]);
        }
    }

    int ch;
    bool is_down = false;
    bool is_up = false;
    bool is_right = false;
    bool is_left = false;
    int key_press;
    snake_axis_x2 = snake_axis_x1;
  
    while (true)
    {
        if ((snake_axis_y2 == (right_bound)) || (snake_axis_y1 == (1 + score)) 
        || (snake_axis_x2 == (1 + score)) || (snake_axis_x1 == (HEIGHT - 2 - score))
        ) 
        {
            is_boundary = true;
            break;
        }
    
        if (is_down || is_right || is_left || is_up)
        {
           timeout(500);
        }
        
        ch = getch();
        
       

        clear(); 

        if (ch == KEY_RIGHT && !is_left) 
        {
            if (key_press == KEY_DOWN) 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x2 + x][snake_axis_y2] = ' ';

                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 + switch_pos][snake_axis_y1 + x] = '@';
                }
                snake_axis_x2 += switch_pos;
                snake_axis_x1 += switch_pos;
            }
            else if (key_press == KEY_UP) 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 - x][snake_axis_y2] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x2 - switch_pos][snake_axis_y1 + x] = '@';
                }
                snake_axis_x2 -= switch_pos;
                snake_axis_x1 -= switch_pos;
            }
            else 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 + x] = ' ';

                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 + x + 1] = '@';
                    snake_axis_y2 = snake_axis_y1 + x + 1;
                }
            
                snake_axis_y1++;
            }
            is_right = true;
            is_up = false;
            is_down = false;
            is_start = true;
        }

        if (ch == KEY_LEFT && !is_right && is_start) 
        {
            if (key_press == KEY_DOWN) 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x2 + x][snake_axis_y2] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 + switch_pos][snake_axis_y1 + x] = '@';
                }
                snake_axis_x2 += switch_pos;
                snake_axis_x1 += switch_pos;
            }
            else if (key_press == KEY_UP) 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 - x][snake_axis_y2] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x2 - switch_pos][snake_axis_y1 + x] = '@';
                }
                snake_axis_x2 -= switch_pos;
                snake_axis_x1 -= switch_pos;
            }
            else 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 + x] = ' ';

                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 - x - 1] = '@';
                    snake_axis_y2 = snake_axis_y1 - x - 1;
                }

                snake_axis_y1--;
            }
            
            is_left = true;
            is_up = false;
            is_down = false;
        }

        if (ch == KEY_DOWN && !is_up) {
            if (!is_down)
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 + x] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 + x][snake_axis_y2] = '@';
                }
                is_down = true;
            }
            else 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 + x][snake_axis_y2] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 + x + 1][snake_axis_y2] = '@';
                }
                snake_axis_x1++;
                snake_axis_x2++;
            }
            is_left = false;
            is_right = false;
            is_start = true;
        }
        
        if (ch == KEY_UP && !is_down) {
            if (!is_up)
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 + x] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 - x][snake_axis_y2] = '@';
                }
                is_up = true;
            }
            else 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 - x][snake_axis_y2] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 - x - 1][snake_axis_y2] = '@';
                }
                snake_axis_x1--;
                snake_axis_x2--;
            }

            is_left = false;
            is_right = false;
            is_start = true;
        }

        if (ch == KEY_LEFT && key_press == KEY_RIGHT)
        {
            ch = 0;
            timer++;
        }

        else if (key_press == KEY_LEFT && ch == KEY_RIGHT)
        {
            ch = 0;
            timer++;
        }

        else if (ch == KEY_DOWN && key_press == KEY_UP)
        {
            ch = 0;
            timer++;
        }
        else if (key_press == KEY_DOWN && ch == KEY_UP)
        {
            ch = 0;
            timer++;
        }   
        else 
        {
            timer = 0;
        }

        if (ch > 0) 
        {
            key_press = ch;
        }
    
        if (ch < 0 || (timer % 10 == 0 && timer > 0)) 
        {
            if (key_press == KEY_LEFT && !is_right)
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 + x] = ' ';

                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 - x - 1] = '@';
                    snake_axis_y2 = snake_axis_y1 - x - 1;
                }

                snake_axis_y1--;
                is_up = false;
                is_down = false;
            }

            if (key_press == KEY_RIGHT && !is_left)
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 + x] = ' ';

                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1][snake_axis_y1 + x + 1] = '@';
                    snake_axis_y2 = snake_axis_y1 + x + 1;
                }
            
                snake_axis_y1++;
                is_up = false;
                is_down = false;
            }
            
            if (key_press == KEY_UP && !is_down) 
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 - x][snake_axis_y2] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 - x - 1][snake_axis_y2] = '@';
                }
                snake_axis_x1--;
                snake_axis_x2--;

                is_right = false;
                is_left = false;
            }

            if (key_press == KEY_DOWN && !is_up)
            {
                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 + x][snake_axis_y2] = ' ';
                }

                for (int x = 0; x < (score + 2); x++)
                {
                    grid[snake_axis_x1 + x + 1][snake_axis_y2] = '@';
                }
                snake_axis_x1++;
                snake_axis_x2++;
                is_right = false;
                is_left = false;
            }
        }

            //for snake in the up position
        if (((snake_axis_x1 == fruit_axis_y) && (snake_axis_y2 == fruit_axis_x)) ||
            //for snake in the down position
            ((snake_axis_x2 == fruit_axis_y) && (snake_axis_y2 == fruit_axis_x)) ||
            //for snake at right position 
            ((snake_axis_x1 == fruit_axis_y) && (snake_axis_y2 == fruit_axis_x)) ||
            //for snake at left position 
            ((snake_axis_x1 == fruit_axis_y) && (snake_axis_y1 == fruit_axis_x))
        ) 
        {

            add_fruits();
            grid[fruit_axis_y][fruit_axis_x] = '+';
            switch_pos++;
            score++;
            
        }

        for (int rows = 0; rows < HEIGHT; rows++) {
                
            for(int colums = 0; colums < WIDTH; colums++) {
                printw("%c", grid[rows][colums]);
            }
        }
        printw("\n score: %d\n", score);
        refresh();
        usleep(300000);
    }

    if(is_boundary) 
    {
        printw("Game Over!\n");
        refresh();
        sleep(1);
    }

    endwin();
}

//(refine the boundary coordinates. need to work on for len > 2)
// and then for movement from up/down to right/left as some extra bit comes up if len > 2.
//fixed backwards function protection.
//fixed snake goes below/above exepected when snake moves from up/down to right/left.