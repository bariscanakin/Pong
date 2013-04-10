//
//  main.cpp
//  Ders 6
//
//  Created by Gurel Erceis on 3/17/13.
//  Copyright (c) 2013 Yogurt3D. All rights reserved.
//
// Thing to install and test before writing any code
// glm
// assimp (http://assimp.sourceforge.net/main_downloads.html)

#include <iostream>
#include <GL/glew.h>
#include "Scene.h"
#include <GL/glut.h>
#include "ScreenSpaceShaderNode.h"
#include "NormalShaderNode.h"
#include "ColorFillNode.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#define WINDOW_TITLE_PREFIX "Ders 7"

void IdleFunction(void);
void ResizeFunction(int Width, int Height);
void setupScene();

int CurrentWidth = 800,
CurrentHeight = 800,
WindowHandle = 0, mouseX = 0, mouseY = 0;

SceneNode* pad1;
SceneNode* pad2;
SceneNode* board;
SceneNode* ball;

Camera *camera;

Scene* scene;

float directionX = 1;
float directionY = 0.5;
float speed = 0.001;

bool directionChanged = false;
bool gameOver = false;

int player1 = 0;
int player2 = 0;

static void Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    scene->draw();
    glutSwapBuffers();
}

static void Key(unsigned char key, int x, int y)
{
    switch (key) {
		case 27: 
			exit(0);
            break;
        case 'a':
			if(pad1->getPosition().y > -0.84f) {
				pad1->translate(0, -2, 0);
			}
			break;
        case 'q': 
			if(pad1->getPosition().y < 0.86f) {
				pad1->translate(0, 2, 0);
			}
            break;
        case 's':
			if(pad2->getPosition().y > -0.84f) {
				pad2->translate(0, -2, 0);
			}
            break;
        case 'w':
			if(pad2->getPosition().y < 0.86f) {
				pad2->translate(0, 2, 0);
			}
            break;
        case 'v':
			speed /= 2;
            break;    
        case 'b':
			speed *= 2;
            break;
	}
}
void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

static void timerCallback (int value)
{
    if (0 != value) {
		char* TempString = (char*)
        malloc(512 + strlen(WINDOW_TITLE_PREFIX));
        
		sprintf(
                TempString,
                "Player1: %d - %d: Player2",
                player1,
                player2
                );
        
		glutSetWindowTitle(TempString);
		free(TempString);
	}
	glutTimerFunc(1, timerCallback, 1);
}

void IdleFunction(void)
{
    float x = ball->getPosition().x;
    float y = ball->getPosition().y;
	
    if( y > 1.1 )
    {
        directionY *= -1;
    }
    if( y < -1.08 )
    {
        directionY *= -1;
    }
   
	if(x <= -0.945 )
	{
		if((y >= pad1->getPosition().y-0.3 && y <= pad1->getPosition().y + 0.3) && !directionChanged && !gameOver)
		{
			directionX *= -1;
			directionChanged = true;
		}
		else
		{
			gameOver = true;
			
		}
	}
	if(x >= 0.93)
	{
		if((y >= pad2->getPosition().y-0.3 && y <= pad2->getPosition().y + 0.3) && !directionChanged && !gameOver)
		{
			directionX *= -1;
			directionChanged = true;
		}
		else
		{
			gameOver = true;
		}
	}
	if((x > -0.80) && (x < 0.80))
	{
			directionChanged = false;
	}
	if( x > 1.4 )
    {
		player1++;
		gameOver = false;
        ball->setModelMatrix(glm::mat4x4(1.0f));
		ball->scale(0.03,0.03,0.03);
    }
    if( x < -1.4 )
    {
		player2++;
		gameOver = false;
        ball->setModelMatrix(glm::mat4x4(1.0f));
		ball->scale(0.03,0.03,0.03);
    }
	
    ball->translate(directionX*speed*40, directionY*speed*40, 0);
	glutPostRedisplay();
}

void setupScene(){
    scene = new Scene();
    
    pad1 = new ColorFillNode("stick.obj");
    pad2 = new ColorFillNode("stick.obj");
	board = new NormalShaderNode("background.obj");
	ball = new NormalShaderNode("ball.obj");
    
	board->scale(0.5,0.45,0.45);
    pad1->scale(0.01,0.01,0.01);
    pad2->scale(0.01,0.01,0.01); 
    ball->scale(0.03,0.03,0.03);
	
    scene->addNode(board);
    scene->addNode(pad1);
    scene->addNode(pad2);
    scene->addNode(ball);
    camera = new Camera();
    
    camera->translate(0.f, 0.f, 3.f);
    board->translate(-0.11f, 0.f, 0.f);
    pad1->translate(-99.f, 0.f, 0.f);
    pad2->translate(101.f, 0.f, 0.f);
    
    scene->setCamera(camera);
}

int main (int argc, char ** argv)
{
    
    GLenum type;
    
	glutInit(&argc, argv);
	glutInitWindowSize(CurrentWidth,CurrentHeight);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
    type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // Z-Buffer i aciyoruz
    glEnable(GL_DEPTH_TEST);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE); 
    
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    printf("GLSL Version: %s\n",glslVersion);
    
    const GLubyte* glVersion = glGetString(GL_VERSION);
    
    printf("GL Version: %s\n",glVersion);
       
    setupScene();
    
    timerCallback(0);
    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(Draw);
    glutKeyboardFunc(Key);
    glutIdleFunc(IdleFunction);
    glutMainLoop();
    
    return 0;
}


