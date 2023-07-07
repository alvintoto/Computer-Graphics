#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <math.h>
#include "shader.h"
#include "shaderprogram.h"

/*=================================================================================================
	DOMAIN
=================================================================================================*/

// Window dimensions
const int InitWindowWidth  = 800;
const int InitWindowHeight = 800;
int WindowWidth  = InitWindowWidth;
int WindowHeight = InitWindowHeight;

// Last mouse cursor position
int LastMousePosX = 0;
int LastMousePosY = 0;

// Arrays that track which keys are currently pressed
bool key_states[256];
bool key_special_states[256];
bool mouse_states[8];

// Other parameters
bool draw_wireframe = false;
bool flat_shading = true;
bool smooth_shading = false;
bool draw_normals = false;

int n = 10;
float r = 1;
float R = 3;

int counter = 0;
const char* filenames[3] = { "./images/rock.jpg", "./images/leather.jpg", "./images/red.jpg" };


float calc_param(int x, int n) {
	return x * ((2 * 3.14) / n);
}

float* torus_vertices(int n, float r, float R) {
	int total_points = pow(n,2) * 24;
	std::vector<float> temp;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			temp.push_back((R + (r * cos(calc_param(j, n)))) * cos(calc_param(i, n)));
			temp.push_back((R + (r * cos(calc_param(j, n)))) * sin(calc_param(i, n)));
			temp.push_back(r * sin(calc_param(j, n)));
			temp.push_back(1);

			temp.push_back((R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n)));
			temp.push_back((R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n)));
			temp.push_back(r * sin(calc_param(j, n)));
			temp.push_back(1);

			temp.push_back((R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n)));
			temp.push_back((R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n)));
			temp.push_back(r * sin(calc_param(j + 1, n)));
			temp.push_back(1);

			temp.push_back((R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i + 1, n)));
			temp.push_back((R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i + 1, n)));
			temp.push_back(r * sin(calc_param(j + 1, n)));
			temp.push_back(1);

			temp.push_back((R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n)));
			temp.push_back((R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n)));
			temp.push_back(r * sin(calc_param(j + 1, n)));
			temp.push_back(1);

			temp.push_back((R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n)));
			temp.push_back((R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n)));
			temp.push_back(r * sin(calc_param(j, n)));
			temp.push_back(1);
		}
	}

	float* output = new float[total_points];
	for (int i = 0; i < total_points; ++i) {
		output[i] = temp[i];
	}

	return output;
}


float* flat_normals(int n, float r, float R) {
	int total_points = pow(n, 2) * 24;
	std::vector<float> temp;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			float x1 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i, n));
			float y1 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i, n));
			float z1 = (r * sin(calc_param(j, n)));

			float x2 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n));
			float y2 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n));
			float z2 = (r * sin(calc_param(j, n)));

			float x3 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n));
			float y3 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n));
			float z3 = (r * sin(calc_param(j + 1, n)));

			glm::vec3 u = glm::vec3(x2 - x1, y2 - y1, z2 - z1);
			glm::vec3 v = glm::vec3(x3 - x1, y3 - y1, z3 - z1);

			glm::vec3 normal = glm::cross(u, v);

			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);

			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);

			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);


			x1 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i + 1, n));
			y1 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i + 1, n));
			z1 = (r * sin(calc_param(j + 1, n)));

			x2 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n));
			y2 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n));
			z2 = (r * sin(calc_param(j + 1, n)));

			x3 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n));
			y3 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n));
			z3 = (r * sin(calc_param(j, n)));

			u = glm::vec3(x2 - x1, y2 - y1, z2 - z1);
			v = glm::vec3(x3 - x1, y3 - y1, z3 - z1);

			normal = glm::cross(u, v);

			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);

			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);

			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);
		}
	}

	float* output = new float[total_points];
	for (int i = 0; i < total_points; ++i) {
		output[i] = temp[i];
	}

	return output;
}

