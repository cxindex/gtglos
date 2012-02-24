#include "main.h"
#include "base.h"
//wut
#define C_UNIT t1.body //control unit
#define MAX_CONTACTS 17 //ode callback max cont
extern void make_world_conf(const char *config_path, base *wall_array);

dWorldID world;
dSpaceID space;
static dJointGroupID contactgroup;
static dJointID plane2d;

int w_window=800;
int h_window=600;
int w_func_g, h_func_g, x_shear_g, y_shear_g;
int keystates[256];

base t1, t2;
base dynbox;
base dyn_array[256];
base stat_array[256];

struct distance_hit{
	base b;
	int state;
} shots_array[6];

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	int i;
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	
	int *gb;
	if (b1)	gb = (int*) dBodyGetData (b1);
	int *gb1;
	if (b2) gb1 = (int*) dBodyGetData (b2);

	dVector3 pr1;
	if(b1)dBodyGetRelPointVel(b1,0,0,0,pr1);
	dVector3 pr2;
	if(b2)dBodyGetRelPointVel(b2,0,0,0,pr2);
	
	dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
	for (i=0; i<MAX_CONTACTS; i++)
	{
		contact[i].surface.mode = dContactSoftCFM |  dContactSoftERP;
		contact[i].surface.mu = 5;
		contact[i].surface.soft_cfm = 0.02;
		contact[i].surface.soft_erp = 0.1;
	}
	
	if(int numc = dCollide (o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))
	{
		dGeomSetData (o1, (void*)'1');
		dGeomSetData (o2, (void*)'1');
		
	    for (i=0; i<numc; i++)
	    {
			if (dGeomGetClass(o1) == dRayClass || dGeomGetClass(o2) == dRayClass)
			{
				dMatrix3 Rotation;
				dRSetIdentity(Rotation);
				dVector3 End;
				
				End[0] = contact[i].geom.pos[0] + (contact[i].geom.normal[0] * contact[i].geom.depth);
				End[1] = contact[i].geom.pos[1] + (contact[i].geom.normal[1] * contact[i].geom.depth);
				End[2] = contact[i].geom.pos[2] + (contact[i].geom.normal[2] * contact[i].geom.depth);
				End[3] = contact[i].geom.pos[3] + (contact[i].geom.normal[3] * contact[i].geom.depth);
				continue;
			}

			dJointID c = dJointCreateContact (world, contactgroup, contact+i);
			dJointAttach (c, b1, b2);
			
	    }
		float depth = contact[0].geom.depth;
		if (depth != 10 && depth >= 1 && gb >= (int*)1000)
		{
			if (b1 && (pr1[0] > 80 || pr1[0] < -80 || pr1[1] > 80 || pr1[1] < -80)){
				dBodySetData(b1,(void*)((int*)gb - (int)depth));
				printf("GB: %i\n", (int*)gb);
			}
			if (b2 && (pr2[0] > 80 || pr2[0] < -80 || pr2[1] > 80 || pr2[1] < -80)){
				
				dBodySetData(b2,(void*)((int*)gb1 - (int)depth));
//				printf("GB1: %i\n", (int*)gb1);
			}
		}
	}
}

static void simLoop (int pause)
{
    dSpaceCollide (space, 0, &nearCallback);      // обнаруживает столкновение и добавляет точку контакта
    dWorldQuickStep (world, 0.10);               // шаг симуляции и его интервал
    dJointGroupEmpty (contactgroup);            // удаляет все точки контакта
}

void look_at(dBodyID body)
{
	static const dReal *odepos;
	static dVector3 pointrelvel;
	static int old[2];
	int scroll=0;
	static int x_shear=0;
	static int y_shear=0;

	odepos=dBodyGetPosition(body);
	dBodyGetRelPointVel(body,0,0,0,pointrelvel);
	static int w_func=old[0]=w_window;
	static int h_func=old[1]=h_window;
	
	//x
	if((odepos[0]+250)>w_func)
	{
		scroll=fabsf(pointrelvel[0]/10);
		if (scroll == 0) scroll = 4;

		if((odepos[0])>w_func)
			scroll=w_window-60;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(x_shear+=scroll, w_func+=scroll, y_shear, h_func, 100.0, -100.0);
	}	
	else if((odepos[0]-250)<w_func-w_window)
	{
		scroll=fabsf(pointrelvel[0]/10);
		if (scroll == 0) scroll = 4;
		
		if((odepos[0])<w_func-w_window)
			scroll=w_window-60;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(x_shear-=scroll, w_func-=scroll, y_shear, h_func, 100.0, -100.0);
	}

	//y
	if((odepos[1]+50)>h_func)
	{
		scroll=fabsf(pointrelvel[1]/10);
		if (scroll == 0) scroll = 4;
		
		 if((odepos[1])>h_func)
		 	scroll=h_window-60;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(x_shear, w_func, y_shear+=scroll, h_func+=scroll, 100.0, -100.0);
	}	
	else if((odepos[1]-50)<h_func-h_window)
	{
		scroll=fabsf(pointrelvel[1]/10);
		if (scroll == 0) scroll = 4;

		 if((odepos[1])<h_func-h_window)
		 	scroll=h_window-60; //bottom edge

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(x_shear, w_func, y_shear-=scroll, h_func-=scroll, 100.0, -100.0);
	}
	w_func_g=w_func;
	h_func_g=h_func;
	x_shear_g=x_shear;
	y_shear_g=y_shear;
}

