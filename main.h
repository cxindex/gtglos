#pragma once

#include <ode/ode.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <libconfig.h>

extern dSpaceID space;

extern int keystates[256];

extern void look_at(dBodyID body);
extern void get_euler(const dReal* matrix, dReal &kx, dReal &ky, dReal &kz);
extern dReal* set_eular(double deg);
extern void print_bitmap_string(void* font, char* s);


struct texture_struct{
	GLuint texture;
	int n, type; //needed number
	float w,h;
};
