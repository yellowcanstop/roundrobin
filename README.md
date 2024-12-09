# roundrobin
# Adding IUP library
**Using Codeblocks**
- Just fetch from repository and open the RoundRobin.cbp file, then open the roundrobin.c file to work on it
- Make sure you have the iuplib folder, roundrobin.c and roundrobin.cbp file in the same folder, like in the repository
**With GCC compile line**
- Make sure iuplib folder is present and in the same folder as roundrobin.c
- Wherever your GCC commandline is, it should be configured in a similar way to this (Change YOUR_PATH_HERE to your actual git folder path)
```
gcc.exe -Wall -g -Wextra -Wall -Iiuplib\include -Iiuplib -c C:\{YOUR_PATH_HERE}\git\roundrobin\roundrobin.c -o obj\Debug\roundrobin.o
gcc.exe -Liuplib -o bin\Debug\RoundRobin.exe obj\Debug\roundrobin.o   -lgdi32 -luser32 -lkernel32 -lcomctl32 iuplib\libiup.a
```
