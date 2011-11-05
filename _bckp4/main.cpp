#include "main.h"
#include "base.h"
#define MAX_CONTACTS 3 //ode callback max cont
static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
static dJointID plane2d;
bool keystates[256];
base t1, t2;

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    
	// exit without doing anything if the two bodies are static
    if (b1 == 0 && b2 == 0) return;
    // exit without doing anything if the two bodies are connected by a joint
    if (b1 && b2 && dAreConnected (b1, b2)) return;
    
	int classg1 = dGeomGetClass (o1);
	int classg2 = dGeomGetClass (o2);
	dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
	for (int i=0; i<MAX_CONTACTS; i++) {
	    contact[i].surface.mode = dContactSoftERP;
	    contact[i].surface.mu = 1;
	    contact[i].surface.bounce = 0.0;
	    contact[i].surface.bounce_vel = 0.0;
	    contact[i].surface.soft_cfm = 0.001;	//jittering fix too
	    contact[i].surface.soft_erp = 0.1;	//jittering fix too
	  }
    if (int numc = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact))) 
    {
		for (int i=0; i<numc; i++) {
			dJointID c = dJointCreateContact (world,contactgroup,contact+i);
			dJointAttach (c,b1,b2);
		}
    }
}

static void simLoop (int pause)
{
    const dReal *R;
	static dReal angle = 0;
	angle += REAL( 0.01 );
	dBodyID linkbody;
    dVector3 res;
    dSpaceCollide (space,0,&nearCallback);      // обнаруживает столкновение и добавляет точку контакта
    dWorldQuickStep (world,0.10);               // шаг симуляции и его интервал
    dJointGroupEmpty (contactgroup);            // удаляет все точки контакта
}

void render_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	t1.square_render();
	t2.square_render();
	glutSwapBuffers();
}

//set visual state
void setup_rc(void)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
}

//subj
void change_size(GLsizei w, GLsizei h)
{
	GLfloat aspect_ratio;
	//prevent null division
	if(h==0) h=1;
	//set view-field with window-size
	glViewport(0, 0, w, h);

	//refresh coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//with capacity-planes-help set capacity cutting off
	aspect_ratio=(GLfloat)w/(GLfloat)h;
	glOrtho(0, w, 0, h, 100.0, -100.0); //static
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard_func(unsigned char key, int x, int y)
{
		keystates[(int)key]=1;
}

void keyboard_up_func(unsigned char key, int x, int y)
{
		keystates[(int)key]=0;
}

void timer_func(int value)
{
	t1.obj_keyboard_func();
	//~ t2.obj_keyboard_func2();
	glutPostRedisplay();
	//from render

	//^ from render
	simLoop(1);
	glutTimerFunc(10,timer_func,1);
}

//main enter-point
int main (int argc, char **argv)
{
	dInitODE();
    world = dWorldCreate ();
    space = dHashSpaceCreate (0);
	plane2d = dJointCreatePlane2D( world, 0);
	dWorldSetContactSurfaceLayer (world, 0.01); 
    dWorldSetGravity (world,0,-8,0);
    dCreatePlane (space,0,1,0,1);
    contactgroup = dJointGroupCreate (0);
    
	t1.body = dBodyCreate (world);
	dJointAttach( plane2d, t1.body, 0 );
    t1.geom = dCreateSphere (space,18); // / на большую сторону. дописать
    dMassSetSphere (&t1.m,1,0.5);
    
	t2.body = dBodyCreate (world);
	dJointAttach( plane2d, t2.body, 0 );
    t2.geom = dCreateSphere (space,18); // / на большую сторону. дописать
    dMassSetSphere (&t2.m,1,0.5);
	//~ dMassSetSphereTotal (&t1.m,0.5,18);

	t1.ode_init();
	t1.set(300,300);
	t2.ode_init();
	t2.set(100,300);
    
    glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE); //glut_rgba set as default
	glutCreateWindow("wut");
	
	glEnable( GL_TEXTURE_2D );
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode( GL_TEXTURE );
	glLoadIdentity();
	
	t1.img_load("sprite/gasmask/standl.bmp", 56, 37, 2, 0);
	t1.img_load("sprite/gasmask/standr.bmp", 56, 37, 2, 1);
	t1.img_load("sprite/gasmask/runl.bmp", 224, 37, 8, 2);
	t1.img_load("sprite/gasmask/runr.bmp", 224, 37, 8, 3);
	t1.img_load("sprite/gasmask/jumpl.bmp", 150, 24, 6, 4);
	t1.img_load("sprite/gasmask/jumpr.bmp", 150, 24, 6, 5);

	t1.speed=100;
	
	t2.img_load("sprite/gasmask/standl.bmp", 56, 37, 2, 0);
	t2.img_load("sprite/gasmask/standr.bmp", 56, 37, 2, 1);
	t2.img_load("sprite/gasmask/runl.bmp", 224, 37, 8, 2);
	t2.img_load("sprite/gasmask/runr.bmp", 224, 37, 8, 3);
	t2.img_load("sprite/gasmask/jumpl.bmp", 150, 24, 6, 4);
	t2.img_load("sprite/gasmask/jumpr.bmp", 150, 24, 6, 5);

	t2.speed=100;
	
	glutDisplayFunc(render_scene);
	glutReshapeFunc(change_size);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(keyboard_up_func);
	glutTimerFunc(30,timer_func,1);
	setup_rc();
	glutMainLoop();
}
