#include "base.h"
void base::square_render (void)
{
	//~ if(keystates['d'])glBindTexture( GL_TEXTURE_2D, t1.texture );
	//~ else glBindTexture( GL_TEXTURE_2D, t2.texture );
	if(keystates['d']) x+=5;
	if(keystates['a']) x-=5;
	
	glBindTexture( GL_TEXTURE_2D, texture );
//~ 
	if(keystates['w']) y+=5;
	if(keystates['s']) y-=5;
	if(keystates['e']) rot+=50;
	if(keystates['q']) rot--;
	//~ if(keystates['c']) tr+=5;
	//~ if(keystates['z']) tr-=5;
	glPushMatrix();
	glTranslated(x, y, 0);
	glTranslated(w/2, h/2, 0); //make with variable
	glRotatef(rot,0,0,1);
	glTranslated(-w/2, -h/2, 0);

	glBegin( GL_QUADS );
		//Bottom-r vertex (corner)
		glTexCoord2i( 0, 0 );
		glVertex3f( w, 0.f, 0.0f );
		
		//Bottom-l vertex (corner)
		glTexCoord2i( 1,0 );
		glVertex3f( 0.f, 0.f, 0.f );
		
		//Top-l vertex (corner)
		glTexCoord2i( 1, 1 );
		glVertex3f( 0.f, h, 0.f );
		
		//Top-r vertex (corner)
		glTexCoord2i( 0,1 );
		glVertex3f( w, h, 0.f );
	glEnd();

	glPopMatrix();
	
}

void base::img_load (const char *str, int _w, int _h)
{
	SDL_Surface *surface;
	GLenum texture_format;
	GLint  n_of_colors;
	if ( (surface = IMG_Load(str)) )
	{
		n_of_colors = surface->format->BytesPerPixel;
		if (n_of_colors == 4)
		{     // contains an alpha channel
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
		} else if (n_of_colors == 3)
		{     // no alpha channel
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		} else 
		printf("warning: the image is not truecolor..  this will probably break\n");
			
		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &texture );
		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, texture );
		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, n_of_colors, surface->w, surface->h, 0,
		texture_format, GL_UNSIGNED_BYTE, surface->pixels );
		w=_w;
		h=_h;
	}
	else fprintf(stderr,"base::img_load: loading error %s\n", str);
}
