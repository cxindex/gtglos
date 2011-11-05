#include "statbox.h"

int statbox::create(char *img,int NumFrames, int Pause,
							int r, int g, int b,SDL_Surface *screen)
{
	svsInit svsInit;
	//~ svsBase svsBase;
	svsInit.bmpFile=img;
	svsInit.numFrames=NumFrames;
	svsInit.pause=Pause;
	svsInit.r=r;
	svsInit.g=g;
	svsInit.b=b;
	base_box.init(&svsInit);
	
	//~ if(base_box.svsNumFrames>1) svsAnimating=1;
	//~ svsBackreplacement = SDL_DisplayFormat(base_box.svsAnim[0].image);
	//~ svsBackreplacement = SDL_DisplayFormat(base_box.svsAnim->image);
	svsBackreplacement = SDL_DisplayFormat(base_box.svsAnim->image);
	svsScreen = screen;
	//~ return 0;
}
int statbox::set(SDL_Surface* screen,int xx, int yy)
{
    SDL_Rect dest;
	dest.w = base_box.svsW;
    dest.h = base_box.svsH;
   	dGeomSetPosition(box, xx, yy, 0);
    //~ dest.x = xx; //должно быть ноль
    dest.x = xx-(rw()/2); //должно быть ноль
    //~ dest.y = (SCREEN_Y-base_box.svsH)-yy+50; //должно быть 100
    //~ dest.y = SCREEN_Y-yy-10/2; //должно быть 100 тут поработать
    dest.y = SCREEN_Y-yy-(rh()/2); //должно быть 100 тут поработать
    SDL_BlitSurface(base_box.svsAnim->image, NULL, screen, &dest);
    //~ (rh()/1.5)
    //~ int rh(void) { return dynobjBase->svsH;};
}

int statbox::ode_create(dSpaceID space, int xx, int yy)
{
	dGeomID box = dCreateBox (space, xx, yy, 0);
}
//~ //1
//~ make base
	//~ svsInit svsInit;
	//~ svsBase svsBase;
	//~ svsInit.bmpFile=bmpFile;
	//~ svsInit.numFrames=numFrames;
	//~ svsInit.pause=pause;
	//~ svsInit.r=r;
	//~ svsInit.g=g;
	//~ svsInit.b=b;
	//~ svsBase.init(&svsInit);
	//~ return svsBase;
//~ //2
//~ int dynobj::init(svsBase *base, SDL_Surface *screen)
//~ {
  //~ dynobjBase = base;
  //~ if(dynobjBase->svsNumFrames>1) svsAnimating=1;
  //~ svsBackreplacement = SDL_DisplayFormat(dynobjBase->svsAnim[0].image);
  //~ svsScreen = screen;
  //~ return 0;
//~ 
//~ //3
  //~ init(&basewalk_Right,screen);
