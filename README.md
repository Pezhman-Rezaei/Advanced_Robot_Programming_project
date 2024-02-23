# Advanced Robotic Programming assignment 1 
## Authors: Seyed Alireza Mortazavi | Pezhman Rezaei Salkhori (AMPR group)
---
## Overview
This project simulates a drone operation environment, integrating multiple components for real-time control, monitoring, and management. It utilizes shared memory for efficient inter-process communication, allowing real-time updates and control flows between a simulated drone, control server, keyboard manager, graphical window interface, and a watchdog for system stability. The logging mechanism ensures that operations are recorded for debugging or analysis. Signal handling provides graceful startup, shutdown, and error handling.
---
## Components
### Drone Simulation:
The Drone component simulates a drone's operational behaviors, acting as the system's core. It handles the drone's movements, status updates, and reactions to commands, leveraging shared memory for communication with other components. Signal handling for interrupt signals (SIGINT) allows for graceful termination and logging of relevant information, offering insights into the simulation's execution and behavior.
### Master:The Master component orchestrates the simulation's operation, including spawning child processes for various roles, initializing logging, and ensuring a coordinated shutdown. It sets up signal handling for a controlled process stoppage, ensuring all processes complete their tasks gracefully.
### Server:
This component establishes shared memory for communication and initiates a server socket waiting for client connections. It manages process information, handles errors and SIGINT signals, and ensures resource cleanup upon termination, maintaining system integrity.
### Keyboard Manager:Applying physics-based calculations, the Keyboard Manager interprets user inputs to simulate drone movement, considering environmental constraints and velocity adjustments. It records actions and states for analysis, ensuring robust keyboard management within the simulation.
### Window:
Utilizing the NCurses library, the Window component offers a graphical interface for the simulation, displaying drone positions and capturing user inputs. It demonstrates inter-process communication, GUI development, and signal handling, enhancing user interaction with the simulation.
### Watchdog:The Watchdog monitors process activity, signaling termination for any process inactive for a specified duration. It serves as a mechanism for ensuring continuous operation, reading log files to check activity statuses and manage process health.
---
### Prerequisites
- Linux-based operating system
- GCC compiler- ncurses library installed (for window component visualization)
### Clone the repository:
```shgit clone https://github.com/Pezhman-Rezaei/Advanced_Robot_Programming_project.git
