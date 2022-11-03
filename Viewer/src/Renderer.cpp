#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;

	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color, int flag)
{

	int x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int start_point, end_point, point_toadd, delta_1, delta_2;
	//in case we need to incerment x every iteration
	if (!flag)
	{
		start_point = x1;
		end_point = x2;
		delta_1 = dx;
		point_toadd = y1;
		delta_2 = dy;
	}
	//in case we need to incerment y every iteration
	else
	{
		start_point = y1;
		end_point = y2;
		delta_1 = dy;
		point_toadd = x1;
		delta_2 = dx;
	}
	int e = -delta_1;
	int to_add = 1;
	//check if we need to reflect
	if (delta_2 < 0)
	{
		delta_2 = -delta_2;
		to_add = -1;
	}
	while (start_point < end_point)
	{
		if (e > 0)
		{
			point_toadd = point_toadd + to_add;
			e = e - 2 * delta_1;
		}
		//check that we put pixel correctly(x should be first cordinate and y second)
		if (!flag)
			PutPixel(start_point, point_toadd, color);
		else
			PutPixel(point_toadd, start_point, color);

		e = e + 2 * delta_2;
		start_point++;
	}
}

void Renderer::ChangePoints(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	int dx = p2.x - p1.x, dy = p2.y - p1.y;
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;
	//in this case A is from 0 to 1 or 0 to -1
	if (dy < dx)
	{
		//check if we need to draw from left to right or rigt to left
		if (p2.x < p1.x)
			DrawLine(p2, p1, color, 0);
		else
			DrawLine(p1, p2, color, 0);
	}
	//same as last one but for A bigger than 1 or lesser than -1
	else
	{
		if (p2.y < p1.y)
			DrawLine(p2, p1, color, 1);
		else
			DrawLine(p1, p2, color, 1);
	}
}



void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	int r = 100,a=2,half=180*r;
	
	for (int i = 0;i<360*r;i++)
	{
		
		ChangePoints(glm::ivec2(650, 400), glm::ivec2(650 + r*sin((360*i)/a), 400+ r * cos((360 * i) / a)), glm::ivec3(1, 1, 1));
		ChangePoints(glm::ivec2(250, 400), glm::ivec2(250 + r * sin((360 * i) / a), 400 + r * cos((360 * i) / a)), glm::ivec3(0, 0, 0));


	}
	ChangePoints(glm::ivec2(650,400), glm::ivec2(650,50), glm::ivec3(1, 1, 1));
	ChangePoints(glm::ivec2(650, 225), glm::ivec2(500, 200), glm::ivec3(1, 1, 1));
	ChangePoints(glm::ivec2(650, 200), glm::ivec2(500, 200), glm::ivec3(1, 1, 1));
	ChangePoints(glm::ivec2(250, 400), glm::ivec2(250, 50), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(250, 215), glm::ivec2(150, 300), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(250, 215), glm::ivec2(350, 300), glm::ivec3(0, 0, 0));


	for (int i = 0;i <25;i++)
	{
		ChangePoints(glm::ivec2(500, 200+i), glm::ivec2(485, 200+i), glm::ivec3(0, 0, 0));
		

	}
	for(int i=0;i<12;i++)
	ChangePoints(glm::ivec2(500, 225 - i), glm::ivec2(460, 225 - i), glm::ivec3(0, 0, 0));


	ChangePoints(glm::ivec2(1000,500), glm::ivec2(950, 400 ), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(1000, 500), glm::ivec2(1050, 400), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(950, 400), glm::ivec2(875, 350), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(1050, 400), glm::ivec2(1125, 350), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(875, 350), glm::ivec2(950,350), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(1125, 350), glm::ivec2(1050,350), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(950, 350), glm::ivec2(875, 300), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(1050, 350), glm::ivec2(1125, 300), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(875, 300), glm::ivec2(950, 300), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(1125, 300), glm::ivec2(1050, 300), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(950, 300), glm::ivec2(875, 250), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(1050, 300), glm::ivec2(1125, 250), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(875, 250), glm::ivec2(950, 250), glm::ivec3(0, 0, 0));
	ChangePoints(glm::ivec2(1125, 250), glm::ivec2(1050, 250), glm::ivec3(0, 0, 0));

	
	
	

		



}


int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}