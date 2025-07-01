CC      = gcc
CFLAGS  = -I inc -Wall -Wextra -O2
LDFLAGS = -lm -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf -lSDL2_mixer
OBJDIR  = obj
OBJ     = $(SRC:src/%.c=$(OBJDIR)/%.o)
EXEC    = void_reign
SRC     = src/main.c            src/camera.c          src/event.c           src/renderer.c         \
          src/ore.c             src/assets_gestion.c  src/map.c             src/text.c             \
          src/solar_system.c    src/landing_page.c    src/window.c          src/ship_window.c      \
          src/planet_window.c   src/tools.c           src/planet.c          src/ship.c 			   \
		  src/place.c           src/build.c           src/info_view.c       src/basic_ship_window.c\
		  src/asteroid.c        src/side_bar_window.c src/enemy.c           src/tile.c             \
		  src/pause_window.c
	      
INCLUDE = inc/camera.h          inc/planet.h          inc/event.h           inc/renderer.h         \
          inc/ship.h            inc/ore.h             inc/map.h             inc/assets_gestion.h   \
          inc/text.h            inc/config.h          inc/tools.h           inc/landing_page.h     \
          inc/window.h	        inc/solar_system.h    inc/place.h           inc/basic_ship_window.h\
		  inc/build.h           inc/mine.h            inc/info_view.h       inc/asteroid.h         \
		  inc/side_bar_window.h inc/enemy.h           inc/tile.h 			inc/pause_window.h

# Dependances automatiques
DEPS = $(OBJ:$(OBJDIR)/%.o=$(OBJDIR)/%.d)

all: $(EXEC)

$(EXEC): $(OBJ)
	@echo "Done\n"
	@echo "======================================================================================"
	@echo "Linking object files to create the executable '$(EXEC)'\n..."
	@$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Done\n"
	@echo "======================================================================================"
	@echo "Run './$(EXEC)' to start the program"
	@echo "======================================================================================"
	@echo "\n"

# Creer le dossier des objets si necessaire
$(OBJDIR):
	@echo "======================================================================================"
	@echo "Creating directory '$(OBJDIR)'\n..."
	@mkdir -p $(OBJDIR)
	@echo "Done\n"

	@echo "======================================================================================"
	@echo "Compilation of source files\n..."

# Compilation des fichiers .c en .o, avec generation de dependances .d
$(OBJDIR)/%.o: src/%.c $(INCLUDE) | $(OBJDIR)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Inclure les fichiers de dependances generes pour chaque fichier .o
-include $(DEPS)

clean:
	@echo "======================================================================================"
	@echo "Cleaning up object files\n..."
	@rm -rf $(OBJDIR)
	@echo "Done\n"


# Lancer le programme :
#		- de la meme maniere qu'avec le Makefile :
# 				gcc src/*.c -Wall -Wextra -O2 -I inc -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -o void_reign && ./void_reign
# 		- idem Makefile mais sans les unused parameter/variable:
#				gcc src/*.c -Wall -Wextra -O2 -Wno-unused-parameter -Wno-unused-variable -I inc -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -o void_reign && ./void_reign
#		- avec valgrind (pour trouver ou ont lieu les fuits de memoires):
# 				gcc -g src/*.c -Wall -Wextra -O2 -I inc -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -o void_reign && valgrind ./void_reign
#		- en mode paranoiaque :
# 				gcc -Wall -Wextra -O -Wwrite-strings -Wstrict-prototypes -Wuninitialized -Wunreachable-code -Wno-missing-braces  -Wno-missing-field-initializers -O2 -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int -Werror-implicit-function-declaration -Wmain -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings -Wconversion -Wsign-compare -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long -Wunreachable-code src/*.c -I inc -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -o void_reign && ./void_reign
#