void hit(const dReal* from, int x, int y)
{
	static int n, over;
	float f0, f1;
	
	if(n>=5) { n=0; over=1; }
	if(over==1){
		shots_array[n].state=0;
		//~ dBodyDisable(shots_array[n].b.body);
		//~ dGeomDisable(shots_array[n].b.geom);
		//~ free(shots_array[n].b);
		dBodyDestroy(shots_array[n].b.body);
		dGeomDestroy(shots_array[n].b.geom);
		//~ delete shots_array[n].b;
	}
	int i ;
	// if(!over)
	//~ else
	//~ shots_array[n].b = new base;
		
	shots_array[n].state=3;
	
	shots_array[n].b.body = dBodyCreate (world);
	dJointAttach( plane2d, shots_array[n].b.body, 0 );
	shots_array[n].b.geom = dCreateSphere (space,18); // / на большую сторону. дописать
//	dMassSetSphere (&shots_array[n].b.m,1,0.5);
	dMassSetSphere (&shots_array[n].b.m,1,0.4);
	shots_array[n].b.ode_init();
	
	shots_array[n].b.rotatebit=1;
	shots_array[n].b.ode_init();
	
	shots_array[n].b.img_load(36, 36, "sprite/sun.bmp", 1, 0, 0);
	
	if(x+::x_shear_g>=from[0])
	{
		f0=from[0]+40;
//	    dBodyAddForce(shots_array[n].b.body, 500, h_window - y, 0);
	    dBodyAddForce(shots_array[n].b.body, w_window - x, h_window - y, 0);
		printf("Y:%i\n", y);
	}	  
	else
	{
	    f0=from[0]-40;
	    dBodyAddForce(shots_array[n].b.body, -w_window - x, h_window - y, 0);
	}	    
	//~ fprintf(stderr, "%.3f %.3f\n", from[0], from[1]);
	//~ fprintf(stderr, "%i %i\n", x+::x_f, y);

	shots_array[n].b.set_pos(f0, from[1]);
	//~ shots_array[n].b.set_pos(x, y);

	//	dBodyAddForce(shots_array[n].b.body, from[0], h-y, 0);
	n++;
}

//--------------------------------- ODE END
void render_scene(void)
{
	static int frames=0, fps=0;
	static int lastUpdate = 0;
	static char fpsS[100];
	
	int currentTime = glutGet( GLUT_ELAPSED_TIME );
	frames++;
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	static int si=0;
	si=0;
	while(stat_array[si].geom){
		//~ printf("str is(main): %s\n",stat_array[si].str);
		stat_array[si++].passive_square_render();
	}

	static int i=0;
	i=0;
	while(dyn_array[i].body) dyn_array[i++].active_square_render();
	//---------------------------------//---------------------------------
//	dynbox.active_square_render();
	t2.active_square_render();
	t1.active_square_render();
	//приоритетные ниже
	
	static int i2=0;
	i2=0;
	while(i2!=5)
	{
		if(shots_array[i2].state==3)
			shots_array[i2++].b.active_square_render();
		else
			i2++;
	}
	glutSwapBuffers();
}

void setup_rc(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void keyboard_func(unsigned char key, int x, int y)
{
	if (keystates[(int)key] == 1) keystates[(int)key] = 2;
	if (keystates[(int)key] == 0) keystates[(int)key] = 1;
}

void keyboard_up_func(unsigned char key, int x, int y)
{
	keystates[(int)key]=0;
}

void mouse_action(int button, int state, int x, int y)
{
	static int to;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		hit(dBodyGetPosition(C_UNIT), x, y+y_shear_g);
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		dyn_array[to].body = dBodyCreate (world);
		dJointAttach (plane2d, dyn_array[to].body, 0);
		dyn_array[to].geom = dCreateBox (space, 60, 45, 10);
		dMassSetBoxTotal (&dyn_array[to].m,0.6,60,45,10); //work
		dyn_array[to].rotatebit=1;
		dyn_array[to].ode_init();

		dyn_array[to].img_load(60, 45, "block.bmp", 1, 0, 2);
		//why h_f_g? its top edge
		dyn_array[to].set_pos(x_shear_g+x, h_func_g-y);
		to++;
	}
}

