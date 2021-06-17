
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <allegro.h>
#include <winalleg.h>

#include <vector>
using namespace std;


#include "csg.h"



// define PI if not defined already
#ifndef M_PI
#define M_PI	3.141592654
#endif


// safe range of point coords
#define MAX_COORD	20936




PolygonMesh2D main_mesh;				// mesh on which user operates


bool csg_mode_sub = false;				// current CSG mode (add/sub)
int  csg_poly_sides = 4;				// selected shape

double view_center_x = 0;				// pan & zoom
double view_center_y = 0;
double view_zoom = 1;

bool	edit_show_lines = true;			// show polygon boundaries

bool	edit_creating = false;			// UI handling
bool	edit_moving = false;
bool	edit_zooming = false;
double	edit_create_x;
double	edit_create_y;
bool	edit_mouse_hidden = false;
bool	edit_exit = false;

int  ui_mouse_x;						// UI mouse handling
int  ui_mouse_y;
bool ui_mouse_down;


// helper function
inline int round(double d)
{
	return int(floor(d+0.5f));
}


// draw Polygon2D shape
void draw_polygon(BITMAP *dst,const Polygon2D &poly,double scale,double ofs_x,double ofs_y,int color,bool fill)
{
	if(poly.is_empty())
		return;

	// clip polygon roughly to screen (Allegro fill function
	// doesn't handle huge coordinates too well, and such
	// coordinates can happen when zooming)
	Polygon2D tmp1, tmp2, tmp3;
	int min_x = int(floor((-1-ofs_x)/scale));
	int min_y = int(floor((-1-ofs_y)/scale));
	int max_x = int(ceil((SCREEN_W+1-ofs_x)/scale));
	int max_y = int(ceil((SCREEN_H+1-ofs_y)/scale));
	if(min_x<-MAX_COORD) min_x = -MAX_COORD;
	if(min_x> MAX_COORD) min_x =  MAX_COORD;
	if(min_y<-MAX_COORD) min_y = -MAX_COORD;
	if(min_y> MAX_COORD) min_y =  MAX_COORD;
	if(max_x<-MAX_COORD) max_x = -MAX_COORD;
	if(max_x> MAX_COORD) max_x =  MAX_COORD;
	if(max_y<-MAX_COORD) max_y = -MAX_COORD;
	if(max_y> MAX_COORD) max_y =  MAX_COORD;

	poly.split_by_line( PointRP2(min_x,min_y).line_to(PointRP2(max_x,min_y)), tmp1, tmp2 );	// top
	tmp2.split_by_line( PointRP2(max_x,min_y).line_to(PointRP2(max_x,max_y)), tmp1, tmp3 );	// right
	tmp3.split_by_line( PointRP2(max_x,max_y).line_to(PointRP2(min_x,max_y)), tmp1, tmp2 );	// bottom
	tmp2.split_by_line( PointRP2(min_x,max_y).line_to(PointRP2(min_x,min_y)), tmp1, tmp3 );	// left

	if(tmp3.is_empty())
		return;


	// prepare list of polygon vertices on screen (x0, y0, x1, y1, ... )
	static vector<int> vtx_coords;

	if(vtx_coords.size()<tmp3.edges.size()*2)
		vtx_coords.resize(tmp3.edges.size()*2);

	for(int i=0;i<tmp3.edges.size();i++)
	{
		int xp = round( tmp3.edges[i].start.get_x()*scale + ofs_x );
		int yp = round( tmp3.edges[i].start.get_y()*scale + ofs_y );
		vtx_coords[i*2  ] = xp;
		vtx_coords[i*2+1] = yp;
	}

	// draw polygon
	if(fill)
	{
		// render as triangle fan
		for(int i=1;i<tmp3.edges.size()-1;i++)
			triangle(dst,
					vtx_coords[0],vtx_coords[1],
					vtx_coords[i*2  ],vtx_coords[i*2+1],
					vtx_coords[i*2+2],vtx_coords[i*2+3],
					color);
	}
	else
	{
		// closed line loop
		for(int i=0;i<tmp3.edges.size();i++)
		{
			int j = (i+1)%tmp3.edges.size();
			line(dst,vtx_coords[i*2],vtx_coords[i*2+1],vtx_coords[j*2],vtx_coords[j*2+1],color);
		}
	}

}

// draw PolygonMesh2D
void draw_mesh(BITMAP *dst,const PolygonMesh2D &mesh,double scale,double ofs_x,double ofs_y,int color,bool fill)
{
	for(int i=0;i<mesh.polys.size();i++)
		draw_polygon(dst,mesh.polys[i],scale,ofs_x,ofs_y,color,fill);
}

// create regular polygon as Polygon2D
void create_regular_polygon(int sides,double xpos,double ypos,double size,double angle,Polygon2D &out)
{
	vector<PointRP2> vtx;
	for(int i=0;i<sides;i++)
	{
		double a = i*2*M_PI/sides + angle;
		double dx = cos(a)*size;
		double dy = sin(a)*size;

		vtx.push_back( PointRP2( round(xpos+dx), round(ypos+dy) ) );
	}

	out = Polygon2D(&vtx[0],sides);
}


