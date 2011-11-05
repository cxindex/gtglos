#include "main.h"
#include "base.h"

void print_bitmap_string(void* font, char* s)
{
	if (s && strlen(s))
	{
		while (*s)
		{
			glutBitmapCharacter(font, *s);
			s++;
		}
	}
}

void get_euler(const dReal* matrix, dReal &kx, dReal &ky, dReal &kz)
{       
	const dReal epsilon=0.0000001;
	if(matrix[2] < 1-epsilon && matrix[2] > -1+epsilon)
	{
		ky=-asin(matrix[2]);
		dReal c=cos(ky);

		kx= atan2(matrix[6]/c,matrix[10]/c);
		kz= atan2(matrix[1]/c,matrix[0]/c);

	} else
	{       
		kz=0;   
		ky=-atan2(matrix[2],0);
		kx= atan2(-matrix[9],matrix[5]);
	}       
}

dReal* set_eular(double deg)
{
	double rad = deg/-57;
	dReal *m = (dReal*) malloc(sizeof(dReal)*12);
	//~ dReal m[12];

	m[0] = cos(rad);
	m[1] = sin(rad);
	m[4] = -sin(rad);
	m[5] = cos(rad);
	m[10] = 1;
	
	m[2] = 0;
	m[3] = 0;
	m[6] = 0;
	m[7] = 0;
	m[8] = 0;
	m[9] = 0;
	m[11] = 0;
	//~ printf("%.3f %.3f\n", m[0], m[1]);
	//~ printf("1111 %.3f %.3f\n", m[4], m[5]);
	return m;
}

void make_world_conf(const char *config_path, base *wall_array)
{
	config_t fc;
	config_init(&fc);
	
	if(config_read_file (&fc, config_path))
		printf("config is inited and opened\n");
	else
	{
		printf("cant open config %s\n", config_path);
		return;
	}

	
	config_setting_t *settings, *idx;
	char path[200]="main.blocks.[0]";
	int w,h, x,y; //tmp
	//~ static int to=0;
	
	for ( int i=0; settings = config_lookup (&fc, path); i++ )
	{
		printf("\npath is: %s\n", path);

		for ( int li=0; idx = config_setting_get_elem (settings, li); li++ )
		{
			char *setting_name;
			setting_name = config_setting_name (idx);
			
			if(!strcmp(setting_name, "title"))
			{
				wall_array[i].str = config_setting_get_string (idx);
				printf("hello: %s\n", wall_array[i].str);
			}
			else if(!strcmp(setting_name, "w"))
			{
				w = config_setting_get_int (idx);
				printf("W:%i\n", w);

			}
			else if(!strcmp(setting_name, "h"))
			{
				h = config_setting_get_int (idx);
				printf("H:%i\n", h);
				wall_array[i].geom = dCreateBox (space, w, h, 10);
				wall_array[i].img_load(w, h, "panel.bmp", 3, 0, 0); //with this == texure. withou == no_texture
			}
			else if(!strcmp(setting_name, "x"))
			{
				x = config_setting_get_int (idx);
				printf("X:%i\n", x);

			}
			else if(!strcmp(setting_name, "y"))
			{
				y = config_setting_get_int (idx);
				printf("Y:%i\n", y);
				dGeomSetPosition (wall_array[i].geom, x, y, 0);

				//~ wall_array[i].set_pos(x, y);
			}
			
		}
		sprintf(path, "main.blocks.[%i]", i);
	}
}
