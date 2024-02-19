#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int *pointerParameter;
#define  SHARED_MEMORY_SIZE 100
char *choosen_name_for_shared_memory =  "board";  

int socket_fd_for_server, newSocket_for_server, clilent;
struct sockaddr_in serv_addr, cli_addr;

int* make_shared_memory_for_app(const char *name);

void make_log_for_data_into_file(char *nameOfFile, char *mode_of_file);


// handling signals function
void handle_killing_of_signal(int signo){
    if (signo == SIGINT){
        printf("great , the SIGINT signal is received!\n");

    
     
      if (munmap(pointerParameter, SHARED_MEMORY_SIZE) == -1) {
          perror("unmap sh memory");
          exit(EXIT_FAILURE);
      }

     
      if (close(*pointerParameter) == -1) {
          perror("close sh_memory");
          exit(EXIT_FAILURE);
      }

      
      if (shm_unlink(choosen_name_for_shared_memory) == -1) {
          perror("unlink sh_memoty");
          exit(EXIT_FAILURE);
      }

         // Closing the socket
      if (close(socket_fd_for_server) == -1) {
          perror("closing socket error");
          exit(EXIT_FAILURE);
      }

        // Destroying all the prcoess
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

    // Writing logs through opennig a file
    make_file = fopen(nameOfFile, mode_of_file);

    if (make_file < 0)
    {
        printf("the file of %s has an error , the error=>%d", nameOfFile, errno);
        exit(1);
    }

    fprintf(make_file, "pid is %d =>>>>> registered time is =====> %d  : %d  :  %d ", processID, current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
    fclose(make_file);
}
int main(int argc, char *argv[])
{   
  /*Creating a 'Log File' to record the process information*/
  make_log_for_process("./serverLog.txt","w+");
  make_log_for_data_into_file("serverLog.txt","w+");
  //a 'SIGINT' signal from the 'watchdog process' to the 'motorx process'.
  signal(SIGINT, handle_killing_of_signal);

    pointerParameter = make_shared_memory_for_app(choosen_name_for_shared_memory);
    socket_fd_for_server = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socket_fd_for_server < 0) 
        perror("ERROR opening socket");

    //Config the server address and bind the socket to the IP address
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(3500);
    if (bind(socket_fd_for_server, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        perror("ERROR on binding");
            
    //Waiting for new client to request
    printf ("Server is ready! Waits for new client to request...\n");
    listen(socket_fd_for_server,5);

    //Accepting the request
    clilent = sizeof(cli_addr);
    newSocket_for_server = accept(socket_fd_for_server, (struct sockaddr *) &cli_addr, &clilent);
    printf ("Accepts the request\n");

    //Error Handling
    if (newSocket_for_server < 0)
        perror("ERROR on accept");
}

int* make_shared_memory_for_app(const char *name){
 
  int shared_memory_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  if(shared_memory_fd == -1 ){  
    printf("there is an error in creating sh memory!");
    exit(1);
  }

 
  if(ftruncate(shared_memory_fd, SHARED_MEMORY_SIZE) == -1)
    printf("Trancate failed\n");

  //Mapping the memory address of the calling process to the shared memory 
  int *pointeParameter = (int *)mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);

   //Error Handling
  if (pointeParameter == MAP_FAILED) {
    printf("there is an error in map\n");
  }
    return pointeParameter;
}
