CC = gcc
CFLAGS = -I../include -Wall -Wextra -O2
LDFLAGS = -lSDL2 -lSDL2_gfx -lm
SRC = main.c camera.c planet.c event.c renderer.c ship.c
OBJ = $(SRC:.c=.o)
EXEC = sdl_zoom

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