// draw and check mouse events for UI button
bool run_button(BITMAP *dst,int xp,int yp,int w,int h,const char *text,const Polygon2D *poly,bool lit)
{
	rectfill(dst,xp,yp,xp+w,yp+h,lit?0xFFFF00:0x000000);
	rect(dst,xp,yp,xp+w,yp+h,0xFFFFFF);
	if(text)
		textout_centre_ex(dst,font,text,xp+w/2,yp+(h+2)/2-4,lit?0x000000:0xFFFFFF,-1);
	if(poly)
		draw_polygon(dst,*poly,1,xp+w/2,yp+h/2,lit?0x000000:0xFF0000,false);

	if(ui_mouse_x>=xp && ui_mouse_x<xp+w && ui_mouse_y>=yp && ui_mouse_y<yp+h)
		return ui_mouse_down;

	return false;
}

// draw and handle UI
void run_interface(BITMAP *dst)
{
	int dmx, dmy;

	// capture mouse state
	get_mouse_mickeys(&dmx,&dmy);
	bool last_mouse_down = ui_mouse_down;
	ui_mouse_down = ((mouse_b&1)!=0);

	// move and zoom view
	if(!ui_mouse_down)
	{
		edit_moving = false;
		edit_zooming = false;
	}

	if(edit_moving || edit_zooming)
	{
		// hide and center mouse for mouse delta capture
		if(!edit_mouse_hidden)
		{
			show_mouse(NULL);
			edit_mouse_hidden = true;
		}

		position_mouse(ui_mouse_x,ui_mouse_y);
	}
	else
	{
		// show mouse and move it back to last position
		if(edit_mouse_hidden)
		{
			show_mouse(screen);
			edit_mouse_hidden = false;
		}

		ui_mouse_x = mouse_x;
		ui_mouse_y = mouse_y;
	}

	if(edit_moving)
	{
		// move view
		view_center_x += dmx/view_zoom;
		view_center_y += dmy/view_zoom;
		ui_mouse_down = false;			// prevent other functions from triggering
	}

	if(edit_zooming)
	{
		// zoom view
		view_zoom *= pow(2,(dmx+dmy)/100.0);
		if(view_zoom<1.0/128) view_zoom = 1.0/128;
		if(view_zoom>32) view_zoom = 32;
		ui_mouse_down = false;			// prevent other functions from triggering
	}

	// start creating polygons
	if(!edit_creating && ui_mouse_down && ui_mouse_x<SCREEN_W-64)
	{
		edit_creating = true;
		edit_create_x = (ui_mouse_x - (SCREEN_W-64)/2)/view_zoom + view_center_x;
		edit_create_y = (ui_mouse_y - SCREEN_H/2     )/view_zoom + view_center_y;
	}

	// handle creating polygons
	if(edit_creating)
	{
		Polygon2D poly;

		// compute desired polygon size and angle
		double vertex_x = (ui_mouse_x - (SCREEN_W-64)/2)/view_zoom + view_center_x - edit_create_x;
		double vertex_y = (ui_mouse_y - SCREEN_H/2     )/view_zoom + view_center_y - edit_create_y;
		double angle = atan2(vertex_y,vertex_x);
		double size = sqrt(vertex_x*vertex_x + vertex_y*vertex_y);

		bool allowed = false;
		if(size>=5)
		{
			// create polygon
			create_regular_polygon(csg_poly_sides,edit_create_x,edit_create_y,size,angle,poly);

			// check if are vertices are within allowed coord range
			allowed = true;
			for(int i=0;i<poly.edges.size();i++)
			{
				double x = poly.edges[i].start.get_x();
				double y = poly.edges[i].start.get_y();
				if(x<-MAX_COORD || x>MAX_COORD || y<-MAX_COORD || y>MAX_COORD)
					allowed = false;
			}

			// draw polygon as it was do be added to the shape
			if(allowed)
			{
				draw_polygon(dst, poly, view_zoom,
								-view_center_x*view_zoom + (SCREEN_W-64)/2,
								-view_center_y*view_zoom + SCREEN_H/2,
								0xFFFF00, false);
			}
		}
		
		// if mouse released, run CSG algorithm and exit creating mode
		if(!ui_mouse_down)
		{
			if(allowed)
			{
				if(csg_mode_sub)	main_mesh.csg_subtract(poly);
				else				main_mesh.csg_add(poly);
			}

			edit_creating = false;
		}

		ui_mouse_down = false;	// prevent UI buttons from triggering
	}

	// draw and handle buttons
	int xp = SCREEN_W-64;
	int yp = 0;

	// ADD/SUB
	if(run_button(dst,xp,yp,64,16,"ADD",NULL,(csg_mode_sub==false)))
		csg_mode_sub = false;
	yp += 16;
	if(run_button(dst,xp,yp,64,16,"SUB",NULL,(csg_mode_sub==true)))
		csg_mode_sub = true;
	yp += 16;

	// spacing
	yp += 16;

	// shape selection
	Polygon2D poly;
	for(int s=3;s<=7;s++)
	{
		create_regular_polygon(s,0,0,28,0,poly);
		if(run_button(dst,xp,yp,64,64,NULL,&poly,(csg_poly_sides==s)))
			csg_poly_sides = s;
		yp += 64;
	}

	// spacing
	yp += 16;

	// MOVE/ZOOM
	if(run_button(dst,xp,yp,64,16,"MOVE",NULL,edit_moving ))
		edit_moving=true;
	yp += 16;
	if(run_button(dst,xp,yp,64,16,"ZOOM",NULL,edit_zooming))
		edit_zooming=true;
	yp += 16;

	// spacing
	yp += 16;

	// LINES
	if(run_button(dst,xp,yp,64,16,"LINES",NULL,edit_show_lines))
		if(!last_mouse_down)
			edit_show_lines = !edit_show_lines;
	yp += 16;

	// spacing
	yp += 16;

	// EXIT
	if(run_button(dst,xp,yp,64,15,"EXIT",NULL,false)) edit_exit = true;
	yp += 16;
}


