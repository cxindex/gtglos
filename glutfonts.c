#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

typedef enum {
   MODE_BITMAP,
   MODE_STROKE
} mode_type;

//~ static mode_type mode;
//~ static int font_index;

void print_bitmap_string(void* font, char* s)
{
   if (s && strlen(s)) {
      while (*s) {
         glutBitmapCharacter(font, *s);
         s++;
      }
   }
}

void my_reshape(int w, int h)
{
   GLdouble size;
   GLdouble aspect;
	w=800; h=700;
   /* Use the whole window. */
   glViewport(0, 0, w, h);

   /* We are going to do some 2-D orthographic drawing. */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   size = (GLdouble)((w >= h) ? w : h) / 2.0;
   if (w <= h) {
      aspect = (GLdouble)h/(GLdouble)w;
   }
   else {
      aspect = (GLdouble)w/(GLdouble)h;
      glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
   }
	fprintf(stderr,"%i %i\n", w, h);
   /* Make the world and window coordinates coincide so that 1.0 in */
   /* model space equals one pixel in window space.                 */
   glScaled(aspect, aspect, 1.0);

   /* Now determine where to draw things. */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

}

void my_display(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
   
	glRasterPos2f(0, 0);
	glColor3f(1.0, 0.0, 0.0);

	print_bitmap_string(GLUT_BITMAP_9_BY_15, "asdfghjk");
	
   glutSwapBuffers();
}

int main(int argc, char **argv)
{
   glutInitWindowSize(600, 600);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
      glutCreateWindow("GLUT fonts");

   //---------------------------------
   //~ int w=1000; int h=700;
   //~ GLdouble size;
   //~ GLdouble aspect;
//~ 
   //~ /* Use the whole window. */
   //~ glViewport(0, 0, w, h);
//~ 
   //~ /* We are going to do some 2-D orthographic drawing. */
   //~ glMatrixMode(GL_PROJECTION);
   //~ glLoadIdentity();
   //~ size = (GLdouble)((w >= h) ? w : h) / 2.0;
   //~ if (w <= h) {
      //~ aspect = (GLdouble)h/(GLdouble)w;
   //~ }
   //~ else {
      //~ aspect = (GLdouble)w/(GLdouble)h;
      //~ glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
   //~ }

   /* Make the world and window coordinates coincide so that 1.0 in */
   /* model space equals one pixel in window space.                 */
   //~ glScaled(0.1, 0.1, 1.0);

   /* Now determine where to draw things. */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
//---------------------------------
      //~ glViewport(0, 0, 600, 600);
//~ 
   //~ glMatrixMode(GL_PROJECTION);
   //~ glLoadIdentity();
   //~ 
  //~ glOrtho(0, 600, 0, 600, -100000.0, 100000.0);
   //~ glScaled(1, 1, 1.0);
//~ 
   //~ glMatrixMode(GL_MODELVIEW);
   //~ glLoadIdentity();
   //~ 

   glutDisplayFunc(my_display);
   //~ glutReshapeFunc(my_reshape);
   //~ glutReshapeFunc(NULL);

   glutMainLoop();
}
