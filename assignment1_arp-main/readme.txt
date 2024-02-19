 The app made from different components . 
 drone , keyboard_manager , master , server , watch_dogs , window 
 and alos this has two header file which is in include folder ,
 I want to explain about this components now .

 At first I want to explain about the drone component.
 the c file of drone is a simulation that utilizes shared memory 
to communicate with other processes. The drone's motion is calculated numerically
 based on physical parameters, and its position is updated according to user input 
 received from a shared memory board. The code also includes signal handling for interrupt 
 signals (SIGINT) to gracefully terminate the program and log relevant information. 
 Additionally, the program creates log files for process information and data, providing 
 insights into the execution and behavior of the drone simulation.

the next component is master.c 
Child Processes Creation:

It creates several smaller programs (child processes) that represent different parts of a system.
For example, there are processes for a server, drone, , keyboard_manager , watchdog, and a 
special one to open a new terminal window.

Logging Information:
It keeps a log of important information about these processes, 
like their unique IDs (PIDs) and the time when they started.

Handling Signals:
It's set up to handle a specific signal (SIGINT), 
allowing for a controlled way to stop one of the processes.

Wait for Completion:
The main program waits for all the created processes to 
finish their tasks before it completes. This ensures a 
coordinated and orderly shutdown of the entire system



the next one is server.c

This server component  establishes a server using sockets, 
manages shared memory, 
and handles signals. It creates shared memory for inter-process communication,
initializes a server socket, and waits for client connections.
The program logs process information and data into files, 
handling errors and signals like SIGINT. Upon receiving a SIGINT signal, 
it cleans up resources, unlinks shared memory, closes sockets, and terminates the process.'
The code demonstrates basic networking and inter-process communication concepts in C.


about watch_dog.C

This watch_dog componet monitors the activity status of multiple processes by analyzing 
their last recorded activity times. It reads log files,
 extracts process IDs and timestamps, and checks for inactivity over a specified duration. 
 If any process remains inactive for a set period, the program signals their termination. 
 The code effectively serves as a watchdog mechanism for process activity, ensuring continuous operation.

about window.C

This window component uses the NCurses library to create a simple interactive window for a drone simulation.
 It reads shared memory containing drone  positions, updates the display accordingly, 
 and captures user input to control the drone. The code handles signals, logs process information, and 
 efficiently manages the graphical interface. It demonstrates the integration of inter-process communication, 
graphical user interface development, and signal handling in a drone simulation environment.