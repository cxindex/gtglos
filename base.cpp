#include "base.h"

void base::set_pos(int _x, int _y)
{
	dBodySetPosition (body, _x, _y, 0);
	x=_x;
	y=_y;
}

void base::rotate()
{
	dReal kx, ky, kz;
	
	if(body)
		get_euler(dBodyGetRotation(body), kx, ky, kz);
	else
		get_euler(dGeomGetRotation(geom), kx, ky, kz);

	rotate_angle = -kz*57; //from rad to deg
	
	if((int)rotate_angle == -179)
		rotate_angle=179;
}

void base::hack_2d(void)
{
	const dReal *rot = dBodyGetAngularVel (body);
	const dReal *quat_ptr;
	dReal quat[4], quat_len;

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
    if(!rotatebit) //if there is no set rotatebit then don't make rotate in ode
    {
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
	
	dBodyGetRelPointVel(body,0,0,0,pointrelvel);
	odepos=dBodyGetPosition(body); 
	force = dBodyGetForce (body);
		
	
	dBodySetForce(body,force[0],force[1],0.000);
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
	//for wall-up i must be a sphere with total-mass, torq in air and in stand, and with force in air
    if(body) dBodyGetRelPointVel(body,0,0,0,pointrelvel);
//	const dReal *tr = dBodyGetTorque(body);

    if (up){ //in up
		if (size > 0 && pointrelvel[0]<max) dBodyAddForce (body,size/10,0,0);
//		if (size > 0 && pointrelvel[0]<max) dBodyAddTorque (body,size/4, 0, 0);
		else if (size < 0 && pointrelvel[0]>-max) dBodyAddForce (body,size/10,0,0);
//		else if (size < 0 && pointrelvel[0]>-max) dBodyAddTorque (body,size,0,0);
	}
	
	else{
	if (size > 0 && pointrelvel[0] < max) {
		dBodyAddForce (body,size,0,0);
//		dBodyAddRelTorque (body, 0, 0, -size*60);
	}		
	else if (size < 0 && pointrelvel[0] > -max) {
		dBodyAddForce (body,size,0,0);
//			dBodyAddTorque (body, 0, 0, -size*60);
	}

    //~ if (up) //in up
    //~ {
		//~ if (size > 0 && pointrelvel[0]<max) dBodyAddForce (body,size/4,0,0);
		//~ else if (size < 0 && pointrelvel[0]>-max) dBodyAddForce (body,size/4,0,0);
	//~ }
	//~ 
	//~ else
	//~ {
		//~ if (size > 0 && pointrelvel[0] < max) dBodyAddForce (body, size,0,0);
		//~ else if (size < 0 && pointrelvel[0] > -max) dBodyAddForce (body,size,0,0);
//~ 
	}
}

//all keyboard processing
void base::active_control(void)
{
	const dReal *odepos;
	static int wj;
	static int dj;
if(bitd!=2){		
	if(keystates['d']){
		move(20, 80);
		last=RIGHT;
		if(keystates['c'])
			dBodyAddRelForce(body,-100,0,0);
	}
	
	if(keystates['a']){
		move(-20, 80);
		last=LEFT;
		if(keystates['z'])
			dBodyAddRelForce(body,100,0,0);
	}
	
	if(keystates[' ']){
		up=check_state();
		if (up==0){
			dBodyAddForce(body,0.0,40.0,0.0);	//if stand
			wj=0;		//can wj
			dj=0;		//can dj
		}
		
		if ((last==UL1 || last==UR1) && !wj){
			if(last==UL1) dBodyAddForce(body,10.0,200.0,0.0);		//from wall
			else if(last==UR1) dBodyAddForce(body,-10.0,200.0,0.0);		//from wall
			printf("SSSS\n");
//			wj=1;
		}
		else if (up==2 && !dj){
//			dBodyAddForce(body,0.0,180.0,0.0);		//for dj
			dj=1;
		}
	}
	//---------------------------------


	float tmp;

	up=check_state();
	dBodyGetRelPointVel(body,0,0,0,pointrelvel); //get body's speed
	if(last>1 && !up) tmp=pointrelvel[0]/speed;
	else if(!up)tmp=0.03; //stand anim speed
	else tmp=0.4; //jump anim speed
	
	if(tmp>=0)calc_speed+=tmp;
	else calc_speed+=(tmp*-1);
	
	if(up){
		char *pd = (char*) dGeomGetData (geom);					
		if(last==UP || last==LEFT || last==UL1)
			if(pd == (char *)'1' && keystates['a'] && pointrelvel[0] > -1 && pointrelvel[0] < 1){
				last=UL1;
			}
			else last=UL;
			
		if(last==DOWN || last==RIGHT || last== UR1)
 			if(pd == (char *)'1' && keystates['d'] && pointrelvel[0] > -1 && pointrelvel[0] < 1){
				last=UR1;
			}
			else last=UR;
	}
	
	if(!keystates['a'] && !keystates['d'] && !up)	//idle
	  //		dBodyAddForce(body,-pointrelvel[0]/1.5,0,0); //for active_control _only_!
	;
	if(pointrelvel[0]<=1 && pointrelvel[0]>=-1 && !up) //stand animation
		if(last!=0 && last!=1) last-=2;
	
	if(calc_speed>=1){
		translated_val+=(double)1/texture[last].n;
		calc_speed=0;
	}
	
	if(last_prev!=last) translated_val=0;
	last_prev=last;

	dGeomSetData (geom, (void*)'0');
}
}

void base::passive_control(void)
{
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
		dBodyAddForce(body,-pointrelvel[0]/1.5,0,0); //for circle only

	if(pointrelvel[0]<=1 && pointrelvel[0]>=-1 && !up) //stand animation
		if(last!=0 && last!=1) last-=2;
	
	if(calc_speed>=1)
	{
		translated_val+=(double)1/texture[last].n;
		calc_speed=0;
	}
	
	if(last_prev!=last) translated_val=0;
	last_prev=last;
	//~ hack_2d();
	//~ if(keystates['e']) rot+=5;
	//~ if(keystates['q']) rot-=5;
}

void base::active_square_render (void)
{ //with frame-animation, but without set-type img
	//0 alive, 1 - dead, nocontrol, 2 -  delete
	int *gb;
	if (!bitd){
		gb = (int*) dBodyGetData (body);
//		printf("GET %i\n", (int*)gb);
		if (gb <= (int*) 1900 && gb != (int*) 0 ){
			printf("DEAD\n");
			if(!bitd) bitd=1;
		}
	} else last=DL, translated_val=0;

	if(bitd==1 && !up){
		dBodyDestroy(body);
		dGeomDestroy(geom);
		printf("DESTROY\n");
		bitd=2;
	}
	
	if(body) odepos=dBodyGetPosition(body);
	else if(geom) odepos=dGeomGetPosition(geom);

	x=odepos[0]-texture[last].w/2; //is it needed?
	y=odepos[1]-texture[last].h/2; //here maybe some troubles with up
	

	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	
	glTranslated(translated_val, 0, 0); //for all except stand anim
	glScalef((float)1/texture[last].n, 1.0f, 1.0f); //1==fullsize. -(w+1/n)==one frame size + -- invers	
	/*invers---------------------------
	glTranslated(-translated_val, 0, 0);
	glScalef(-(float)1/texture[last].n, 1.0f, 1.0f);
	invers----------------------------- */
	
	glBindTexture( GL_TEXTURE_2D, texture[last].texture );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(rotatebit)
		rotate();
		
	glTranslated(x, y, 0);	//set position
	glTranslated(texture[last].w/2, texture[last].h/2, 0); //all three for rotate
	glRotatef(rotate_angle,0,0,1);
	glTranslated(-texture[last].w/2, -texture[last].h/2, 0);

	glBegin( GL_QUADS );
	    glColor3f(1.0f,1.0f,1.0f);
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
	if(body && bitd!=2)hack_2d();
}

void base::passive_square_render (void)
{ //without any animation, but with set-type img
	if(body) odepos=dBodyGetPosition(body);
	else odepos=dGeomGetPosition(geom);
	x=odepos[0]-texture[0].w/2; //is it needed?
	y=odepos[1]-texture[0].h/2; //here maybe some troubles with up
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	switch(texture[last].type)
	{
		case 0:
			glScalef((float)1*texture[last].n, (float)1*texture[last].n, 1.0f); //1==fullsize. -(w+1/n)==one frame size + -- invers
			break;
		case 1:
			glScalef((float)1*texture[last].n, (float)1/texture[last].n, 1.0f); //1==fullsize. -(w+1/n)==one frame size + -- invers
			break;
		case 2:
			glScalef((float)1/texture[last].n, (float)1/texture[last].n, 1.0f); //1==fullsize. -(w+1/n)==one frame size + -- invers
			break;
		case 3:
			glScalef((float)1/texture[last].n, (float)1*texture[last].n, 1.0f); //1==fullsize. -(w+1/n)==one frame size + -- invers
			break;
	}

	//~ glScalef((float)1/texture[last].w, (float)1*texture[last].n, 1.0f);

	glBindTexture( GL_TEXTURE_2D, texture[0].texture );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//---------------------------------
	rotate();
	glTranslated(x, y, 0);	//set position
	glTranslated(texture[last].w/2, texture[last].h/2, 0); //all three for rotate
	glRotatef(rotate_angle,0,0,1);
	glTranslated(-texture[last].w/2, -texture[last].h/2, 0);

	glBegin( GL_QUADS );
	    glColor3f(1.0f,1.0f,1.0f);

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
	if(body) hack_2d();
}

short base::check_state(void)
{
	if(body) dBodyGetRelPointVel (body, 0, 0, 0, pointrelvel);
	if (pointrelvel[1] > -1 && pointrelvel[1] < 1)
	{
		stand++;	
		if (stand>=1 && c_tmp==0) Dup=1,c_tmp=1;		//if >=1 enable dj
		if (stand>=10) Tup=0, Dup=0, c_tmp=0;		//enable dj on floor //stand>=X --X time for set stand
		if (Dup==1) Tup=2, Dup=0;
	}
	else stand=0;

	if(body) dBodyGetRelPointVel(body,0,0,0,pointrelvel);
	if (pointrelvel[1] > 1 || pointrelvel[1] < -1)
	{
		fly++;
//		if (fly>=3) Tup=1, Dup=0; //fly>=X where X is time to keep not_jump state
		if (fly>=8) Tup=1, Dup=0; //fly>=X where X is time to keep not_jump state
	}
	else fly=0;
//test	
	return Tup;	//1 - first j. 2- dj. 0 - floor
}

void base::img_load (int _w, int _h, const char *str, int _n, short t_n, short type) 
{//t_n - texture's number// type: 0, 1
	SDL_Surface *surface;
	GLenum texture_format;
	GLint  n_of_colors;
	if ( (surface = IMG_Load(str)) )
	//~ if ( (surface = SDL_LoadBMP(str)) )
	{
		n_of_colors = surface->format->BytesPerPixel;
		//~ texture_format = GL_LUMINANCE;
		//~ texture_format = GL_LUMINANCE_ALPHA;
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
		glTexImage2D( GL_TEXTURE_2D, 0, n_of_colors, surface->w, surface->h, 0,				//it do not allow color+texture
		texture_format, GL_UNSIGNED_BYTE, surface->pixels );
		switch(type)
		{ //S == x; T == y;
			case 0:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				texture[t_n].w=_w;
				texture[t_n].h=_h;
				break;
			case 1:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
				texture[t_n].w=_w/_n;
				texture[t_n].h=_h;
				break;
			case 2:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
				texture[t_n].w=_w/_n;
				texture[t_n].h=_h/_n;
				break;
			case 3:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				texture[t_n].w=_w;
				texture[t_n].h=_h/_n;
				break;
			default:
				fprintf(stderr, "base:::img_load::warning: unknown img type %i\n", type);
				break;
		}
		texture[t_n].n=_n;
		texture[t_n].type=type;
	}
	else fprintf(stderr,"base:::img_load::error: loading error %s\n %i\n", str);
}
