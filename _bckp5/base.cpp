#include "base.h"

void base::set(int _x, int _y)
{
	dBodySetPosition (body, _x, _y, 0);
	x=_x;
	y=_y;
}

void base::get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz)
{       
   const dReal epsilon=0.0000001;
   if(matrix[2] < 1-epsilon && matrix[2] > -1+epsilon)
   {
      ky=-asin(matrix[2]);
      dReal c=cos(ky);
      kx= atan2(matrix[6]/c,matrix[10]/c);
      kz= atan2(matrix[1]/c,matrix[0]/c);
   } else {       
      kz=0;   
      ky=-atan2(matrix[2],0);
      kx= atan2(-matrix[9],matrix[5]);
   }       
}

void base::hack_2d(void)
{
	const dReal     *rot = dBodyGetAngularVel (body);
	const dReal     *quat_ptr;
	dReal           quat[4],
					quat_len;

	quat_ptr = dBodyGetQuaternion (body);
	quat[0] = quat_ptr[0];
	quat[1] = 0;
	quat[2] = 0;
	quat[3] = quat_ptr[3];
	quat_len = sqrt (quat[0] * quat[0] + quat[3] * quat[3]);
	quat[0] /= quat_len;
	quat[3] /= quat_len;
	dBodySetQuaternion (body, quat);
	dBodySetAngularVel (body, 0, 0, rot[2]);

//Restricting Rotation To One Axis
//The plane2D stops objects rotating along non-2d axes,
//but it does not compensate for drift

    const dReal *rot1 = dBodyGetAngularVel( body );
    const dReal *quat_ptr1;
    dReal quat1[4], quat_len1;
    quat_ptr1 = dBodyGetQuaternion( body );
    quat1[0] = quat_ptr1[0];
    quat1[1] = 0;
    quat1[2] = 0; 
    quat1[3] = quat_ptr1[3]; 
    quat_len1 = sqrt( quat1[0] * quat1[0] + quat1[3] * quat1[3] );
    quat1[0] /= quat_len1;
    quat1[3] /= quat_len1;
    dBodySetQuaternion( body, quat1 );
    dBodySetAngularVel( body, 0, 0, rot1[2] );

    dMatrix3 R = { 0, 0, 0, 0, 0, 1};
    // 0 0 y
    if(!rotatebit){
		quat1[0] = 0;
		quat1[1] = 0;
		quat1[2] = 0; 
		quat1[3] = 1; 
	    dBodySetQuaternion( body, quat1 );
		dBodySetAngularVel( body, 0, 0, 0 );
	}

	static dVector3 pointrelvel;
	const dReal *odepos;
	const dReal *force;
	
	dBodyGetRelPointVel(body,0,0,0,pointrelvel); //берем скорость тела
	odepos=dBodyGetPosition(body); 
	force = dBodyGetForce (body);
		
	
	dBodySetForce(body,force[0],force[1],0.000); // мб проверять сразу и на пос
	if (odepos[2]>=0.001 || odepos[2]<=-0.001 ){
		dBodySetPosition (body,odepos[0],odepos[1], 0.000);
		dBodyAddRelForce (body,0,0, -(pointrelvel[2]));
		dBodySetTorque (body, 0.00, 0.00, 0.000);
	}
}	

void base::ode_init(void)
{
	dBodySetMass (body,&m);
    dGeomSetBody (geom,body);
}

void base::move(int size, int max)
{
    dBodyGetRelPointVel(body,0,0,0,pointrelvel);
    if (up) //in up
    {
		if (size > 0 && pointrelvel[0]<max) dBodyAddForce (body,size/4,0,0);
		else if (size < 0 && pointrelvel[0]>-max) dBodyAddForce(body,size/4,0,0);
	}
	
	else
	{
		if (size > 0 && pointrelvel[0] < max) dBodyAddForce (body, size, 0, 0);
		else if (size < 0 && pointrelvel[0] > -max) dBodyAddForce (body, size, 0, 0);
	}
}

//all keyboard processing
void base::active_control(void)
{
	if(keystates['d'])
	{
		move(15,60);
		last=3;
	}
	if(keystates['a'])
	{
		move(-15,60);
		last=2;
	}
	if(keystates['w'])
	{
		if (up==0) dBodyAddForce(body,0.0,100.0,0.0);	//if stand
		else if (up==2) dBodyAddForce(body,0.0,100.0,0);		//for dj
	}
	//---------------------------------
	float tmp;
	up=check_state();
	dBodyGetRelPointVel(body,0,0,0,pointrelvel); //get body's speed
	if(last>1 && !up) tmp=pointrelvel[0]/speed;
	else if(!up)tmp=0.03; //stand anim speed
	else tmp=0.3; //jump anim speed
	
	if(tmp>=0)calc_speed+=tmp;
	else calc_speed+=(tmp*-1);
	
	if(up)
	{
		if(last==0 || last==2)
			last=4;
		if(last==1 || last==3)
			last=5;
	}
	
	if(!keystates['a'] && !keystates['d'] && !up)	//idle
		dBodyAddForce(body,-pointrelvel[0]/1.5,0,0); //for circle only!
	
	if(pointrelvel[0]<=1 && pointrelvel[0]>=-1 && !up) //stand animation
		if(last!=0 && last!=1) last-=2;
	
	if(calc_speed>=1)
	{
		translated_val+=(double)1/texture[last].n;	//add tss reset to null
		calc_speed=0;
	}
	
	if(last_prev!=last) translated_val=0;
	last_prev=last;

	dReal rad;
	rad=dGeomSphereGetRadius (geom);
	if(keystates['t']) printf("UP:%.4f\n", rad);

	//~ if(keystates['e']) rot+=5;
	//~ if(keystates['q']) rot-=5;

}