// draw grid and workspace boundary
void draw_grid(BITMAP *dst)
{
	double scale = view_zoom;
	double ofs_x = -view_center_x*view_zoom + (SCREEN_W-64)/2;
	double ofs_y = -view_center_y*view_zoom + SCREEN_H/2;

	// draw grid only when zoomed close enough
	if(view_zoom>=8)
	{
		double xpos = ceil(-ofs_x/scale)*scale + ofs_x;
		while(xpos<SCREEN_W)
		{
			vline(dst,round(xpos),0,SCREEN_H,0x404040);
			xpos += scale;
		}

		double ypos = ceil(-ofs_y/scale)*scale + ofs_y;
		while(ypos<SCREEN_W)
		{
			hline(dst,0,round(ypos),SCREEN_W,0x404040);
			ypos += scale;
		}
	}

	// draw workspace boundary
	double workspace_min_x = -MAX_COORD*scale + ofs_x;
	double workspace_min_y = -MAX_COORD*scale + ofs_y;
	double workspace_max_x =  MAX_COORD*scale + ofs_x;
	double workspace_max_y =  MAX_COORD*scale + ofs_y;

	if(workspace_min_x<       0) workspace_min_x = -1;
	if(workspace_min_x>SCREEN_W) workspace_min_x = SCREEN_W;
	if(workspace_min_y<       0) workspace_min_y = -1;
	if(workspace_min_y>SCREEN_H) workspace_min_y = SCREEN_H;
	if(workspace_max_x<       0) workspace_max_x = -1;
	if(workspace_max_x>SCREEN_W) workspace_max_x = SCREEN_W;
	if(workspace_max_y<       0) workspace_max_y = -1;
	if(workspace_max_y>SCREEN_H) workspace_max_y = SCREEN_H;

	rect(dst,round(workspace_min_x),round(workspace_min_y),round(workspace_max_x),round(workspace_max_y),0xFFFFFF);

}

// close window button callback
void close_callback()
{
	edit_exit = true;
}


int main()
{
	// init allegro
	allegro_init();
	install_keyboard();
	install_mouse();

	// init screen
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,640,480,0,0);

	set_close_button_callback(close_callback);

	show_mouse(screen);

	BITMAP *backbuffer = create_bitmap(SCREEN_W,SCREEN_H);

	// build initial mesh
	Polygon2D poly;
	create_regular_polygon(5,-50,0,100,0,poly);
	main_mesh.csg_add(poly);

	create_regular_polygon(6,50,0,150,0,poly);
	main_mesh.csg_add(poly);

	// main loop
	while(!key[KEY_ESC] && !edit_exit)
	{
		// draw
		clear(backbuffer);

		draw_mesh(backbuffer, main_mesh, view_zoom,
						-view_center_x*view_zoom + (SCREEN_W-64)/2,
						-view_center_y*view_zoom + SCREEN_H/2,
						edit_show_lines ? 0x400000 : 0x800000, true);

		draw_grid(backbuffer);

		if(edit_show_lines)
		{
			draw_mesh(backbuffer, main_mesh, view_zoom,
							-view_center_x*view_zoom + (SCREEN_W-64)/2,
							-view_center_y*view_zoom + SCREEN_H/2,
							0xFF0000, false);
		}

		textout_ex(backbuffer,font,"CSG editor based on RP2 projective space",4,4,0xC0C0C0,-1);
		textout_ex(backbuffer,font,"Written by Krzysztof Kluczek",4,14,0xC0C0C0,-1);

		// draw & handle interface
		run_interface(backbuffer);

		// blit backbuffer
		vsync();
		blit(backbuffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);

		// give some CPU power to other applications
		rest(10);
	}

	// cleanup
	destroy_bitmap(backbuffer);

	return 0;
}
END_OF_MAIN()
