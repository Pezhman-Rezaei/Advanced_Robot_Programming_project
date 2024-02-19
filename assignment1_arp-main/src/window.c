#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include "../include/board.h"
#include "../include/shared_memory.h"

// Boundries dimensions
#define WIDTH 150
#define HEIGHT 35


void make_log_for_data_into_file(char *nameOfFile, char *mode_of_file);
void handle_killing_of_signal(int signo)
{
    if (signo == SIGINT)
    {
        printf("great , the SIGINT signal is received!\n");
        munmap(board_sh_memory, sizeof(Board));
        close(shared_memory_fd);
        kill(getpid(), SIGKILL);
    }
}

void make_log_for_process(char *nameOfFile, char *mode_of_file)
{
    FILE *make_file;
    if (!remove(nameOfFile))
    {
        printf("ok great , the old file for logs has removed \n");
    }
    if (make_file = fopen(nameOfFile, mode_of_file))
    {
        printf("woow , a new filed for logs is created ,\n");
        make_log_for_data_into_file(nameOfFile, mode_of_file);
    }
    else
    {
        printf("there  is an error in creating a file for logs !\n");
    }
}

void make_log_for_data_into_file(char *nameOfFile, char *mode_of_file)
{
    FILE *make_file;
    int processID = getpid();

    time_t time_instance = time(NULL);
    struct tm current_time = *localtime(&time_instance);

    // Writing logs
    make_file = fopen(nameOfFile, mode_of_file);

    if (make_file < 0)
    {
        printf("the file of %s has an error , the error=>%d", nameOfFile, errno);
        exit(1);
    }

    fprintf(make_file, "pid is %d =>>>>> registered time is =====> %d  : %d  :  %d ", processID, current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
    fclose(make_file);
}

// Draw border to show the working zone
void make_border_for_window()
{
    // Horizontal borders
    attron(COLOR_PAIR(1)); 
    for (int i = 0; i < WIDTH; ++i)
    {
        mvaddch(0, i, '!');                 
        mvaddch(HEIGHT - 1, i, '!'); 
    }
    attroff(COLOR_PAIR(1));

    // Vertical borders
    attron(COLOR_PAIR(1)); 
    for (int i = 0; i < HEIGHT; ++i)
    {
        mvaddch(i, 0, '!');                
        mvaddch(i, WIDTH - 1, '!'); 
    }
    attroff(COLOR_PAIR(1));
}

// Initializing colors
void color_initing()
{
    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);  
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    }
}

void make_char(KeyCharacter *character)
{
    wattron(stdscr, COLOR_PAIR(character->used_colors));
    mvaddch(character->row, character->col, character->charachter_for_symbols);
    wattroff(stdscr, COLOR_PAIR(character->used_colors));
}


void getInput(Board *sh_memory)
{
    // User input for drone movement
    char ch = getch();
    sh_memory->requried_key = ch;
}

int main()
{


    make_log_for_process("./windowLog.txt", "w+");

    make_log_for_data_into_file("windowLog.txt", "w+");
 
    signal(SIGINT, handle_killing_of_signal);

    int first_resize = TRUE;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);  
    timeout(200); 

    color_initing();

    shared_memory_fd = shm_open("/board", O_RDWR, 0666);
    board_sh_memory = (Board *)mmap(NULL, sizeof(Board), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);

    while (1)
    {
        clear(); 

    
        make_border_for_window();

  
        KeyCharacter drone_character = {
            .row = board_sh_memory->drone.x,
            .col = board_sh_memory->drone.y,
            .charachter_for_symbols = 'x',
            .used_colors = 3,
            .status_of_activity = 1};
        make_char(&drone_character);

    

        refresh();

       
        getInput(board_sh_memory);
    }

    munmap(board_sh_memory, sizeof(Board));
    close(shared_memory_fd);
    endwin();
    return 0;
}

