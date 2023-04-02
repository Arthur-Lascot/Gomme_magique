CC = gcc
CFLAGS = -Wall -Wextra 
CPPFLAGS= `pkg-config --cflags sdl2 SDL2_image --cflags gtk+-3.0` 
LDLIBS= -lm `pkg-config --libs sdl2 SDL2_image --cflags gtk+-3.0`

EXE = main

OBJ = $(patsubst %.c, %.o, $(wildcard src/*/*.c))
MAIN_SRC = $(wildcard src/*.c)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
HEADERS = $(wildcard src/*/*.h)

interface: $(EXE)

$(EXE):	$(OBJ) $(MAIN_OBJ)
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -o $@


#truc qui fait les o
$(OBJ) $(MAIN_OBJ): %.o: %.c $(HEADERS)



.PHONY: clean

clean : 
	$(RM) $(OBJ)
	$(RM) $(MAIN_OBJ)
	$(RM) $(EXE)
	$(RM) src/*.o
	$(RM) src/*/*.o
	$(RM) */*.d
	$(RM) *.png
