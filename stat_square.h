#ifndef __SVSSTATBOX_H__
#define __SVSSTATBOX_H__

#include "SDL.h"
#include <ode/ode.h>
#include "svsbase.h"

extern int SCREEN_X;
extern int SCREEN_Y;

class statbox
{
	public:
	//~ svsInit SiBox; //повесим в функцию на мейк имг
	dGeomID box;
	svsBase base_box;
	SDL_Surface* svsBackreplacement;
	SDL_Surface* svsScreen;
	int svsAnimating;		//1-анимация есть
	///func
	int create(char *img,int NumFrames, int Pause,
			int r, int g, int b,SDL_Surface *screen);
	int ode_create(dSpaceID space, int xx, int yy);
	int set(SDL_Surface* screen, int xx, int yy);
	int rh(void) { return base_box.svsH;};
	int rw(void) { return base_box.svsW;};

};


#endif //__STATBOX_H__

#include "main.h"

class stat_square{
public:
	texture_struct texture[6];
	GLfloat x,y,rot;
	float speed; //set. val >  == speed <
	float calc_speed; //for calc
	
	//ode
	dBodyID body;	
	dGeomID geom;	
	dMass m;
	bool rotatebit;
	
	//op func
	void img_load (const char *str, int _w, int _h, int _n, short t_n);
	void square_render (void);	//ode+opengl
	
	//utils
	void active_control(void);
	void passive_control(void);

	//ode func
	void ode_init(void);
	void hack_2d(void);
	void get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz);
	void set(int xx, int yy);
	void move(int size, int max);
	short check_state(void);

	//for square_render
	const dReal *odepos;
	dVector3 pointrelvel;
	float translated_val;
	short last_prev;

	//for check_state
	short stand, fly, Tup;
	bool Dup, c_tmp; 
};
