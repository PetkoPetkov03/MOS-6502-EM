gdb "./build/dist/MOS"

valgrind --leak-check=full -s "./build/dist/MOS"
