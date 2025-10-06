#      Last edited by Levi S. and Benjamin B.     10/6/2025
 


CC = gcc
CFLAGS = -Wall
INCLUDES = -Iinclude

SRC = src/main.c src/parse.c src/builtins.c src/exec.c
OBJ = $(SRC:.c=.o)

all: wish

wish: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJ)


# path to wish.h
%.o: %.c include/wish.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


clean:
	rm -f $(OBJ) wish