void mouse_passive_motion(int x, int y)
{
	//~ fprintf(stderr, "X: %i Y: %i W_F: %i H_F: %i\n", x, y, w_f, h_f);
	//~ fprintf(stderr, "X: %i Y: %i\n", ::x_f+x, ::h-y);
	//~ fprintf(stderr, "X: %i Y: %i\n", t1.x+x, y);
}

void timer_func(int value)
{
	
	t1.active_control();
	t2.passive_control();
	look_at(C_UNIT);

	glutPostRedisplay();
	simLoop(1);
	glutTimerFunc(15,timer_func,1);
}

void reshape_window(GLsizei w, GLsizei h)
{
}
//--------------------------------- GL END

int main (int argc, char **argv)
{

	dInitODE();
    world = dWorldCreate ();
    space = dHashSpaceCreate (0);
	plane2d = dJointCreatePlane2D( world, 0);
    dWorldSetGravity (world,0,-8,0);
    dCreatePlane (space,0,1,0,0);
    contactgroup = dJointGroupCreate (0);

    glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE); //glut_rgba set as default
	glutInitWindowSize(w_window, h_window);
	glutCreateWindow("gtglos");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w_window, h_window);
	glOrtho(0, w_window, 0, h_window, 100.0, -100.0); //static
	
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//---------------------------------
	t1.body = dBodyCreate (world);
	dJointAttach( plane2d, t1.body, 0 );
	    t1.geom = dCreateSphere (space,18); // / на большую сторону. дописать
	    dMassSetSphereTotal (&t1.m, 0.4, 18);
	    t1.ode_init();
		
		t1.img_load(56, 39, "sprite/gasmask/standl.bmp", 2, 0, 1);
		t1.img_load(56, 39, "sprite/gasmask/standr.bmp", 2, 1, 1);
		t1.img_load(224, 37, "sprite/gasmask/runl.bmp", 8, 2, 1);
		t1.img_load(224, 37, "sprite/gasmask/runr.bmp", 8, 3, 1);
		t1.img_load(150, 24, "sprite/gasmask/jumpl.bmp", 6, 4, 1);
		t1.img_load(150, 24, "sprite/gasmask/jumpr.bmp", 6, 5, 1);
		t1.img_load(44, 34, "sprite/gasmask/ul1.bmp", 1, 6, 1);
		t1.img_load(44, 34, "sprite/gasmask/ur1.bmp", 1, 7, 1);
		t1.img_load(43, 37, "sprite/gasmask/dl.bmp", 1, 8, 1);
		t1.set_pos(-30,100);
		t1.speed=100;
		dBodySetData (t1.body, (void*)2000);
		//---------------------------------
		
		// dynbox.body = dBodyCreate (world);
		// dJointAttach ( plane2d, dynbox.body, 0 );
	    // dynbox.geom = dCreateBox (space, 60, 45, 10);
	    // dMassSetBoxTotal (&dynbox.m,0.9, 60, 45, 10); //work
	    // dynbox.rotatebit=1;
   	    // dynbox.ode_init();
   	    
		// dynbox.img_load(60, 45, "block.bmp", 1, 0, 2);
	    // dynbox.set_pos(-400,900);
		//---------------------------------
    
		t2.body = dBodyCreate (world);
		dJointAttach( plane2d, t2.body, 0 );
	    t2.geom = dCreateSphere (space,18); // / на большую сторону. дописать
	    dMassSetSphere (&t2.m,1,0.5);
		t2.ode_init();
	    
		t2.img_load(56, 39, "sprite/gasmask/standl.bmp", 2, 0, 1);
		t2.img_load(56, 39, "sprite/gasmask/standr.bmp", 2, 1, 1);
		t2.img_load(224, 37, "sprite/gasmask/runl.bmp", 8, 2, 1);
		t2.img_load(224, 37, "sprite/gasmask/runr.bmp", 8, 3, 1);
		t2.img_load(150, 24, "sprite/gasmask/jumpl.bmp", 6, 4, 1);
		t2.img_load(150, 24, "sprite/gasmask/jumpr.bmp", 6, 5, 1);
		t2.img_load(44, 34, "sprite/gasmask/ul1.bmp", 1, 6, 1);
		t2.img_load(44, 34, "sprite/gasmask/ur1.bmp", 1, 7, 1);
		t2.img_load(43, 37, "sprite/gasmask/dl.bmp", 1, 8, 1);
		t2.set_pos(200,900);
		t2.speed=100;
		dBodySetData (t2.body, (void*)2000);
		//---------------------------------
		make_world_conf("1.conf", stat_array);
//---------------------------------

	glutTimerFunc(1,timer_func,1);
	glutDisplayFunc(render_scene);
	glutReshapeFunc(reshape_window); //void reshape func doesn't make glortho
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(keyboard_up_func);
	glutMouseFunc(mouse_action);
	glutPassiveMotionFunc(mouse_passive_motion);

	setup_rc();
	glutMainLoop();
}