float* draw_flat_normals(int n, float r, float R) {
	int total_points = pow(n, 2) * 16;
	std::vector<float> temp;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			float x1 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i, n));
			float y1 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i, n));
			float z1 = (r * sin(calc_param(j, n)));

			float x2 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n));
			float y2 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n));
			float z2 = (r * sin(calc_param(j, n)));

			float x3 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n));
			float y3 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n));
			float z3 = (r * sin(calc_param(j + 1, n)));

			glm::vec3 u = glm::vec3(x2 - x1, y2 - y1, z2 - z1);
			glm::vec3 v = glm::vec3(x3 - x1, y3 - y1, z3 - z1);

			glm::vec3 normal = glm::cross(u, v);

			temp.push_back((x1 + x2 + x3) / 3);
			temp.push_back((y1 + y2 + y3) / 3);
			temp.push_back((z1 + z2 + z3) / 3);
			temp.push_back(1);

			temp.push_back((x1 + x2 + x3) / 3 + normal.x);
			temp.push_back((y1 + y2 + y3) / 3 + normal.y);
			temp.push_back((z1 + z2 + z3) / 3 + normal.z);
			temp.push_back(1);



			x1 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i + 1, n));
			y1 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i + 1, n));
			z1 = (r * sin(calc_param(j + 1, n)));

			x2 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n));
			y2 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n));
			z2 = (r * sin(calc_param(j + 1, n)));

			x3 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n));
			y3 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n));
			z3 = (r * sin(calc_param(j, n)));

			u = glm::vec3(x2 - x1, y2 - y1, z2 - z1);
			v = glm::vec3(x3 - x1, y3 - y1, z3 - z1);

			normal = glm::cross(u, v);

			temp.push_back((x1 + x2 + x3) / 3);
			temp.push_back((y1 + y2 + y3) / 3);
			temp.push_back((z1 + z2 + z3) / 3);
			temp.push_back(1);

			temp.push_back((x1 + x2 + x3) / 3 + normal.x);
			temp.push_back((y1 + y2 + y3) / 3 + normal.y);
			temp.push_back((z1 + z2 + z3) / 3 + normal.z);
			temp.push_back(1);
		}
	}

	float* output = new float[total_points];
	for (int i = 0; i < total_points; ++i) {
		output[i] = temp[i];
	}

	return output;
}


float* smooth_normals(int n, float r, float R) {
	int total_points = pow(n, 2) * 24;
	std::vector<float> temp;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			float x1 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i, n));
			float y1 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i, n));
			float z1 = (r * sin(calc_param(j, n)));

			float x2 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n));
			float y2 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n));
			float z2 = (r * sin(calc_param(j, n)));

			float x3 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n));
			float y3 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n));
			float z3 = (r * sin(calc_param(j + 1, n)));

			glm::vec3 normal = glm::vec3(x1 - R * cos(calc_param(i, n)), y1 - R * sin(calc_param(i, n)), z1);
			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);

			//////////////////

			normal = glm::vec3(x2 - R * cos(calc_param(i + 1, n)), y2 - R * sin(calc_param(i + 1, n)), z2);
			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);

			//////////////////

			normal = glm::vec3(x3 - R * cos(calc_param(i, n)), y3 - R * sin(calc_param(i, n)), z3);
			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);


			x1 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i + 1, n));
			y1 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i + 1, n));
			z1 = (r * sin(calc_param(j + 1, n)));

			x2 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n));
			y2 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n));
			z2 = (r * sin(calc_param(j + 1, n)));

			x3 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n));
			y3 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n));
			z3 = (r * sin(calc_param(j, n)));

			normal = glm::vec3(x1 - R * cos(calc_param(i + 1, n)), y1 - R * sin(calc_param(i + 1, n)), z1);
			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);

			//////////////////

			normal = glm::vec3(x2 - R * cos(calc_param(i, n)), y2 - R * sin(calc_param(i, n)), z2);
			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);

			//////////////////

			normal = glm::vec3(x3 - R * cos(calc_param(i + 1, n)), y3 - R * sin(calc_param(i + 1, n)), z3);
			temp.push_back(normal.x);
			temp.push_back(normal.y);
			temp.push_back(normal.z);
			temp.push_back(1);
		}
	}

	float* output = new float[total_points];
	for (int i = 0; i < total_points; ++i) {
		output[i] = temp[i];
	}

	return output;
}

