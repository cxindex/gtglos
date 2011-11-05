#include "main.h"

struct texture_struct{
	GLuint texture; // 0 stand ,1 left, 2r,3u,4d
	int n; //needed number
	float w,h;
};

class base{
public:
	//~ GLuint texture[5]; // 0 standl ,1 standr, 2l,3r,4u,5d
	texture_struct texture[5];
	GLfloat x,y,rot;
	//~ int n; //needed number
	float speed; //set. val >  == speed <
	float calc_speed; //for calc
	short last;	//last status. 0 standl ,1 standr, 2l,3r,4u,5d
	//ode
	dBodyID body;	
	dGeomID geom;	
	dMass m;
	bool rotatebit;
	//func
	void img_load (const char *str, int _w, int _h, int _n, short t_n);
	void square_render (void);	//ode+opengl
	void obj_keyboard_func(void);
	//ode func
	void ode_init(void);
	void hack_2d(void);
	void get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz);
	void set(int xx, int yy);
	void move(int size, int max);

};
//сюда же перенести и рендер куба, и выдачу текстуры, и обработку спиды
