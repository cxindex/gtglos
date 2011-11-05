#include "main.h"

class base{
public:
	GLuint texture; // mb make it as array
	GLfloat x,y,w,h, rot;
	void img_load( const char *str, int, int );
	void square_render (void);

};
//сюда же перенести и рендер куба, и выдачу текстуры, и обработку спиды
