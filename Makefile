CC      = gcc
CFLAGS  = -I include -Wall -Wextra -O2
LDFLAGS = -lm -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf
SRC     = src/main.c            src/camera.c          src/event.c           src/renderer.c          \
          src/ore.c             src/assets_gestion.c  src/map.c             src/text.c              \
	      src/button.c          src/landing_page.c    src/window.c          src/ship_window.c       \
	      src/planet_window.c   src/tools.c           src/planet.c          src/ship.c 				\
		  src/solar_system.c
INCLUDE = include/camera.h      include/planet.h      include/event.h       include/renderer.h      \
          include/ship.h        include/ore.h         include/map.h         include/assets_gestion.h\
          include/text.h        include/button.h      include/config.h      include/landing_page.h  \
          include/window.h      include/tools.h		  include/solar_system.h
OBJ     = $(SRC:.c=.o)
EXEC    = space_miner

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

# Lancer le programme avec valgrind :
# 		gcc -g -Wall -Wextra -O2 -I include -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx src/*.c -o space_miner && valgrind ./space_miner
