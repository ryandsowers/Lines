// File: lines.c
// 
// Description: This program displays a number of lines (as input by user) of a 
// requested length (as input by user) in random locations in the terminal window.
// The program will clear the diplay, then print one line per second until all lines
// have been printed.
//
// Syntax: This program is run by calling the program name "./lines" followed by two arguments.
// The first argument is the desired number of lines (between 1 and 1000, inclusive).
// The second argument is the desired length of the lines (between 1 and 40, inclusive).
// For example, if the user would like 50 lines of length 5, the program will be called as such:
// ./lines 50 5

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define NUM_VALID_ARGS 3
#define NUM_LINES_IN 1          // index of number of lines input
#define LEN_LINES_IN 2          // index of length of lines input
#define MAX_NUM_LINES 1000
#define MAX_LEN_LINES 40
#define MIN_NUM_LINES 1
#define MIN_LEN_LINES 1
#define FIRST_COL 0             // first column of screen
#define ADVANCE_1 1             // advance 1 from modulo result for screen coords
#define BASE_10 10

int Max_rows = 0;
int Max_cols = 0;

void display_line(int col, int row, int length);

int main(int argc, char const *argv[])
{
        int numLines;
        int lenLines;
        char *ptr;
        char *ptr2;
        int *x = NULL;
        int *y = NULL;
        errno = 0;

        // check for valid number of arguments entered
        if (argc != NUM_VALID_ARGS) {
                printf("ERROR. Expected three inputs: ./days '# of lines' 'length of lines'\n");
                exit(-1);
        }

        // convert number of lines entered to long int and check for valid number of lines as input 
        numLines = strtol(argv[NUM_LINES_IN], &ptr, BASE_10);
        if (numLines < MIN_NUM_LINES || numLines > MAX_NUM_LINES) {
                printf("ERROR. Please enter a number of lines between 1 and 1000 (inclusive).\n");
                exit(-1);
        }

        // convert length of lines entered to long int and check for valid length of lines as input 
        lenLines = strtol(argv[LEN_LINES_IN], &ptr2, BASE_10);
        if (lenLines < MIN_LEN_LINES || lenLines > MAX_LEN_LINES) {
                printf("ERROR. Please enter a line length between 1 and 40 (inclusive).\n");
                exit(-1);
        }

        // get terminal size
        struct winsize win;
        ioctl(0, TIOCGWINSZ, &win);
        Max_rows = win.ws_row;
        Max_cols = win.ws_col;

        // allocate memory to store random coordinates
        x = malloc(sizeof(int) * numLines);
        y = malloc(sizeof(int) * numLines);

        if ((x == NULL) || (y == NULL) || (errno != 0)) {
                perror("malloc failed");
                exit(-1);
        }

        // seed random # generator
        srandom(time(NULL));

        // get random (x,y) coords for requested number of lines
        for (int i = 0; i < numLines; ++i) {
                x[i] = (random() % Max_cols) + ADVANCE_1;
                y[i] = (random() % Max_rows) + ADVANCE_1;
        }

        // clear the screen before displaying lines
        printf("\033[2J");

        // display requested number of lines
        for (int i = 0; i < numLines; ++i) {
                display_line(x[i], y[i], lenLines);
        } 

        // move the cursor to the bottom left of the screen when done
        printf("\033[%d;%dH", Max_rows, FIRST_COL);

        // freeing allocated memory
        free(x);
        x = NULL;
        free(y);
        y = NULL;

        return 0;
}

// display each line by printing the requested number of dashes ("-")
void display_line(int col, int row, int length)
{
        // ensure lines don't print on last row of screen
        if (row == Max_rows) {
                --row;
        }

        // move the cursor to the random location
        printf("\033[%d;%dH", row, col);

        // print the line of requested length
        for (int i = 0; i < length; ++i) {
                // don't let lines wrap at end of window
                if ((col + i) < (Max_cols + ADVANCE_1)) {
                        printf("-");
                }
        }

        // force print the lines and then wait 1 second between each line
        fflush(stdout);
        sleep(1);
}












