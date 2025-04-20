CC      = gcc
CFLAGS  = -I include -Wall -Wextra -O2
LDFLAGS = -lm -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf
SRC     = src/main.c            src/camera.c          src/event.c           src/renderer.c          \
          src/ore.c             src/assets_gestion.c  src/map.c             src/text.c              \
	      src/solar_system.c    src/landing_page.c    src/window.c          src/ship_window.c       \
	      src/planet_window.c   src/tools.c           src/planet.c          src/ship.c 				\
		  
INCLUDE = include/camera.h      include/planet.h      include/event.h       include/renderer.h      \
          include/ship.h        include/ore.h         include/map.h         include/assets_gestion.h\
          include/text.h        include/config.h      include/tools.h       include/landing_page.h  \
          include/window.h	    include/solar_system.h
OBJ     = $(SRC:.c=.o)
EXEC    = space_miner

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

# Lancer le programme :
#		- de la meme maniere qu'avec le Makefile :
# 				gcc src/*.c -Wall -Wextra -O2 -I include -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -o space_miner && ./space_miner
#		- avec valgrind (pour trouver ou ont lieu les fuits de memoires):
# 				gcc -g src/*.c -Wall -Wextra -O2 -I include -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -o space_miner && valgrind ./space_miner
#		- en mode paranoiaque :
# 				gcc -Wall -Wextra -O -Wwrite-strings -Wstrict-prototypes -Wuninitialized -Wunreachable-code -Wno-missing-braces  -Wno-missing-field-initializers -O2 -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int -Werror-implicit-function-declaration -Wmain -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings -Wconversion -Wsign-compare -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long -Wunreachable-code src/*.c -I include -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -o space_miner && ./space_miner