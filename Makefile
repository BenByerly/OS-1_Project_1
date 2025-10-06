CC = gcc
CFLAGS = -Wall
INCLUDES = -Iinclude

SRC = src/main.c src/parse.c
OBJ = $(SRC:.c=.o)

wish: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) wish