float* draw_smooth_normals(int n, float r, float R) {
	int total_points = pow(n, 2) * 16;
	std::vector<float> temp;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			float x1 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i, n));
			float y1 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i, n));
			float z1 = (r * sin(calc_param(j, n)));

			float x2 = (R + (r * cos(calc_param(j, n)))) * cos(calc_param(i + 1, n));
			float y2 = (R + (r * cos(calc_param(j, n)))) * sin(calc_param(i + 1, n));
			float z2 = (r * sin(calc_param(j, n)));

			float x3 = (R + (r * cos(calc_param(j + 1, n)))) * cos(calc_param(i, n));
			float y3 = (R + (r * cos(calc_param(j + 1, n)))) * sin(calc_param(i, n));
			float z3 = (r * sin(calc_param(j + 1, n)));

			temp.push_back(x1);
			temp.push_back(y1);
			temp.push_back(z1);
			temp.push_back(1);

			glm::vec3 normal = glm::vec3(x1 - R * cos(calc_param(i, n)), y1 - R * sin(calc_param(i, n)), z1);
			temp.push_back(x1 + normal.x);
			temp.push_back(y1 + normal.y);
			temp.push_back(z1 + normal.z);
			temp.push_back(1);

			//////////////////

			temp.push_back(x2);
			temp.push_back(y2);
			temp.push_back(z2);
			temp.push_back(1);

			normal = glm::vec3(x2 - R * cos(calc_param(i + 1, n)), y2 - R * sin(calc_param(i + 1, n)), z2);
			temp.push_back(x2 + normal.x);
			temp.push_back(y2 + normal.y);
			temp.push_back(z2 + normal.z);
			temp.push_back(1);
		}
	}

	float* output = new float[total_points];
	for (int i = 0; i < total_points; ++i) {
		output[i] = temp[i];
	}

	return output;
}


float* load_textures(int n, float r, float R) {
	int total_points = pow(n, 2) * 12;
	std::vector<float> temp;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			temp.push_back(0);
			temp.push_back(0);

			temp.push_back(1);
			temp.push_back(0);

			temp.push_back(0);
			temp.push_back(1);

			temp.push_back(1);
			temp.push_back(0);

			temp.push_back(0);
			temp.push_back(1);

			temp.push_back(1);
			temp.push_back(1);
		}
	}

	float* output = new float[total_points];
	for (int i = 0; i < total_points; ++i) {
		output[i] = temp[i];
	}

	return output;
}

void create_texture(void) {
	int width, height, channels;
	unsigned char* texture = stbi_load(filenames[counter], &width, &height, &channels, 0);

	GLuint textureId;
	glGenTextures(1, &textureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(texture);
}



/*=================================================================================================
	SHADERS & TRANSFORMATIONS
=================================================================================================*/

ShaderProgram PassthroughShader;
ShaderProgram PerspectiveShader;
ShaderProgram NewShader;
ShaderProgram NewShader1;

glm::mat4 PerspProjectionMatrix( 1.0f );
glm::mat4 PerspViewMatrix( 1.0f );
glm::mat4 PerspModelMatrix( 1.0f );

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;

/*=================================================================================================
	OBJECTS
=================================================================================================*/

GLuint axis_VAO;
GLuint axis_VBO[2];

float axis_vertices[] = {
	//x axis
	-1.0f,  0.0f,  0.0f, 1.0f,
	1.0f,  0.0f,  0.0f, 1.0f,
	//y axis
	0.0f, -1.0f,  0.0f, 1.0f,
	0.0f,  1.0f,  0.0f, 1.0f,
	//z axis
	0.0f,  0.0f, -1.0f, 1.0f,
	0.0f,  0.0f,  1.0f, 1.0f
};

float axis_colors[] = {
	//x axis
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	//y axis
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	//z axis
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f
};

GLuint torus_VAO;
GLuint torus_VBO[4];

GLuint normals_VAO;
GLuint normals_VBO[2];

/*=================================================================================================
	HELPER FUNCTIONS
=================================================================================================*/

void window_to_scene( int wx, int wy, float& sx, float& sy )
{
	sx = ( 2.0f * (float)wx / WindowWidth ) - 1.0f;
	sy = 1.0f - ( 2.0f * (float)wy / WindowHeight );
}

/*=================================================================================================
	SHADERS
=================================================================================================*/

void CreateTransformationMatrices( void )
{
	// PROJECTION MATRIX
	PerspProjectionMatrix = glm::perspective<float>( glm::radians( 60.0f ), (float)WindowWidth / (float)WindowHeight, 0.01f, 1000.0f );

	// VIEW MATRIX
	glm::vec3 eye   ( 0.0, 0.0, 2.0 );
	glm::vec3 center( 0.0, 0.0, 0.0 );
	glm::vec3 up    ( 0.0, 1.0, 0.0 );

	PerspViewMatrix = glm::lookAt( eye, center, up );

	// MODEL MATRIX
	PerspModelMatrix = glm::mat4( 1.0 );
	PerspModelMatrix = glm::rotate( PerspModelMatrix, glm::radians( perspRotationX ), glm::vec3( 1.0, 0.0, 0.0 ) );
	PerspModelMatrix = glm::rotate( PerspModelMatrix, glm::radians( perspRotationY ), glm::vec3( 0.0, 1.0, 0.0 ) );
	PerspModelMatrix = glm::scale( PerspModelMatrix, glm::vec3( perspZoom ) );
}

void CreateShaders( void )
{
	// Renders without any transformations
	PassthroughShader.Create( "./shaders/simple.vert", "./shaders/simple.frag" );

	// Renders using perspective projection
	PerspectiveShader.Create( "./shaders/persp.vert", "./shaders/persp.frag" );

	// our new shader
	NewShader.Create("./shaders/persplight.vert", "./shaders/persplight.frag");


	NewShader1.Create("./shaders/texpersplight.vert", "./shaders/texpersplight.frag");
}

/*=================================================================================================
	BUFFERS
=================================================================================================*/

void CreateAxisBuffers( void )
{
	glGenVertexArrays( 1, &axis_VAO );
	glBindVertexArray( axis_VAO );

	glGenBuffers( 2, &axis_VBO[0] );

	glBindBuffer( GL_ARRAY_BUFFER, axis_VBO[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( axis_vertices ), axis_vertices, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void*)0 );
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, axis_VBO[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( axis_colors ), axis_colors, GL_STATIC_DRAW );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void*)0 );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );
}

