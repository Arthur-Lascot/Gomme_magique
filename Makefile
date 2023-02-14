CC = gcc
CFLAGS = -Wall -Wextra 
CPPFLAGS= `pkg-config --cflags sdl2 SDL2_image ` 
LDLIBS= -lm `pkg-config --libs sdl2 SDL2_image`

TEST_STP = test-stp
TEST_CHANGE_IMAGE = image_D
TEST_GM = gomme

OBJ = $(patsubst %.c, %.o, $(wildcard src/*.c))
OBJ_STP = $(patsubst %.c, %.o, $(wildcard src/STP/*.c))
OBJ_GM = $(patsubst %.c, %.o, $(wildcard src/gomme_magique/*.c))
TEST_SRC = $(wildcard test/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)
#TESTOBJ_CI = $($(patsubst %.c, %.o,wildcard src/change_image/*.c))
HEADERS = $(wildcard src/*/*.h)

stp : $(TEST_STP)

$(TEST_STP): $(OBJ_STP) $(TEST_OBJ)
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -fsanitize=address -o $@


$(OBJ_GM) $(OBJ_STP) $(TEST_OBJ): %.o: %.c $(HEADERS)

gm: $(TEST_GM)

$(TEST_GM): $(OBJ_STP) $(OBJ_GM)
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -fsanitize=address -o $@


.PHONY: clean

clean : 
	$(RM) $(OBJ_STP)
	$(RM) $(TEST_OBJ)
	$(RM) $(TEST_STP)
	$(RM) $(TEST_GM)
	$(RM) src/*/*.o
	$(RM) */*.d




#change_image : 
#	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image ` -g   -c -o src/#change_image/image_D.o src/change_image/image_D.c
#	gcc   src/change_image/image_D.o  -lm `pkg-config --libs sdl2 SDL2_image` #-o ./image_D
#
#gomme_magique : src/gomme_magique/gomme_magique.c src/STP/selection_to_pixel.h
#	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image ` -g   -c -o src/#gomme_magique/gomme_magique.o src/gomme_magique/gomme_magique.c
#	gcc src/gomme_magique/gomme_magique.o  -lm `pkg-config --libs sdl2 #SDL2_image` -o ./gomme
#
#
#
##$(TEST_CHANGE_IMAGE) : $(TESTOBJ_CI)
##	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -o $@ 
#
#$(TEST_STP) : $(OBJ_STP) $(TEST_OBJ)
#	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -o $@
#
#
#$(OBJ_GM) $(OBJ_STP) $(TEST_OBJ): %.o: %.c $(HEADERS)
#
#.PHONY: clean
#
#clean : 
#	$(RM) $(OBJ_STP)
#	$(RM) $(TEST_OBJ)
#	$(RM) $(TEST_STP)
#	$(RM) $(TEST_CHANGE_IMAGE)
#	$(RM) $(TESTOBJ_CI)
#	$(RM) */*.d
