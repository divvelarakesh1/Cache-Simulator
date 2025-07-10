# This is a comment line
CC=g++
# CFLAGS will be the options passed to the compiler.
CFLAGS=-c -Wall
OBJECTS = main.o implementation.o cache.o
all: riscv_sim 
riscv_sim: $(OBJECTS)
	 $(CC) $(OBJECTS) -o riscv_sim

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -rf *.o riscv_sim