void UpdateTorus() {
	create_texture();

	float* vertices = torus_vertices(n, r, R);
	float* colors = new float[pow(n, 2) * 24];
	for (int i = 0; i < pow(n, 2) * 24; ++i) {
		colors[i] = 1;
	}
	float* normals;
	

	if (flat_shading) {
		normals = flat_normals(n, r, R);
	}
	else {
		normals = smooth_normals(n, r, R);
	}

	float* textures = load_textures(n, R, r);
	
	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pow(n,2) * 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, pow(n,2) * 24 * sizeof(float), colors+1, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, pow(n, 2) * 24 * sizeof(float), normals + 1, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, pow(n, 2) * 12 * sizeof(float), textures, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	delete[] vertices;
	delete[] normals;
}

void CreateTorusBuffers( void )
{
	glGenVertexArrays(1, &torus_VAO);
	glBindVertexArray(torus_VAO);

	glGenBuffers(4, &torus_VBO[0]);

	UpdateTorus();
}

void UpdateNormals() {
	float* draw_normals;

	if (flat_shading) {
		draw_normals = draw_flat_normals(n, r, R);
	}
	else {
		draw_normals = draw_smooth_normals(n, r, R);
	}

	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pow(n, 2) * 16 * sizeof(float), draw_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, pow(n, 2) * 16 * sizeof(float), draw_normals - 1, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	delete[] draw_normals;
}

void CreateNormalsBuffers(void)
{
	glGenVertexArrays(1, &normals_VAO);
	glBindVertexArray(normals_VAO);

	glGenBuffers(2, &normals_VBO[0]);

	UpdateNormals();
}


/*=================================================================================================
	CALLBACKS
=================================================================================================*/

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	// uncomment below to repeatedly draw new frames
	glutPostRedisplay();
}

void reshape_func( int width, int height )
{
	WindowWidth  = width;
	WindowHeight = height;

	glViewport( 0, 0, width, height );
	glutPostRedisplay();
}

void keyboard_func( unsigned char key, int x, int y )
{
	key_states[ key ] = true;

	switch( key )
	{	
		case 'q':
		{
			n += 5;
			UpdateTorus();
			UpdateNormals();
			break;
		}
		case 'a':
		{
			n -= 5;
			UpdateTorus();
			UpdateNormals();
			break;
		}
		case 'w':
		{
			r += 0.1;
			UpdateTorus();
			UpdateNormals();
			break;
		}
		case 's':
		{
			r -= 0.1;
			UpdateTorus();
			UpdateNormals();
			break;
		}
		case 'e':
		{
			R += 0.1;
			UpdateTorus();
			UpdateNormals();
			break;
		}
		case 'd':
		{
			R -= 0.1;
			UpdateTorus();
			UpdateNormals();
			break;
		}
		case 'z':
		{
			flat_shading = true;
			smooth_shading = false;
			UpdateTorus();
			UpdateNormals();
			break;
		}
		case 'x':
		{
			smooth_shading = true;
			flat_shading = false;
			UpdateTorus();
			UpdateNormals();
			break;
		}
		case 'c':
		{	
			draw_normals = !draw_normals;
			break;
		}
		case 'f':
		{
			draw_wireframe = !draw_wireframe;
			if( draw_wireframe == true )
				std::cout << "Wireframes on.\n";
			else
				std::cout << "Wireframes off.\n";
			break;
		}
		case ' ':
		{
			if (counter == 2) {
				counter = 0;
			}
			else {
				counter++;
			}
			UpdateTorus();
			UpdateNormals();
			break;
		}

		// Exit on escape key press
		case '\x1B':
		{
			exit( EXIT_SUCCESS );
			break;
		}
	}
}

