CC = gcc
CFLAGS = -I include -Wall -Wextra -O2
LDFLAGS = -lm -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf
SRC = src/main.c src/camera.c src/planet.c src/event.c src/renderer.c src/ship.c src/ore.c src/assets_gestion.c src/map.c src/text.c src/button.c
OBJ = $(SRC:.c=.o)
EXEC = space_miner

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS);

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
