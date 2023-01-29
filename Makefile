CC = gcc
CFLAGS = -Wall -Wextra
CPPFLAGS= `pkg-config --cflags sdl` -MMD
LDLIBS= `pkg-config --libs sdl SDL_image` -lm -lSDL_gfx
TEST_STP = test-stp
OBJ = $(patsubst %.c, %.o, $(wildcard src/*.c))
TEST_SRC = $(wildcard test/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)
HEADERS = $(wildcard src/*.h)

stp : $(TEST_STP)







$(TEST_STP): $(OBJ) $(TEST_OBJ)
	$(CC) $^ $(CFLAGS) -o $@


$(OBJ) $(TEST_OBJ): %.o: %.c $(HEADERS)

.PHONY: clean

clean : 
	$(RM) $(OBJ)
	$(RM) $(TEST_OBJ)
	$(RM) $(TEST_STP)
