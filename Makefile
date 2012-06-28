TARGET=bin.out
sTARGET=stat-bin.out
TARGETW=bin.exe
SDL_LIBS = `sdl-config --libs`
SDL_CFLAGS = `sdl-config --cflags`
#ODE_LIBS = `ode-config --libs`
#ODE_LIBS = ./libode.so
ODE_LIBS = 
ODE_CFLAGS = `ode-config --cflags`
CC = g++ -g -Wl,-R./ ./libode.so #example of link to shared-local
#CC = g++ -g

#~ sSDL_LIBS = /usr/lib/libSDL.a /usr/lib/libSDL_image.a  /usr/lib/libSDLmain.a  -ldl -lm -lpthread
#~ sSDL_LIBS = /usr/lib/libSDL.a
sSDL_LIBS = `sdl-config --libs`
sSDL_CFLAGS = `sdl-config --cflags`
sODE_LIBS = /usr/lib/libode.a
sODE_CFLAGS = `ode-config --cflags`
sCC = g++

CCW = i486-mingw32-g++
WW = -I/usr/i486-mingw32/include/

all:
	$(CC) $(SDL_CFLAGS)  -c utils.cpp -o utils.o $(ODE_LIBS) $(ODE_CFLAGS) 
	$(CC) $(SDL_CFLAGS)  -c main.cpp -o main.o
	$(CC) $(SDL_CFLAGS) $(SDL_LIBS) -c base.cpp -o base.o $(ODE_LIBS) $(ODE_CFLAGS) 
	$(CC) $(SDL_CFLAGS) $(SDL_LIBS) main.o base.o utils.o -o $(TARGET) -lGL -lSDL_image -lglut -lconfig $(ODE_LIBS) $(ODE_CFLAGS) 

	#~ $(CC) $(SDL_CFLAGS) $(SDL_LIBS) main.o base.o -o $(TARGET) -lGL -lSDL_image -lglut $(ODE_LIBS) $(ODE_CFLAGS) 
	rm ./*.o

stat:
	$(sCC) $(sSDL_CFLAGS)  -c utils.cpp -o utils.o $(sODE_LIBS) $(sODE_CFLAGS) 
	$(sCC) $(sSDL_CFLAGS)  -c main.cpp -o main.o
	$(sCC) $(sSDL_CFLAGS) $(sSDL_LIBS) -c base.cpp -o base.o $(sODE_LIBS) $(sODE_CFLAGS) 
	$(sCC) $(sSDL_CFLAGS) $(sSDL_LIBS)  main.o base.o utils.o -o $(sTARGET) /usr/lib/libSDL_image.a /usr/lib/libGL.so /usr/lib/libglut.a $(sODE_LIBS) $(sODE_CFLAGS) 

	#~ $(CC) $(SDL_CFLAGS) $(SDL_LIBS) main.o base.o -o $(TARGET) -lGL -lSDL_image -lglut $(ODE_LIBS) $(ODE_CFLAGS) 
	rm ./*.o

w32:
	$(CCW) $(SDL_CFLAGS) $(SDL_LIBS) -c base.cpp -o base.o $(ODE_LIBS) $(ODE_CFLAGS)
	$(CCW) $(SDL_CFLAGS)  -c main.cpp -o main.o $(ODE_CFLAGS)
	#~ $(CCW) $(SDL_CFLAGS) $(SDL_LIBS) main.o base.o -o $(TARGETW) -lGL -lSDL_image -lglut $(ODE_LIBS) $(ODE_CFLAGS) 
	$(CCW) $(SDL_CFLAGS) $(SDL_LIBS) main.o base.o -o $(TARGETW) -L/usr/lib/ -lGL.so -lSDL_image -lglut -L/usr/lib/libode.so $(ODE_CFLAGS) 
	rm ./*.o
 #~ i486-mingw32-gcc -o heat.exe heat.c matrix.c initial.c solver.c plotter.c args.c solution.c -lm 

