CC = /usr/bin/gcc-9

CFLAGS = -Wall -g -O2 -Werror -std=gnu99

EXECUTABLE = MyTinyOs

SRC_DIR = ./src

CODE = ./src/memory/instruction.c ./src/disk/code.c ./src/memory/dram.c ./src/cpu/mmu.c  ./src/main.c

.PHONY:MyTinyOs
MyTinyOs:
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(CODE) -o $(EXECUTABLE)

clean:
	rm -f *.o *~ $(EXECUTABLE)

run:
	./$(EXECUTABLE)
