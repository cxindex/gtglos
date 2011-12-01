#include "main.h"
enum {UP, DOWN, LEFT, RIGHT, UL, UR, UL1, UR1 };
class base{
public:
	const char *str;
	texture_struct texture[10];
	GLfloat x,y;
	float speed; //set. val >  == speed <
	float calc_speed; //for calc
	short last, up;	//last status. 0 standl ,1 standr, 2l,3r
	float rotate_angle;
	static int bitd;

	
	//ode
	dBodyID body;	
	dGeomID geom;	
	dMass m;
	bool rotatebit;
	
	//op func
	void img_load (int _w, int _h, const char *str, int _n, short t_n, short type);
	void active_square_render (void);	//ode+opengl
	void passive_square_render (void);	//ode+opengl
	
	//utils
	void active_control(void);
	void passive_control(void);

	//ode func
	void ode_init(void);
	void hack_2d(void);
	//~ void get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz);
	void rotate(void);
	void set_pos(int xx, int yy);
	void set_wh(int ww, int hh);
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