void base::passive_control(void)
{
	//---------------------------------
	float tmp;
	up=check_state();
	dBodyGetRelPointVel(body,0,0,0,pointrelvel); //get body's speed
	if(last>1 && !up) tmp=pointrelvel[0]/speed;
	else if(!up)tmp=0.03; //stand anim speed
	else tmp=0.3; //jump anim speed
	
	if(tmp>=0)calc_speed+=tmp;
	else calc_speed+=(tmp*-1);
	
	if(up)
	{
		if(last==0 || last==2)
			last=4;
		if(last==1 || last==3)
			last=5;
	}
	
	if(!up)	//idle
		dBodyAddForce(body,-pointrelvel[0]/1.5,0,0); //for circle only!
	
	if(pointrelvel[0]<=1 && pointrelvel[0]>=-1 && !up) //stand animation
		if(last!=0 && last!=1) last-=2;
	
	if(calc_speed>=1)
	{
		translated_val+=(double)1/texture[last].n;	//add tss reset to null
		calc_speed=0;
	}
	
	if(last_prev!=last) translated_val=0;
	last_prev=last;
	
	//~ if(keystates['e']) rot+=5;
	//~ if(keystates['q']) rot-=5;
}

void base::square_render (void)
{
	odepos=dBodyGetPosition(body);
	x=odepos[0]-texture[last].w/2; //is it needed?
	y=odepos[1]-texture[last].h/2; //here maybe some troubles with up
	
	glTranslated(translated_val, 0, 0); //for all except stand anim
	glScalef((float)1/texture[last].n, 1.0f, 1.0f); //1==fullsize. -(w+1/n)==one frame size + -- invers	
	/*invers---------------------------
	glTranslated(-translated_val, 0, 0);
	glScalef(-(float)1/texture[last].n, 1.0f, 1.0f);
	invers----------------------------- */
	
	glBindTexture( GL_TEXTURE_2D, texture[last].texture );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslated(x, y, 0);	//set position
	glTranslated(texture[last].w/2, texture[last].h/2, 0); //al three for rotate
	glRotatef(rot,0,0,1);
	glTranslated(-texture[last].w/2, -texture[last].h/2, 0);

	glBegin( GL_QUADS );
		//Bottom-r vertex (corner)
		glTexCoord2i( 0, 1 );
		glVertex3f( 0.f, 0.f, 0.0f );
		
		//Bottom-l vertex (corner)
		glTexCoord2i( 1,1 );
		glVertex3f( texture[last].w, 0.f, 0.f );
		
		//Top-l vertex (corner)
		glTexCoord2i( 1, 0 );
		glVertex3f( texture[last].w, texture[last].h, 0.f );
		
		//Top-r vertex (corner)
		glTexCoord2i( 0,0 );
		glVertex3f( 0.f, texture[last].h, 0.f );
	glEnd();
	
	glPopMatrix();
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

}

short base::check_state(void)
{
	dBodyGetRelPointVel (body, 0, 0, 0, pointrelvel);
	if (pointrelvel[1] > -1 && pointrelvel[1] < 1)
	{
		stand++;	
		if (stand>=1 && c_tmp==0) Dup=1,c_tmp=1;		//включаем при >=1.  1 - можно делать дж
		if (stand>=4) Tup=0, Dup=0, c_tmp=0;		//выключаем дж на 3емле //stand>=X --X time for set stand?
		if (Dup==1) Tup=2, Dup=0;
	}
	else stand=0;

	dBodyGetRelPointVel(body,0,0,0,pointrelvel);
	if (pointrelvel[1]>1 || pointrelvel[1]<-1)
	{
		fly++;
		if (fly>=1) Tup=1, Dup=0;
	}
	else fly=0;
	
	return Tup;	//1 - действует первый джамп. 2- можно дж. 0 - на земле
}

void base::img_load (const char *str, int _w, int _h, int _n, short t_n) 
{//t_n - texture's number 
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
		printf("base:::img_load::warning: the image is not truecolor. this will probably break\n");
			
		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &texture[t_n].texture );
		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, texture[t_n].texture );
		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, n_of_colors, surface->w, surface->h, 0,
		texture_format, GL_UNSIGNED_BYTE, surface->pixels );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		texture[t_n].w=_w/_n;
		texture[t_n].h=_h;
		texture[t_n].n=_n;
	}
	else fprintf(stderr,"base:::img_load::error: loading error %s\n", str);
}
