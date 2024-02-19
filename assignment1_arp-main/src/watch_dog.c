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

#define THE_NUMBER_OF_INACTIVING 6
#define LIMITATION_TIME_FOR_INACTING 70
#define TIME_SPECIFIED_FOR_CHECKING 30

void handle_killing_of_signal(int signo)
{
  if (signo == SIGINT)
  {
    printf("great , the SIGINT signal is received!\n");
    kill(getpid(), SIGKILL);
  }
}

bool activity_check_time(int the_hour_of_begining, int the_miutes_of_begining, int the_seconds_of_begining, int second_duration)
{
  time_t time_instance = time(NULL);
  struct tm current_time = *localtime(&time_instance);

  bool status_of_activity = (current_time.tm_hour - the_hour_of_begining) * 3600 + (current_time.tm_min - the_miutes_of_begining) * 60 + (current_time.tm_sec - the_seconds_of_begining) > second_duration;
  if (status_of_activity)
  {
    printf("the time is ended\n");
    return true;
  }
  return false;
}

int main(int argc, char const *argv[])
{
  FILE *file_instance;
  char *token_instance;
  char buffer_list[100];
  int process_parent_ID;
  int last_time_of_activity_hour, last_time_of_activity_minutes, last_time_of_activity_seconds;
  bool status_of_activity_about_process;
  int couting_inactive_process = 0;
  char name_of_files[5][35] = {"./droneLog.txt", "./serverLog.txt", "./keyboard_managerLog.txt", "./windowLog.txt", "./masterLog.txt"};
  char PID_list[5][20] = {"", "", "", "", ""};

  while (1)
  {
    sleep(TIME_SPECIFIED_FOR_CHECKING); // wait for 60 seconds then check the activity staatus of the process
    while (couting_inactive_process < THE_NUMBER_OF_INACTIVING)
    {

      file_instance = fopen(name_of_files[couting_inactive_process], "r");

      if (file_instance < 0)
      {
        printf("there is an error in process =>>>>> %d\n", errno);
        exit(1);
      }

      fseek(file_instance, 0, SEEK_SET);

      fread(buffer_list, 50, 1, file_instance);

      fclose(file_instance);

      token_instance = strtok(buffer_list, ",");
      strcpy(PID_list[couting_inactive_process], token_instance);
      process_parent_ID = atoi(token_instance);

      token_instance = strtok(NULL, ",");
      last_time_of_activity_hour = atoi(token_instance);

      token_instance = strtok(NULL, ",");
      last_time_of_activity_minutes = atoi(token_instance);

      token_instance = strtok(NULL, ",");
      last_time_of_activity_seconds = atoi(token_instance);

      printf("for process %d last activity time is %d:%d:%d \n",
             process_parent_ID, last_time_of_activity_hour, last_time_of_activity_minutes, last_time_of_activity_seconds);

      // Checking the activity in the last 70 sec
      printf("chekcing activity ====> in process\n");
      status_of_activity_about_process = activity_check_time(last_time_of_activity_hour,
       last_time_of_activity_minutes, last_time_of_activity_seconds, THE_NUMBER_OF_INACTIVING);

      if (status_of_activity_about_process)
      {

        printf("the processof %d was deactive in the last 70 sec:(\n\n\n\n", process_parent_ID);
        // Increasing the inactive process
        couting_inactive_process += 1;
        if (couting_inactive_process == THE_NUMBER_OF_INACTIVING)
        {
          printf("all of the process is destroyed:|\n");
          for (int i = 0; i < THE_NUMBER_OF_INACTIVING; i++)
          {

            printf("destroying all of the processes %s\n", PID_list[i]);
            kill(atoi(PID_list[i]), SIGINT);
          }
        }
      }
      else
      {
        // The process is active now!
        printf("it is great , the process of %d is active\n\n", process_parent_ID);
        couting_inactive_process = 0;
        break;
      }
    }
  }
  return 0;
}


