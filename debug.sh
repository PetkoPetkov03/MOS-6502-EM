gdb "./build/linux/dist/MOS"

valgrind --leak-check=full -s "./build/MOS"
