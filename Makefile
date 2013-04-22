## Dave Nicholas <dave@davenicholas.me.uk>

## Compiler
CC=g++
## Linker
LD=$(CC)
## Flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags`
LFLAGS = `sdl-config --libs` -lGL -lGLU -lglut -lSDL_mixer

HEADERFILES = main.h

OBJS = main.o level.o sound.o text.o msa.o menu.o planet.o moon.o green_planet.o gas_planet.o spaceship.o error.o

SRCS =  main.cpp level.cpp sound.cpp text.cpp msa.cpp menu.cpp planet.cpp moon.cpp green_planet.cpp gas_planet.cpp spaceship.cpp  error.cpp

all : msa
	chmod g+rw *.o

clean :
	rm -rf msa *.o

msa : ${OBJS}
	$(LD) -o $@ ${OBJS} ${LFLAGS}

depend :
	makedepend ${SRCS}

