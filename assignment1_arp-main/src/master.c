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
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>

void make_log_for_data_into_file(char *nameOfFile, char *mode_of_file);
// Creating child_parent process
int makeFork(const char *app_file, char *arg_list[])
{

  pid_t childParent_instance = fork();

  if (childParent_instance < 0)
  {
    perror("there is an error in making fork process ....");
    return 1;
  }

  else if (childParent_instance != 0)
  {
    return childParent_instance;
  }

  else
  {
    if (execvp(app_file, arg_list) == 0)
      ;
    perror("there is an error in executing file");
    return 1;
  }
}

void handle_killing_of_signal(int signo)
{   

  
  if (signo == SIGINT)
  {
    printf("Signint signal\n\n");

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
    printf("the file of %s has an error , the error => %d", nameOfFile, errno);
    exit(1);
  }

  fprintf(make_file, "pid is %d =>>>>> registered time is =====> %d  : %d  :  %d ", processID, current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
  fclose(make_file);
}

int main()
{

  
  make_log_for_process("./masterLog.txt", "w+");
  

  make_log_for_data_into_file("masterLog.txt", "w+");

  signal(SIGINT, handle_killing_of_signal);

  
  char *aListForWindow[] = {"/usr/bin/konsole", "-e", "./bin/window", NULL};

  /* Develop different processes*/
  pid_t pidServer = makeFork("./bin/server", NULL);
  pid_t  pidDrone = makeFork("./bin/drone", NULL);
  pid_t pidKeyboardManager = makeFork("./bin/keyboard_manager", NULL);
  pid_t pidWatchDog = makeFork("./bin/watch_dog", NULL);
  pid_t pidkonsole = makeFork("/usr/bin/konsole", aListForWindow);

  /* Waiting for all the child processes*/
  int status;
  waitpid(pidServer, &status, 0);
  waitpid(pidDrone, &status, 0);
  waitpid(pidKeyboardManager, &status, 0);
  waitpid(pidWatchDog, &status, 0);
  waitpid(pidkonsole , &status, 0);

  printf("status of main app is  %d\n", status);
  return 0;
}
