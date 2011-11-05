TARGET=bin.out
SDL_LIBS = `sdl-config --libs`
SDL_CFLAGS = `sdl-config --cflags`
ODE_LIBS = `ode-config --libs`
ODE_CFLAGS = `ode-config --cflags`
CC = g++ 

all:
	$(CC) $(SDL_CFLAGS) $(SDL_LIBS) -c base.cpp -o base.o $(ODE_LIBS) $(ODE_CFLAGS) 
	$(CC) $(SDL_CFLAGS)  -c main.cpp -o main.o
	$(CC) $(SDL_CFLAGS) $(SDL_LIBS) main.o base.o -o $(TARGET) -lGL -lSDL_image -lglut $(ODE_LIBS) $(ODE_CFLAGS) 
