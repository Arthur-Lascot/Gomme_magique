# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image ` -g
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

all: change_image/image_D

SRC = change_image/image_D.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

change_image/image_D: change_image/image_D.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
# 
