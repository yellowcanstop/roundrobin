# COMP2035 Operating Systems & Concurrency
## Round Robin Process Scheduler
## Group 10
The program is a simulator of a Round-Robin process scheduler written in C with a graphical user interface (GUI) using the IUP library. For the convenience of the module covenor, Method 1 allows direct execution of the program.
## Method 1: Run batch file (RUN_AND_SAVE.bat or RUN_ONLY.bat)
- From the ./run folder, run RUN_AND_SAVE.bat or RUN_ONLY.bat
- RUN_AND_SAVE.bat will save the simulation output in two places: 1) ./run/analysis.txt and 2) a relevant timestamped file inside the ./run/log folder. The ./run/analysis.txt file will automatically open for review.
- RUN_ONLY.bat only prints the simulation output to the terminal. 
- For both batch files, users are prompted in the terminal if they wish to start a new simulation. Press 'Y' to start a new simulation or 'N' to terminate the application.
- Please ensure that roundrobin.exe and iup.dll are in the same folder as the batch files. This should already be the structure of the ./run folder.

## Method 2: Compilation
- For modularity and to facilitiate division of work, more than one c file is used. 
## 2.1. Using command line (gcc)
- Please ensure that you are in the directory where the c files and the iuplib folder are located.
- Please ensure that the iuplib folder (containing the include directory with the iup.h file inside it) is in the same folder as the c files.
- Run the following commands based on your Operating System:
- 2.1.1. For Windows Command Prompt:
```shell

// Compile each source file into an object file
gcc -Wall -g -Wextra -Iiuplib\include -Iiuplib -c .\queue.c -o .\obj\queue.o
gcc -Wall -g -Wextra -Iiuplib\include -Iiuplib -c .\robinui.c -o .\obj\robinui.o
gcc -Wall -g -Wextra -Iiuplib\include -Iiuplib -c .\roundrobin.c -o .\obj\roundrobin.o

// Link the object files together to create the executable
gcc -Liuplib -o .\run\roundrobin.exe .\obj\queue.o .\obj\robinui.o .\obj\roundrobin.o -lgdi32 -luser32 -lkernel32 -lcomctl32 .\iuplib\libiup.a

```
- 2.1.2 For Bash:
```bash

// Compile each source file into an object file
gcc -Wall -g -Wextra -Iiuplib/include -Iiuplib -c ./queue.c -o ./obj/queue.o
gcc -Wall -g -Wextra -Iiuplib/include -Iiuplib -c ./robinui.c -o ./obj/robinui.o
gcc -Wall -g -Wextra -Iiuplib/include -Iiuplib -c ./roundrobin.c -o ./obj/roundrobin.o

// Link the object files together to create the executable
gcc -Liuplib -o ./run/roundrobin.exe ./obj/queue.o ./obj/robinui.o ./obj/roundrobin.o -lgdi32 -luser32 -lkernel32 -lcomctl32 ./iuplib/libiup.a

```
- From the ./run folder, run RUN_AND_SAVE.bat or RUN_ONLY.bat
- RUN_AND_SAVE.bat will save the simulation output in two places: 1) ./run/analysis.txt and 2) a relevant timestamped file inside the ./run/log folder. The ./run/analysis.txt file will automatically open for review.
- RUN_ONLY.bat only prints the simulation output to the terminal. 
- For both batch files, users are prompted in the terminal if they wish to start a new simulation. Press 'Y' to start a new simulation or 'N' to terminate the application.

## 2.2. Using Code::Blocks
- In Code::Blocks, import the directory and open the RoundRobin.cbp file. 
- Build and run from within the IDE.
- Please ensure you have the iuplib folder, roundrobin.c and roundrobin.cbp file in the same folder (same structure as in the repository).
- The simulation output is only printed to the console.