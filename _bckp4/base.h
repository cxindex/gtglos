#include "main.h"

struct texture_struct{
	GLuint texture; // 0 stand ,1 left, 2r,3u,4d
	int n; //needed number
	float w,h;
};

class base{
public:
	texture_struct texture[6];
	GLfloat x,y,rot;
	float speed; //set. val >  == speed <
	float calc_speed; //for calc
	short last, up;	//last status. 0 standl ,1 standr, 2l,3r
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
	short check_state(void);

	//from render
	const dReal *odepos;
	dVector3 pointrelvel;
	float translated_val;
	short last_prev;

	//from check_state
	short stand, fly, Tup;
	bool Dup, c_tmp; //edit in func


};
//сюда же перенести и рендер куба, и выдачу текстуры, и обработку спиды
