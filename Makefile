CC = gcc
CFLAGS = -I include -Wall -Wextra -O2
LDFLAGS = -lSDL2 -lSDL2_gfx -lm -lSDL2_image
SRC = src/main.c src/camera.c src/planet.c src/event.c src/renderer.c src/ship.c src/ore.c src/config.c src/assets_gestion.c
OBJ = $(SRC:.c=.o)
EXEC = space_miner

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS);

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
