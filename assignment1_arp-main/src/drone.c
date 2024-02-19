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



void handle_killing_of_signal(int signo);
void make_log_for_process(char *fileName, char *mode);
void make_log_for_data_into_file(char *fileName, char *mode);



int main()
{
    // Creating files for registring log
    make_log_for_process("./droneLog.txt", "w+");

    make_log_for_data_into_file("droneLog.txt", "w+");
 
    signal(SIGINT, handle_killing_of_signal);
   

   // Initilizng shared memory and opening it
    shared_memory_fd = shm_open("/board", O_RDWR, 0666);
    board_sh_memory = (Board *)mmap(NULL, 
    sizeof(Board), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);

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
    
    // Writing logs through opening a file
    make_file = fopen(nameOfFile, mode_of_file);
    


    if (make_file < 0)
    {
        printf("the file of %s has an error , the error=> %d", nameOfFile, errno);
        exit(1);
    }
 
    fprintf(make_file, "pid is %d =>>>>> registered time is =====> %d  : %d  :  %d ", processID, current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
    fclose(make_file);
}
