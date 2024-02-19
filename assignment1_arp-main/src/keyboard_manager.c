#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <termios.h>
#include "../include/board.h"
#include "../include/shared_memory.h"
#include <math.h>

// Working zone for the drone
#define AREA_OF_X_MINIMUM 0
#define AREA_OF_X_MAXIMUM 35
#define AREA_OF_Y_MINIMUM 0
#define AREA_OF_Y_MAXIMUM 150

void handle_killing_of_signal(int signo);
void make_log_for_process(char *fileName, char *mode);
void make_log_for_data_into_file(char *fileName, char *mode);

double m_parameter = 1.0, k_parameter = 0.1, time_step_parameter = 0.1;
double x_power = 0, y_power = 0;

// Initial conditions
double x_position_initilizing = 25.0, y_position_initilizing = 25.0;
double x_rate = 0.0, y_rate = 0.001;

// The drone's motion calculation numerically
void drone_movement_formula()
{
    // Velocities calculation
    x_rate += (x_power - k_parameter * (x_rate)) / m_parameter * time_step_parameter;
    y_rate += (y_power - k_parameter * (y_rate)) / m_parameter * time_step_parameter;

    // Updating postions
    x_position_initilizing += x_rate * time_step_parameter + 0.5 *
                                                                 ((x_rate - k_parameter * x_rate) / m_parameter) * pow(time_step_parameter, time_step_parameter);
    y_position_initilizing += y_rate * time_step_parameter + 0.5 *
                                                                 ((y_rate - k_parameter * y_rate) / m_parameter) * pow(time_step_parameter, time_step_parameter);

    // Setting the limitation for working zone of drone
    if (x_position_initilizing < AREA_OF_X_MINIMUM || x_position_initilizing > AREA_OF_X_MAXIMUM)
    {
        x_rate = -(x_rate);
    }
    if (y_position_initilizing < AREA_OF_Y_MINIMUM || y_position_initilizing > AREA_OF_Y_MAXIMUM)
    {
        y_rate = -(y_rate);
    }
}

// User input
void movement_condtion(char move_direction)
{

    switch (move_direction)
    {
    case 'q':
        x_power += -1;
        make_log_for_data_into_file("keyboard_manager.txt", "w+");
        break;
    case 'w':
        x_power += 1;
        make_log_for_data_into_file("keyboard_manager.txt", "w+");
        break;
    case 'e':
        y_power += -1;
        make_log_for_data_into_file("keyboard_manager.txt", "w+");
        break;
    case 'r':
        y_power += 1;
        make_log_for_data_into_file("keyboard_manager.txt", "w+");
        break;
    case 't':
        x_power += -sqrt(2) / 2;
        y_power += sqrt(2) / 2;
        break;
    case 'a':
        x_power += sqrt(2) / 2;
        y_power += -sqrt(2) / 2;
        make_log_for_data_into_file("keyboard_manager.txt", "w+");
        break;
    case 's':
        x_power += sqrt(2) / 2;
        y_power += sqrt(2) / 2;
        make_log_for_data_into_file("keyboard_manager.txt", "w+");
        break;
    case 'd':
        x_power += -sqrt(2) / 2;
        y_power += -sqrt(2) / 2;
        make_log_for_data_into_file("keyboard_manager.txt", "w+");
        break;
    case 'f':
        x_power = 0;
        y_power = 0;
        x_rate = 0;
        y_rate = 0;
        make_log_for_data_into_file("keyboard_manager.txt", "w+");
        break;
    }
}

void way_of_moving_drone(Position *PositionOfDrone, char move_direction)
{

    movement_condtion(move_direction);

    drone_movement_formula();

    // Update position
    PositionOfDrone->x = x_position_initilizing;
    PositionOfDrone->y = y_position_initilizing;
}

int main()
{
    // Creating files for registring log
    make_log_for_process("./keyboard_manager.txt", "w+");

    make_log_for_data_into_file("keyboard_manager.txt", "w+");
 
    signal(SIGINT, handle_killing_of_signal);
   

   // Initilizng shared memory and opening it
    shared_memory_fd = shm_open("/board", O_RDWR, 0666);
    board_sh_memory = (Board *)mmap(NULL, 
    sizeof(Board), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);

    while (1)
    {
        // input order for moving drone
        char ch = board_sh_memory->requried_key;
        way_of_moving_drone(&(board_sh_memory->drone), ch);
        usleep(100000); 
    }

    munmap(board_sh_memory, sizeof(Board));
    close(shared_memory_fd);
    return 0;
}


void handle_killing_of_signal(int signo)
{
    if (signo == SIGINT)
    {
        printf("SIGINT Signal!!!!!\n\n");
        
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
    
    // Writing logs through openning a file
    make_file = fopen(nameOfFile, mode_of_file);
    


    if (make_file < 0)
    {
        printf("the file of %s has an error , the error %d", nameOfFile, errno);
        exit(1);
    }
 
    fprintf(make_file, "pid is %d =>>>>> registered time is =====> %d  : %d  :  %d ", processID, current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
    fclose(make_file);
}
