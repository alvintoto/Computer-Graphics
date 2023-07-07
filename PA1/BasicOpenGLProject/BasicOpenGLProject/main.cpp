#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>

int counter = 0;

float x_tran = 0.0f;
float y_tran = 0.0f;
float x_scale = 1.0f;
float y_scale = 1.0f;
float z_scale = 1.0f;
float red_scale = 1.0f;
float green_scale = 0.0f;
float blue_scale = 0.0f;

//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	//glutPostRedisplay();
}

void reshape_func( int width, int height )
{
	glViewport( 0, 0, width, height );
	glutPostRedisplay();
}

void keyboard_func( unsigned char key, int x, int y )
{
	switch( key )
	{
		case 'w':
		{
			y_tran += 0.05;
			break;
		}

		case 'a':
		{
			x_tran -= 0.05;
			break;
		}

		case 's':
		{
			y_tran -= 0.05;
			break;
		}

		case 'd':
		{
			x_tran += 0.05;
			break;
		}

		case 'z':
		{
			x_scale += 0.05;
			y_scale += 0.05;
			z_scale += 0.05;
			break;
		}
		
		case 'x':
		{
			x_scale -= 0.05;
			y_scale -= 0.05;
			z_scale -= 0.05;
			break;
		}

		case 'h':
		{
			std::cout << "WASD to translate letters" << std::endl;
			std::cout << "Z and X to scale letters" << std::endl;
			std::cout << "click to change color of letters" << std::endl;
			break;
		}

		// Exit on escape key press
		case '\x1B':
		{
			exit( EXIT_SUCCESS );
			break;
		}
	}

	glutPostRedisplay();
}

void key_released( unsigned char key, int x, int y )
{
}

void key_special_pressed( int key, int x, int y )
{
}

void key_special_released( int key, int x, int y )
{
}

void mouse_func( int button, int state, int x, int y )
{
	if (button == 0 && state == 1 && counter == 0) {
		red_scale = 0;
		green_scale = 1;
		counter++;
	}
	else if (button == 0 && state == 1 && counter == 1) {
		green_scale = 0;
		blue_scale = 1;
		counter++;
	}
	else if (button == 0 && state == 1 && counter == 2) {
		blue_scale = 0;
		red_scale = 1;
		counter = 0;
	}
}

void passive_motion_func( int x, int y )
{
}

void active_motion_func( int x, int y )
{
}

//=================================================================================================
// RENDERING
//=================================================================================================

void display_func( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glScaled(x_scale, y_scale, z_scale);
	glColor3f(red_scale, green_scale, blue_scale);

	// A
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.62 + x_tran, 0.0f + y_tran);
	glVertex2f(-0.6f + x_tran, 0.0f + y_tran);
	glVertex2f(-0.5f + x_tran, 0.5f + y_tran);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.42f + x_tran, 0.0f + y_tran);
	glVertex2f(-0.4f + x_tran, 0.0f + y_tran);
	glVertex2f(-0.5f + x_tran, 0.5f + y_tran);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.6f + x_tran, 0.2f + y_tran);
	glVertex2f(-0.4f + x_tran, 0.2f + y_tran);
	glVertex2f(-0.4f + x_tran, 0.25f + y_tran);
	glEnd();

	// L
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.2f + x_tran, 0.5f + y_tran);
	glVertex2f(-0.25f + x_tran, 0.5f + y_tran);
	glVertex2f(-0.2f + x_tran, -0.1f + y_tran);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f + x_tran, 0.05f + y_tran);
	glVertex2f(0.0f + x_tran, 0.02f + y_tran);
	glVertex2f(-0.225f + x_tran, 0.0f + y_tran);
	glEnd();

	// V
	glBegin(GL_TRIANGLES);
	glVertex2f(0.1f + x_tran, 0.5f + y_tran);
	glVertex2f(0.15f + x_tran, 0.5f + y_tran);
	glVertex2f(0.25f + x_tran, -0.1f + y_tran);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0.35f + x_tran, 0.5f + y_tran);
	glVertex2f(0.4f + x_tran, 0.5f + y_tran);
	glVertex2f(0.22f + x_tran, -0.1f + y_tran);
	glEnd();

	// I
	glBegin(GL_TRIANGLES);
	glVertex2f(0.63f + x_tran, 0.5f + y_tran);
	glVertex2f(0.67f + x_tran, 0.5f + y_tran);
	glVertex2f(0.65f + x_tran, -0.1f + y_tran);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0.5f + x_tran, 0.45f + y_tran);
	glVertex2f(0.5f + x_tran, 0.5f + y_tran);
	glVertex2f(0.8f + x_tran, 0.48f + y_tran);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0.8f + x_tran, -0.02f + y_tran);
	glVertex2f(0.8f + x_tran, -0.07f + y_tran);
	glVertex2f(0.5f + x_tran, -0.07f + y_tran);
	glEnd();

	glutSwapBuffers();
}

//=================================================================================================
// INIT
//=================================================================================================

void init( void )
{
	// Print some info
	std::cout << "Vendor:         " << glGetString( GL_VENDOR   ) << "\n";
	std::cout << "Renderer:       " << glGetString( GL_RENDERER ) << "\n";
	std::cout << "OpenGL Version: " << glGetString( GL_VERSION  ) << "\n\n";

	// Set the background color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	std::cout << "Finished initializing...\n\n";
}

//=================================================================================================
// MAIN
//=================================================================================================

int main( int argc, char** argv )
{
	glutInit( &argc, argv );

	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( 800, 600 );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "CSE-170 Computer Graphics" );

	glutDisplayFunc( display_func );
	glutIdleFunc( idle_func );
	glutReshapeFunc( reshape_func );
	glutKeyboardFunc( keyboard_func );
	glutKeyboardUpFunc( key_released );
	glutSpecialFunc( key_special_pressed );
	glutSpecialUpFunc( key_special_released );
	glutMouseFunc( mouse_func );
	glutMotionFunc( active_motion_func );
	glutPassiveMotionFunc( passive_motion_func );

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
