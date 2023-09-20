/* TODO
 * need to add AM and PM
 * need to make ASCII characters bigger without changing the underlying data structure
 * need to add color
 * need to respond to user input and add option for standard and military time
 */


#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>

/* 00:00AM */
#define HOUR_LEN 3
#define MINUTE_LEN 3
#define AM_PM_LEN 3
#define TOTAL_LEN 7

/* 7-segment numbers are displayed
 * on a 3x5 grid */
#define GRID_WIDTH 3
#define GRID_HEIGHT 5
#define PRINT_WIDTH 17

/* used for 7-segment display */
#define ON 1
#define OFF 0

/* in seconds */
#define SLEEP_TIME 0.01

void get_time(char hour[], char minute[], char am_pm[])
{
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    strftime(hour, HOUR_LEN, "%I", time_info);
    strftime(minute, MINUTE_LEN, "%M", time_info);
    strftime(am_pm, AM_PM_LEN, "%p", time_info);
}

void print_seven_segment(char digit, unsigned int y_pos, unsigned int x_pos)
{
    const int seven_segment[10][GRID_HEIGHT][GRID_WIDTH] = {
        {{ON,  ON, ON}, // 0
         {ON, OFF, ON},
         {ON, OFF, ON},
         {ON, OFF, ON},
         {ON,  ON, ON}},
        {{OFF, OFF, ON}, // 1
         {OFF, OFF, ON},
         {OFF, OFF, ON},
         {OFF, OFF, ON},
         {OFF, OFF, ON}},
        {{ON, ON, ON},   // 2
         {OFF, OFF, ON},
         {ON, ON, ON},
         {ON, OFF, OFF},
         {ON, ON, ON}},
        {{ON, ON, ON}, // 3
         {OFF, OFF, ON},
         {ON, ON, ON},
         {OFF, OFF, ON},
         {ON, ON, ON}},
        {{ON, OFF, ON}, // 4
         {ON, OFF, ON},
         {ON, ON, ON},
         {OFF, OFF, ON},
         {OFF, OFF, ON}},
        {{ON, ON, ON}, // 5
         {ON, OFF, OFF},
         {ON, ON, ON},
         {OFF, OFF, ON},
         {ON, ON, ON}},
        {{ON, ON, ON}, // 6
         {ON, OFF, OFF},
         {ON, ON, ON},
         {ON, OFF, ON},
         {ON, ON, ON}},
        {{ON, ON, ON}, // 7
         {OFF, OFF, ON},
         {OFF, OFF, ON},
         {OFF, OFF, ON},
         {OFF, OFF, ON}},
        {{ON,  ON, ON}, // 8
         {ON, OFF, ON},
         {ON, ON, ON},
         {ON, OFF, ON},
         {ON,  ON, ON}},
        {{ON, ON, ON},
         {ON, OFF, ON},
         {ON, ON, ON},
         {OFF, OFF, ON},
         {OFF, OFF, ON}}

    };


    move(y_pos, x_pos);
    if (isdigit(digit)) {
        int idx = digit-'0';
        for (int row = 0; row < GRID_HEIGHT; row++) {
            for (int col = 0; col < GRID_WIDTH; col++) {
                move(y_pos+row, x_pos+col);
                seven_segment[idx][row][col] ? printw("#") : printw(" ");
            }
        }
    } else if (digit == ':') {
        printw("#");
        mvprintw(y_pos+2, x_pos, "#");
    } else if (digit == 'P') {
        printw("###");
        mvprintw(y_pos+1, x_pos, "# #");
        mvprintw(y_pos+2, x_pos, "###");
        mvprintw(y_pos+3, x_pos, "#  ");
        mvprintw(y_pos+4, x_pos, "#  ");
    } else if (digit == 'A') {
        printw(" # ");
        mvprintw(y_pos+1, x_pos, "# #");
        mvprintw(y_pos+2, x_pos, "###");
        mvprintw(y_pos+3, x_pos, "# #");
        mvprintw(y_pos+4, x_pos, "# #");
    } else if (digit == 'M') {
        // can't fit an M in a 3x5 grid
    }
}

void print_time(char hour[], char minute[], char am_pm[])
{
    unsigned int window_height, window_width;
    unsigned int center_y, center_x;

    getmaxyx(stdscr, window_height, window_width);
    center_y = (window_height - GRID_HEIGHT) / 2;
    center_x = (window_width - PRINT_WIDTH) / 2;

    unsigned int y = center_y;
    unsigned int x = center_x;

    for (int i = 0; i < HOUR_LEN-1; i++) {
        print_seven_segment(hour[i], y, x);
        x += GRID_WIDTH+1;
    }
    print_seven_segment(':', y+1, x);
    x += 2;
    for (int i = 0; i < MINUTE_LEN-1; i++) {
        print_seven_segment(minute[i], y, x);
        x += GRID_WIDTH+1;
    }
    for (int i = 0; i < AM_PM_LEN-1; i++) {
        print_seven_segment(am_pm[i], y, x);
        x += GRID_WIDTH;
    }
}

int main(void)
{
    char hour[HOUR_LEN];
    char minute[MINUTE_LEN];
    char am_pm[AM_PM_LEN];

    initscr();
    cbreak();
    noecho();
    curs_set(0);


    while (TRUE) {
        get_time(hour, minute, am_pm);
        print_time(hour, minute, am_pm);
        refresh();
        sleep(SLEEP_TIME);
    }

    endwin();
    return 0;
}
