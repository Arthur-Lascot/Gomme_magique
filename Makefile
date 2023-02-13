CC = gcc
CFLAGS = -Wall -Wextra 
CPPFLAGS= `pkg-config --cflags sdl2 SDL2_image ` -MMD
LDLIBS= -lm `pkg-config --libs sdl2 SDL2_image`

TEST_STP = test-stp
TEST_CHANGE_IMAGE = change_image

OBJ = $(patsubst %.c, %.o, $(wildcard src/*.c))
OBJ_STP = $(patsubst %.c, %.o, $(wildcard src/STP/.c))
TEST_SRC = $(wildcard test/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)
TESTOBJ_CI = $($(patsubst %.c, %.o,wildcard src/change_image/*.c))
HEADERS = $(wildcard src/*/.h)

stp : $(TEST_STP)

change_image : $(TEST_CHANGE_IMAGE)




$(TEST_CHANGE_IMAGE) : $(TESTOBJ_CI)
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -o $@ 

$(TEST_STP) : $(OBJ_STP) $(TEST_OBJ)
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -o $@


$(OBJ_STP) $(TEST_OBJ) $(TESTOBJ_CI): %.o: %.c $(HEADERS)

.PHONY: clean

clean : 
	$(RM) $(OBJ_STP)
	$(RM) $(TEST_OBJ)
	$(RM) $(TEST_STP)
	$(RM) $(TEST_CHANGE_IMAGE)
	$(RM) $(TESTOBJ_CI)
	$(RM) */*.d
