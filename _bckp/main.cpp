#include "main.h"
#include "base.h"

//~ extern bool keystates[256];

GLuint texture, texture1;
SDL_Surface *surface;
GLenum texture_format;
GLint  nOfColors;

GLfloat x;
GLfloat rot;
GLfloat tr;
GLfloat y;
base t1, t2;

void render_scene(void){
	glClear(GL_COLOR_BUFFER_BIT);
	t1.square_render();
	//~ t1.x=x;
	//~ t1.y=y;
	//теперьы бы получше перенсти икс игр в класс
	glutSwapBuffers();
}

//set visual state
void setup_rc(void){
	glClearColor(0.0f,1.0f,0.0f,1.0f);
}

//subj
void change_size(GLsizei w, GLsizei h){
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

void keyboard_func(unsigned char key, int x, int y){
		keystates[(int)key]=1;
}

void keyboard_up_func(unsigned char key, int x, int y){
		keystates[(int)key]=0;
}

void timer_func(int value){
	//~ if(keystates['d']) printf("D\n");
	//~ else printf("A\n");
	//~ if(keystates['d']) x+=5;
	//~ if(keystates['a']) x-=5;
	//~ if(keystates['w']) y+=5;
	//~ if(keystates['s']) y-=5;
	//~ if(keystates['e']) rot+=50;
	//~ if(keystates['q']) rot--;
	//~ if(keystates['c']) tr+=5;
	//~ if(keystates['z']) tr-=5;
	glutPostRedisplay();
	glutTimerFunc(15,timer_func,1);
}

//main enter-point
int main (int argc, char **argv) {
    
    glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE); //glut_rgba set as default
	glutCreateWindow("GLrect");
	
	glEnable( GL_TEXTURE_2D );
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode( GL_TEXTURE );
	glLoadIdentity();
	
	t1.img_load("red_block.bmp", 65, 45);
	//~ t1.w=60; t1.h=45;
	//~ t2.img_load("block.bmp");
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutDisplayFunc(render_scene);
	glutReshapeFunc(change_size);
	glutTimerFunc(15,timer_func,1);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(keyboard_up_func);
	setup_rc();
	glutMainLoop();
}
/*Some general guidelines:

    Pair up your interfaces with implementations. If you have foo.cxx, everything defined in there had better be declared in foo.h.
    Ensure that every header file #includes all other necessary headers or forward-declarations necessary for independent compilation.
    Resist the temptation to create an "everything" header. They're always trouble down the road.
    Put a set of related (and interdependent) functionality into a single file. Java and other environments encourage one-class-per-file. With C++, you often want one set of classes per file. It depends on the structure of your code.
    Prefer forward declaration over #includes whenever possible. This allows you to break the cyclic header dependencies. Essentially, for cyclical dependencies across separate files, you want a file-dependency graph that looks something like this:
        A.cxx requires A.h and B.h
        B.cxx requires A.h and B.h
        A.h requires B.h
        B.h is independent (and forward-declares classes defined in A.h)

If your code is intended to be a library consumed by other developers, there are some additional steps that are important to take:

    If necessary, use the concept of "private headers". That is, header files that are required by several source files, but never required by the public interface. This could be a file with common inline functions, macros, or internal constants.
    Separate your public interface from your private implementation at the filesystem level. I tend to use include/ and src/ subdirectories in my C or C++ projects, where include/ has all of my public headers, and src/ has all of my sources. and private headers.

I'd recommend finding a copy of John Lakos' book Large-Scale C++ Software Design. It's a pretty hefty book, but if you just skim through some of his discussions on physical architecture, you'll learn a lot.
*/
