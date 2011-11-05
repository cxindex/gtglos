#include "main.h"
#include "base.h"
#define MAX_CONTACTS 3 //ode callback max cont
static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
static dJointID plane2d;
bool keystates[256];
float w,h;
base t1, t2;
base box1;
//~ stat_square box1;

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  int i;
  // exit without doing anything if the two bodies are connected by a joint
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

  dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
  for (i=0; i<MAX_CONTACTS; i++)
  {
    contact[i].surface.mode = dContactSoftCFM;
    contact[i].surface.mu = dInfinity;
    contact[i].surface.mu2 = 0;
    contact[i].surface.bounce = 0.1;
    contact[i].surface.bounce_vel = 0.1;
    contact[i].surface.soft_cfm = 0.01;
    contact[i].surface.soft_erp = 0.1;
  }
  if(int numc = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact)))
	{
    dMatrix3 RI;
    dRSetIdentity (RI);
    const dReal ss[3] = {0.02,0.02,0.02};
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

      dJointID c = dJointCreateContact (world,contactgroup,contact+i);
      dJointAttach (c,b1,b2);
    }
  }
}

static void simLoop (int pause)
{
    dSpaceCollide (space,0,&nearCallback);      // обнаруживает столкновение и добавляет точку контакта
    dWorldQuickStep (world,0.10);               // шаг симуляции и его интервал my
    dJointGroupEmpty (contactgroup);            // удаляет все точки контакта
}

void look_at(dBodyID body)
{
	static const dReal *odepos;
	static int x=0;
	static float l_w=w;
	int scroll=3;
	odepos=dBodyGetPosition(body);
	if((odepos[0]+100)>l_w)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		fprintf(stderr,"%.4f %.4f\n", odepos[0], w);
		glOrtho(x+=scroll, l_w+=scroll, 0, h, 100.0, -100.0); //static
			//~ glMatrixMode(GL_MODELVIEW);
	//~ glLoadIdentity();
	}
	if((odepos[0]-100)<l_w-w)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(x-=scroll, l_w-=scroll, 0, h, 100.0, -100.0); //static
	}
}

void render_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	t1.active_square_render();
	//~ t2.active_square_render();
	box1.passive_square_render();
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
	static int www=w+900;
	static int hhh=h+900;
	//set view-field with window-size
	//~ glViewport(0, 0, www, hhh);
	glViewport(0, 0, w, h);
	::w=w;
	::h=h;
	
	//refresh coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//with capacity-planes-help set capacity cutting off
	aspect_ratio=(GLfloat)w/(GLfloat)h;
	//~ glOrtho(0, www*10, 0, hhh*10, 100.0, -100.0); //static
	//~ glOrtho(0, w*10, 0, h, 100.0, -100.0); //static
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard_func(unsigned char key, int x, int y)
{
	keystates[(int)key]=1;
	if(key=='m')
	{
		//~ glViewport((0), 0, (bla), g_h);
		//~ glViewport(0, 0, (--bla)*2, g_h);
		//~ fprintf(stderr, "%i %i\n", take, bla);
	glMatrixMode(GL_PROJECTION);
		//~ glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	//~ glOrtho(-100, g_w*10, 0, g_h*10, 100.0, -100.0); //static
	glOrtho(0, w, 0, h, 100.0, -100.0); //static
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	}
		//~ glViewport(syka++, 0, bla++, g_h);
	//~ glOrtho((take--), g_w+take, 0, g_h, 100.0, -100.0); //static

}

void keyboard_up_func(unsigned char key, int x, int y)
{
	keystates[(int)key]=0;
}

void timer_func(int value)
{
	t1.active_control();
	t2.passive_control();
		look_at(t1.body);

	glutPostRedisplay();
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
    dWorldSetGravity (world,0,-8,0);
    dCreatePlane (space,0,1,0,0);
    contactgroup = dJointGroupCreate (0);
    
	t1.body = dBodyCreate (world);
	dJointAttach( plane2d, t1.body, 0 );
    t1.geom = dCreateSphere (space,18); // / на большую сторону. дописать
    dMassSetSphere (&t1.m,1,0.5);
    
	t2.body = dBodyCreate (world);
	dJointAttach( plane2d, t2.body, 0 );
    t2.geom = dCreateSphere (space,18); // / на большую сторону. дописать
    dMassSetSphere (&t2.m,1,0.5);
    
	t1.ode_init();
	t1.set(300,300);
	t2.ode_init();
	t2.set(100,400);
    
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

	box1.geom = dCreateBox (space, 900, 100, 10);
	//~ box1.img_load("sprite/gasmask/jumpl.bmp", 150, 24, 1, 0); //with this == tex. withou == no_tex
	dGeomSetPosition (box1.geom, 450, 100, 0);
	box1.texture[0].w=900;
	box1.texture[0].h=100;

	glutDisplayFunc(render_scene);
	glutReshapeFunc(change_size);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(keyboard_up_func);
	glutTimerFunc(30,timer_func,1);
	setup_rc();
	glutMainLoop();
}
