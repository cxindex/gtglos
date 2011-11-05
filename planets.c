#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#define SUN 1
#define MERCURY 2
#define VENUS 3
#define EARTH 4
#define MARS 5
#define BUFFER_LENGHT 64


void DrawQ(void)
{
	glBegin( GL_QUADS );
		glVertex3f( 0.f, 0.f, 0.0f );
		glVertex3f( 1, 0.f, 0.f );
		glVertex3f( 1, 2, 0.f );
		glVertex3f( 0.f, 2, 0.f );
	glEnd();
}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//~ glLoadIdentity(); //-
	glPushMatrix();
		//транслирует всю сцену в поле зрения
		glTranslatef(0.0f, 0.0f, 0.0f);
		//инициализируется стек имен
		glInitNames();
		glPushName(9);
		
		//называем и рисуем sun
		glColor3f(1.0f, 1.0f, 0.0f);
		glLoadName(SUN);
		DrawQ();
		
		//рисуем и называем merc
		glColor3f(1.0f, 0.0f, 0.0f);
		glPushMatrix();
		glTranslatef(1.0f, 0.0f, 0.0f);
		glLoadName(MERCURY);
		DrawQ();
		glPopMatrix();

		//рисуем и называем венеру
		glColor3f(1.0f, 0.0f, 1.0f);
		glPushMatrix();
		glTranslatef(2.0f, 0.0f, 0.0f);
		glLoadName(VENUS);
		DrawQ();
		glPopMatrix();

		//рисуем и назыаем землю
		glColor3f(0.0f, 0.0f, 1.0f);
		glPushMatrix();
		glTranslatef(4.0f, 0.0f, 0.0f);
		glLoadName(EARTH);
		DrawQ();
		glPopMatrix();

		//рисуем и называем марс
		glColor3f(1.0f, 0.5f, 0.0f);
		glPushMatrix();
		glTranslatef(5.3f, 0.0f, 0.0f);
		glLoadName(MARS);
		DrawQ();
		glPopMatrix();

		glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 10, 8);
	glOrtho(0, 10, 0, 8, 400.0, -400.0);
	
	glutSwapBuffers();
}

void ProcessSelection(int xPos, int yPos)
{
	GLfloat fAspect;
	//память для буфера выбора
	static GLuint selectBuff[BUFFER_LENGHT];
	//счетчик соответствий и память поля просмотра
	GLint hits, viewport[4];
	//настройка буфера выбора
	glSelectBuffer(BUFFER_LENGHT, selectBuff);
	//получаем поле просмотра
	glGetIntegerv(GL_VIEWPORT, viewport);
	//переключаемся на проектирование и записываем матрицу
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//меняем режим визуализации
	glRenderMode(GL_SELECT);
	//лук ат куб описанный вокруг положения положения курсора мыши
	//(xpos ypos) и простирающийся на два пикселя по верт и гориз
	glLoadIdentity();
	fprintf(stderr, "xPos: %i yPos: %i\n", xPos, viewport[3] - yPos);

	gluPickMatrix(xPos, viewport[3] - yPos, 2,2, viewport);
	//~ gluPickMatrix(xPos, yPos, 20, 20, viewport);
	//Применяем матрицу перспективной проекции.
	fAspect = (float)viewport[2] / (float)viewport[3];
	gluPerspective(45.0f, fAspect, 1.0, 425.0);
	//draw
	display();
	//собираем соответвтсвия
	hits = glRenderMode(GL_RENDER);
	//если произошло 1 хитс, отображаем инфо
	if(hits == 1);
		fprintf(stderr, "%i\n", hits);
	//восстанавливаем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	//ретурн ту наблюдаемая модель для нормальной визуализации
	glMatrixMode(GL_MODELVIEW);
}

void MouseCallback(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		ProcessSelection(x, y);
		//~ fprintf(stderr, "click!\n");
	}
}

void mouse_pas_mot(int x, int y)
{
	fprintf(stderr, "%i %i\n", x,y);
}


int main(int argc, char **argv)
{
	glutInitWindowSize(1000, 800);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);

	glutCreateWindow("planets");
	glutDisplayFunc(display);
	glutMouseFunc(MouseCallback);
	glutPassiveMotionFunc(mouse_pas_mot);
	glutMainLoop();
}
