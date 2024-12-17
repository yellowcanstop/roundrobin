# COMP2035 Operating Systems & Concurrency
## Round Robin Process Scheduler
## Group 10
The program is a simulator of a Round-Robin process scheduler written in C with a graphical user interface (GUI) using the IUP library.
## Method 1: Run batch file (roundrobin.bat)
- Run roundrobin.bat from the "Run" folder.
- Please ensure that and RoundRobin.exe and iup.dll are in the same folder as the batch file. This should already be the structure of the "Run" folder.
- The simulation output is saved  in two places: 1) analysis.txt and 2) a relevant timestamped file inside the “log” folder. The analysis.txt file will automatically open for review.
- Users are prompted in the terminal if they wish to start a new simulation. Press 'Y' to start a new simulation or 'N' to terminate the application.

## Method 2: Compilation Prerequisite: Add IUP Library
## 2.1. Using Code::Blocks
- In Code::Blocks, import the directory and open the RoundRobin.cbp file. 
- Build and run from within the IDE.
- Please ensure you have the iuplib folder, roundrobin.c and roundrobin.cbp file in the same folder (same structure as in the repository).
- The simulation output is printed to the terminal and saved in analysis.txt in the same folder.

## 2.2. Using command line (gcc)
- Please ensure that you are in the directory where roundrobin.c and iuplib folder are located.
- Please ensure the iuplib folder (containing the include directory with the iup.h file inside it) is in the same folder as roundrobin.c
- Windows Command Prompt:
```shell
// Compile each source file into an object file
gcc.exe -Wall -g -Wextra -Iiuplib\include -Iiuplib -c .\queue.c -o .\obj\queue.o
gcc.exe -Wall -g -Wextra -Iiuplib\include -Iiuplib -c .\robinui.c -o .\obj\robinui.o
gcc.exe -Wall -g -Wextra -Iiuplib\include -Iiuplib -c .\roundrobin.c -o .\obj\roundrobin.o

// Link the object files together to create the executable
gcc.exe -Liuplib -o .\run\roundrobin.exe .\obj\queue.o .\obj\robinui.o .\obj\roundrobin.o -lgdi32 -luser32 -lkernel32 -lcomctl32 .\iuplib\libiup.a
```
- Bash:
```bash
// Compile each source file into an object file
gcc -Wall -g -Wextra -Iiuplib/include -Iiuplib -c ./queue.c -o ./obj/queue.o
gcc -Wall -g -Wextra -Iiuplib/include -Iiuplib -c ./robinui.c -o ./obj/robinui.o
gcc -Wall -g -Wextra -Iiuplib/include -Iiuplib -c ./roundrobin.c -o ./obj/roundrobin.o

// Link the object files together to create the executable
gcc -Liuplib -o ./run/roundrobin.exe ./obj/queue.o ./obj/robinui.o ./obj/roundrobin.o -lgdi32 -luser32 -lkernel32 -lcomctl32 ./iuplib/libiup.a
```
- Run roundrobin.bat from the "Run" folder.