//	Michael Dawe
//	CS520 Inference Engine
//	mdawe@digipen.edu

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>

#include "GLinc.h"
#include "GLFont2.h"
using namespace glfont;

#include "Agent.h"

#include "global.h"
#include "debug.h"
#include "World.h"

#include "LuaWrapper.h"

float xpos = 0, ypos = 0;
int width, height;
int mouseX, mouseY;

void DrawScene();
void ProcessHits(GLuint, GLuint[]);

World g_world;
Lua::LuaWrapper *g_lua;
GLFont g_font;

void init() {
	if(!g_font.Create("courierNew.glf", 0)) {
		dprintf("Couldn't create font");
	}

	g_lua = Lua::LuaWrapper::Instance();
	g_lua->Initialize();
	g_lua->Call("ReadFile", "Lua Scripts/patrol.lua");

	g_world.Load(g_lua);
}

int min(int a, int b) {
	if(a < b) {
		return a;
	} 
	return b;
}

void Keyboard(unsigned char key, int, int) {
	switch(key) {
		case 'p':
		case 'P':
			g_world.TogglePlanMerge();
			break;
		case 'i':
		case 'I':
			g_world.GenerateItems();
			break;
		case 27:	//esc
		case 'q':
			exit(0);
			break;
	}
}

void Reshape(int w, int h)
{
	width  = w;
	height = h;
	
    if (w && h) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, 0, h, 0, -1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	}
}

void ProcessHits(GLuint hits, GLuint buffer[]) {
	int numberOfNames, ptr;
	
	//printf("hits = %d\n", hits);
	ptr = 0;
	for (GLuint i = 0; i < hits; i++) {        /*  for each hit  */
		numberOfNames = buffer[ptr];
		//std::cout << " number of names for hit = " << numberOfNames << std::endl;
		++ptr;
		//std::cout << "  z1 is " <<  buffer[ptr] << std::endl;
		++ptr;
		//std::cout << " z2 is " << buffer[ptr] << std::endl;
		++ptr;
		//std::cout << "   the name is ";
		for (GLuint j = 0; j < numberOfNames; j++) {   /*  for each name */
//			if(buffer[ptr] == HOUR_HAND) {
//				hourHandSelected = true;
//			} else if (buffer[ptr] == MINUTE_HAND) {
//				minuteHandSelected = true;
//			}
			//std::cout << buffer [ptr] << " ";
			++ptr;
		}
		//std::cout << std::endl;
	}
}

void Mouse(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GLint hits;
		GLint viewport[4];
		GLuint buffer[40];
		glSelectBuffer(40, buffer);
		
		glGetIntegerv(GL_VIEWPORT, viewport);
		
		glRenderMode(GL_SELECT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix(x, viewport[3] - y, 5.0, 5.0, viewport);	
		glOrtho(0, width, 0, height, 0, -1);
		DrawScene();
			
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		
		hits = glRenderMode(GL_RENDER);
		if(hits > 0) {
			//std::cout << "Hit!" << std::endl;
		}
		ProcessHits(hits, buffer);
	}
	
	mouseX = x - width / 2;
	mouseY = -1 * (y - height / 2);	
}

void MouseMotion(int x, int y) {
	x = x - width / 2;
	y = -1 * (y - height / 2);
	
	mouseX = x;
    mouseY = y;
	
	glutPostRedisplay();
}

void Timer(int value) {
	g_world.Update();
	glutTimerFunc(1000, &Timer, 1);
}
void ItemTimer(int value) {
	
	//Put the item into the world
	//int x = rand() % g_world.GetCols();
	//int y = rand() % g_world.GetRows();;

	//g_world.AddItem(WorldPoint(x,y));

	//glutTimerFunc(30 * 1000, &ItemTimer, 1);
}

void DrawScene() {
	glInitNames();
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	int minSquare = min(width, height);
	//std::cout << "Drawing gpa" << std::endl;
	g_world.DrawGPA(0,minSquare,minSquare);
	//glFlush();
	//glutSwapBuffers();
	//Sleep(2000);

	//std::cout << "Drawing world" << std::endl;
	g_world.Draw(minSquare, minSquare);
	//glFlush();
	//glutSwapBuffers();
	//Sleep(2000);
	
	glPopMatrix();
	//glFlush();
}

void Display() {
	glMatrixMode(GL_MODELVIEW);
	DrawScene();
	glutSwapBuffers();
}

int main (int argc, char * argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	width  = 800;
	height = 640;
	glutCreateWindow("GOAP Demo");
	
	glutReshapeFunc(&Reshape);
	glutDisplayFunc(&Display);
	glutKeyboardFunc(&Keyboard);
	glutMotionFunc(&MouseMotion);
	glutMouseFunc(&Mouse);
	glutIdleFunc(&Display);
	glutTimerFunc(1000, &Timer, 1);
	glutTimerFunc(20 * 1000, &ItemTimer, 1);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,	GL_ONE_MINUS_SRC_ALPHA);

	init();
	glutMainLoop();
}
