# COMP2035 Operating Systems & Concurrency
# Round Robin Process Scheduler
# Group 10
The program is a simulator of a Round-Robin process scheduler written in C with a graphical user interface (GUI) using the IUP library.
## Run executable (.exe)
- Please ensure that iup.dll is in the same folder as the .exe.
- Run the executable.

## Compilation Prerequisite: Add IUP Library
## 1. Using Code::Blocks
- In Code::Blocks, open the RoundRobin.cbp file.Compile and run from within the IDE.
- Please ensure you have the iuplib folder, roundrobin.c and roundrobin.cbp file in the same folder (same structure as in the repository).

## 2. Using command line (gcc)
- Please ensure the iuplib folder is present and in the same folder as roundrobin.c
- Wherever your GCC commandline is, it should be configured in a similar way to this (Change YOUR_PATH_HERE to your actual git folder path)
```
gcc.exe -Wall -g -Wextra -Wall -Iiuplib\include -Iiuplib -c C:\{YOUR_PATH_HERE}\git\roundrobin\roundrobin.c -o obj\Debug\roundrobin.o
gcc.exe -Liuplib -o bin\Debug\RoundRobin.exe obj\Debug\roundrobin.o   -lgdi32 -luser32 -lkernel32 -lcomctl32 iuplib\libiup.a
```


