# MOS 6502 emulator

# In Development!
This will be a full fledged MOS-6502 emulator, with 6502 assembly interpreter.

as of currently the emulator has limited functionality and can only run programs hard coded into memory through the source code!

## Resources
Documentation: http://www.6502.org/users/obelisk/

Used Dave Poo's CPU emulator series first episode as a starting guide: https://www.youtube.com/watch?v=qJgsuQoy9bc&list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37


## Build 
### Windows

# Download executable from:
https://github.com/PetkoPetkov03/MOS-6502-EM/releases/


# DEPRECATED (MIGHT NOT WORK)
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
