# MOS 6502 emulator

## Resources
Documentation: http://www.6502.org/users/obelisk/

Used Dave Poo's CPU emulator series first episode as a starting guide: https://www.youtube.com/watch?v=qJgsuQoy9bc&list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37


## Build 
### Windows
```
cmake -G "version of Visual Studio" .
```
examples of Visual Studio versions
Visual Studio 17 2022
Visual Studio 16 2019

build with visual studio


### Linux
```
cmake .
make

./dist/MOS
```

## Dependancies
### Debug Linix
Valgrind https://valgrind.org/
gdb https://man7.org/linux/man-pages/man1/gdb.1.html 
