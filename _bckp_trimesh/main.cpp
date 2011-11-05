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
  int i;
  // exit without doing anything if the two bodies are connected by a joint
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

  dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
  for (i=0; i<MAX_CONTACTS; i++)
  {
    //~ contact[i].surface.mode = dContactBounce | dContactSoftCFM;
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
    //~ const dReal *R;
	//~ static dReal angle = 0;
	//~ angle += REAL( 0.01 );
	//~ dBodyID linkbody;
    //~ dVector3 res;
    dSpaceCollide (space,0,&nearCallback);      // обнаруживает столкновение и добавляет точку контакта
    dWorldQuickStep (world,0.10);               // шаг симуляции и его интервал my
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
	t1.active_control();
	t2.passive_control();
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
	//~ dMassSetSphereTotal (&t1.m,0.5,18);
    //~ dJointSetPlane2DXParam (plane2d, dParamFMax, 10);
    //~ dJointSetPlane2DYParam (plane2d, dParamFMax, 10);

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
	
	#define VertexCount 5
	#define IndexCount 12

dVector3 Size;
float Vertices[VertexCount][3];
dTriIndex Indices[IndexCount];
//ode_z==op_y. остальное воркает. плюс там выдачу поправить
  Size[0] = 90.0f;
  Size[1] = 90.0f;
  Size[2] = 70.5f;
  //~ //work for z-up
  //
  Vertices[0][0] = -Size[0];	//-5
  Vertices[0][1] = Size[1];	//-5
  Vertices[0][2] = -Size[2];		//2.5
  
  Vertices[1][0] = Size[0];		//5
  Vertices[1][1] = Size[1];	//-5
  Vertices[1][2] = -Size[2];		//2.5
  
  Vertices[2][0] = Size[0];
  Vertices[2][1] = Size[1];
  Vertices[2][2] = Size[2];
  
  Vertices[3][0] = -Size[0];
  Vertices[3][1] = Size[1];
  Vertices[3][2] = Size[2];
  
  Vertices[4][0] = 0;
  Vertices[4][1] = 0;
  Vertices[4][2] = 0;

  
  Vertices[4][0] = 0;
  Vertices[4][1] = 0;
  Vertices[4][2] = 0;
  //just contact. соединение точек как ты понел ыде
  Indices[0] = 0;
  Indices[1] = 3;
  Indices[2] = 4;
  
  Indices[3] = 3;
  Indices[4] = 2;
  Indices[5] = 4;
  
  Indices[6] = 2;
  Indices[7] = 1;
  Indices[8] = 4;
  
  Indices[9] = 1;
  Indices[10] = 0;
  Indices[11] = 4;
  
  dTriIndex Indices2[IndexCount];

  Indices2[0] = 0;
  Indices2[1] = 1;
  Indices2[2] = 4;
  
  Indices2[3] = 1;
  Indices2[4] = 2;
  Indices2[5] = 4;
  
  Indices2[6] = 2;
  Indices2[7] = 3;
  Indices2[8] = 4;
  
  Indices2[9] = 3;
  Indices2[10] = 0;
  Indices2[11] = 4;
	
dTriMeshDataID data = dGeomTriMeshDataCreate();
dTriMeshDataID data2 = dGeomTriMeshDataCreate();
dGeomTriMeshDataBuildSingle(data, Vertices[0], 3 * sizeof(float), VertexCount, &Indices[0], IndexCount, 3 * sizeof(dTriIndex));
dGeomTriMeshDataBuildSingle(data2, Vertices[0], 3 * sizeof(float), VertexCount, &Indices2[0], IndexCount, 3 * sizeof(dTriIndex));
//~ dGeomTriMeshDataBuildSimple(data, (dReal*)Vertices, VertexCount, Indices2, IndexCount);
//~ dGeomTriMeshDataBuildSimple (data2, (dReal*)Vertices, VertexCount, Indices2, IndexCount);

	dGeomID TriMesh;
	dGeomID TriMesh2;
  TriMesh = dCreateTriMesh(space, data, 0, 0, 0);
  TriMesh2 = dCreateTriMesh(space, data2, 0, 0, 0);
  dGeomSetPosition(TriMesh, 100, 200, 0.0);
  dGeomSetPosition(TriMesh2, 100, 200, 0.0);
  dBodySetPosition(t1.body, 100, 400, 0.0);
  dBodySetPosition(t2.body, -1000, 10, 0.0);

	glutDisplayFunc(render_scene);
	glutReshapeFunc(change_size);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(keyboard_up_func);
	glutTimerFunc(30,timer_func,1);
	setup_rc();
	glutMainLoop();
}