void key_released( unsigned char key, int x, int y )
{
	key_states[ key ] = false;
}

void key_special_pressed( int key, int x, int y )
{
	key_special_states[ key ] = true;
}

void key_special_released( int key, int x, int y )
{
	key_special_states[ key ] = false;
}

void mouse_func( int button, int state, int x, int y )
{
	// Key 0: left button
	// Key 1: middle button
	// Key 2: right button
	// Key 3: scroll up
	// Key 4: scroll down

	if( x < 0 || x > WindowWidth || y < 0 || y > WindowHeight )
		return;

	float px, py;
	window_to_scene( x, y, px, py );

	if( button == 3 )
	{
		perspZoom += 0.03f;
	}
	else if( button == 4 )
	{
		if( perspZoom - 0.03f > 0.0f )
			perspZoom -= 0.03f;
	}

	mouse_states[ button ] = ( state == GLUT_DOWN );

	LastMousePosX = x;
	LastMousePosY = y;
}

void passive_motion_func( int x, int y )
{
	if( x < 0 || x > WindowWidth || y < 0 || y > WindowHeight )
		return;

	float px, py;
	window_to_scene( x, y, px, py );

	LastMousePosX = x;
	LastMousePosY = y;
}

void active_motion_func( int x, int y )
{
	if( x < 0 || x > WindowWidth || y < 0 || y > WindowHeight )
		return;

	float px, py;
	window_to_scene( x, y, px, py );

	if( mouse_states[0] == true )
	{
		perspRotationY += ( x - LastMousePosX ) * perspSensitivity;
		perspRotationX += ( y - LastMousePosY ) * perspSensitivity;
	}

	LastMousePosX = x;
	LastMousePosY = y;
}

/*=================================================================================================
	RENDERING
=================================================================================================*/

void display_func( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	

	CreateTransformationMatrices();

	NewShader1.Use();
	NewShader1.SetUniform( "projectionMatrix", glm::value_ptr( PerspProjectionMatrix ), 4, GL_FALSE, 1 );
	NewShader1.SetUniform( "viewMatrix", glm::value_ptr( PerspViewMatrix ), 4, GL_FALSE, 1 );
	NewShader1.SetUniform( "modelMatrix", glm::value_ptr( PerspModelMatrix ), 4, GL_FALSE, 1 );
	NewShader1.SetUniform("texId", 0);

	if( draw_wireframe == true )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	
	glBindVertexArray( axis_VAO );
	glDrawArrays( GL_LINES, 0, 6 );
	
	glBindVertexArray( torus_VAO );
	glDrawArrays(GL_TRIANGLES, 0, pow(n,2) * 6);

	
	if (draw_normals) {
		glBindVertexArray(normals_VAO);
		glDrawArrays(GL_LINES, 0, pow(n, 2) * 4);
	}

	glBindVertexArray(0);

	if (draw_wireframe == true) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glutSwapBuffers();
}

/*=================================================================================================
	INIT
=================================================================================================*/

void init( void )
{
	// Print some info
	std::cout << "Vendor:         " << glGetString( GL_VENDOR   ) << "\n";
	std::cout << "Renderer:       " << glGetString( GL_RENDERER ) << "\n";
	std::cout << "OpenGL Version: " << glGetString( GL_VERSION  ) << "\n";
	std::cout << "GLSL Version:   " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n\n";

	// Set OpenGL settings
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); // background color
	glEnable( GL_DEPTH_TEST ); // enable depth test
	glEnable( GL_CULL_FACE ); // enable back-face culling

	// Create shaders
	CreateShaders();

	// Create buffers
	CreateAxisBuffers();
	CreateTorusBuffers();
	CreateNormalsBuffers();

	std::cout << "Finished initializing...\n\n";
}

/*=================================================================================================
	MAIN
=================================================================================================*/

int main( int argc, char** argv )
{
	glutInit( &argc, argv );

	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( InitWindowWidth, InitWindowHeight );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "CSE-170 Computer Graphics" );

	// Initialize GLEW
	GLenum ret = glewInit();
	if( ret != GLEW_OK ) {
		std::cerr << "GLEW initialization error." << std::endl;
		glewGetErrorString( ret );
		return -1;
	}

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