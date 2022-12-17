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
	 minx = 1000000;
	 miny = 1000000;
	 maxx = -1000000;
	 maxy = -1000000;
	maxz = -1000000;
	 minz = 1000000;
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	glm::vec4 viewportvec(half_width, half_height, 0, 0);
	glm::vec4 normalx, normaly, normalz,facenormal;
	glm::vec3 y1(half_width, viewport_height, 0), y2(half_width, 0, 0), x1(0, half_height, 0), x2(viewport_width, half_height, 0);
	ChangePoints(y1, y2, glm::vec3(0, 0, 0)), ChangePoints(x1, x2, glm::vec3(0, 0, 0));
	glm::vec4 centerM;
	Scene s = scene;
	Camera camera = s.GetCamera(0);
	camera.SetCameraLookAt(glm::vec3(camera.a, camera.b, camera.c), glm::vec3(0, 0,0), glm::vec3(0, 1, 0));
	
	MeshModel mod = scene.GetModel(0);
	glm::mat4 matrix = mod.getTransform();
	centerM = mod.center();
	std::vector<glm::vec3>normals = mod.getNormals();
	std::vector<glm::vec4>facenormals;
	std::vector<glm::vec3> vec = mod.GetVertecies();
	glm::mat4 finaltran =  camera.GetProjectionTransformation() * camera.GetViewTransformation() * matrix;
	
	
	for (int i = 0;i < mod.GetFacesCount();i++)
	{   
		int a = mod.GetFace(i).GetVertexIndex(0);
		int b = mod.GetFace(i).GetVertexIndex(1);
		int c = mod.GetFace(i).GetVertexIndex(2);
		glm::vec4 p1 = glm::vec4(vec.at(a - 1),1.f);
		glm::vec4 p2 = glm::vec4(vec.at(b - 1), 1.f);
		glm::vec4 p3 = glm::vec4(vec.at(c - 1), 1.f);
		glm::vec4 centerF = p1 + p2 + p3;
		centerF.x /= 3;
		centerF.y /= 3;
		centerF.z /= 3;
		centerF.w = 1;
		//vertex normal
		normalx = glm::vec4(glm::cross(vec[b - 1] - vec[a - 1], vec[c - 1] - vec[a - 1]), 1.f);
		normaly = glm::vec4(glm::cross(vec[b - 1] - vec[a - 1], vec[c - 1] - vec[a - 1]), 1.f);
		normalx += p1;
		glm::normalize(normalx);
		//scale normal while keep his direction
		normalx = glm::vec4(glm::vec3(p1.x + scalenormal * (normalx.x - p1.x),
			p1.y + scalenormal * (normalx.y - p1.y),
			p1.z + scalenormal * (normalx.z - p1.z)),1.f);
		normalx.w = 1;
		normalx = finaltran * normalx;
		//face normal
		normaly += centerF;
		glm::normalize(normaly);
		//scale normal while keep his direction
		normaly = glm::vec4(glm::vec3(centerF.x + scalenormal * (normaly.x - centerF.x),
			centerF.y + scalenormal * (normaly.y - centerF.y),
			centerF.z + scalenormal * (normaly.z - centerF.z)), 1.f);
		normaly.w = 1;
		normaly = finaltran * normaly;
		//center of triangle
		centerF = finaltran * centerF;
		centerF.x /= centerF.w;
		centerF.y /= centerF.w;
		centerF.z /= centerF.w;

		p1 = finaltran * p1;
		p2 = finaltran * p2;
		p3 = finaltran * p3;
		
		p1 /= p1.w;
		p2 /= p2.w;
		p3 /= p3.w;
		
	    //viewport transform
		normalx.x /= normalx.w;
		normalx.y /= normalx.w;
		normalx.z /= normalx.w;
		normalx.x = (normalx.x*half_width)+half_width;
		normalx.y = (normalx.y* half_height)+half_height;
		normaly.x /= normaly.w;
		normaly.y /= normaly.w;
		normaly.z /= normaly.w;
		normaly.x = (normaly.x * half_width) + half_width;
		normaly.y = (normaly.y * half_height) + half_height;
		p1.x =p1.x*half_width+half_width;
		p1.y =p1.y*half_height +half_height;
		p2.x =p2.x*half_width + half_width;
		p2.y =p2.y* half_height +half_height;
		p3.x = p3.x * half_width +half_width;
		p3.y = p3.y*half_height +half_height;
		centerF.x = (centerF.x*half_width)+half_width;
		centerF.y = (centerF.y*half_height)+half_height;
		checkminmax(p1);
		checkminmax(p2);
		checkminmax(p3);
		
	    //draw triangle
		ChangePoints(p1, p2, glm::vec3(1, 0, 0));
		ChangePoints(p1, p3, glm::vec3(1, 0, 0));
		ChangePoints(p3, p2, glm::vec3(1, 0, 0));
		//draw normals
		if (drawnormals)
		{
		  ChangePoints(p1, normalx, glm::vec3(1, 1, 0));
	      ChangePoints(centerF, normaly, glm::vec3(0, 0, 1));
		}
		
	}
	//model asexs
	glm::vec4 x11(centerM.x , mod.minY, 1, 1);
	glm::vec4 x22(centerM.x, mod.maxY , 1, 1);
	glm::vec4 x33(mod.minX, centerM.y, 1, 1);
	glm::vec4 x44(mod.maxX, centerM.y, 1, 1);
	x22 = camera.GetProjectionTransformation() * camera.GetViewTransformation()*mod.getWorld() * mod.getsacle() * x22;
	x11 = camera.GetProjectionTransformation() * camera.GetViewTransformation()* mod.getWorld() * mod.getsacle() * x11;
	x22 /= x22.w;
	x11 /= x11.w;
	x22.x = x22.x*half_width+ half_width;
	x11.x = x11.x * half_width + half_width;
	x22.y =x22.y*half_height+ half_height;
	x11.y = x11.y * half_height + half_height;
	x33 = camera.GetProjectionTransformation() * camera.GetViewTransformation() * mod.getWorld()*mod.getsacle() * x33;
	x44 = camera.GetProjectionTransformation() * camera.GetViewTransformation() * mod.getWorld() * mod.getsacle() * x44;
	x33 /= x33.w;
	x44 /= x44.w;
	x33.x = x33.x * half_width + half_width;
	x44.x = x44.x * half_width + half_width;
	x33.y = x33.y * half_height + half_height;
	x44.y = x44.y * half_height + half_height;
	ChangePoints(x11,x22, glm::vec3(1, 1, 1));
	ChangePoints(x33, x44, glm::vec3(1, 1, 1));
	//bounding box
	if (drawboundingboxlocal)
	{
		
		glm::vec4 top1 = finaltran*glm::vec4(mod.maxX, mod.maxY, mod.minZ, 1.f);
		glm::vec4 top2 = finaltran * glm::vec4(mod.maxX, mod.maxY, mod.maxZ, 1.f);
		glm::vec4 top3 = finaltran * glm::vec4(mod.minX, mod.maxY, mod.minZ, 1.f);
		glm::vec4 top4 = finaltran * glm::vec4(mod.minX, mod.maxY, mod.maxZ, 1.f);
		glm::vec4 bottom1 = finaltran * glm::vec4(mod.minX, mod.minY, mod.maxZ, 1.f);
		glm::vec4 bottom2 = finaltran * glm::vec4(mod.maxX, mod.minY, mod.maxZ, 1.f);
		glm::vec4 bottom3 = finaltran * glm::vec4(mod.minX, mod.minY, mod.minZ, 1.f);
		glm::vec4 bottom4 = finaltran * glm::vec4(mod.maxX, mod.minY, mod.minZ, 1.f);
		top1 /= top1.w;
		top2 /= top2.w;
		top3 /= top3.w;
		top4 /= top4.w;
		top1.x= top1.x * half_width + half_width;
		top1.y = top1.y * half_height + half_height;
		top2.x = top2.x * half_width + half_width;
		top2.y = top2.y * half_height + half_height;
		top3.x = top3.x * half_width + half_width;
		top3.y = top3.y * half_height + half_height;
		top4.x = top4.x * half_width + half_width;
		top4.y = top4.y * half_height + half_height;
		bottom1 /= bottom1.w;
		bottom2 /= bottom2.w;
		bottom3 /= bottom3.w;
		bottom4 /= bottom4.w;
		bottom1.x = bottom1.x * half_width + half_width;
		bottom1.y = bottom1.y * half_height + half_height;
		bottom2.x = bottom2.x * half_width + half_width;
		bottom2.y = bottom2.y * half_height + half_height;
		bottom3.x = bottom3.x * half_width + half_width;
		bottom3.y = bottom3.y * half_height + half_height;
		bottom4.x = bottom4.x * half_width + half_width;
		bottom4.y = bottom4.y * half_height + half_height;
	    
		
		//draw bounding box
		ChangePoints(top1, bottom4, glm::vec3(1, 1, 1));
		ChangePoints(top1, top2, glm::vec3(1, 1, 1));
		ChangePoints(top4, top3, glm::vec3(1, 1, 1));
		ChangePoints(top1, top3, glm::vec3(1, 1, 1));
		ChangePoints(top2, bottom2, glm::vec3(1, 1, 1));
		ChangePoints(top2, top4, glm::vec3(1, 1, 1));
		ChangePoints(top3, bottom3, glm::vec3(1, 1, 1));
		ChangePoints(top4, bottom1, glm::vec3(1, 1, 1));
		ChangePoints(bottom1, bottom2, glm::vec3(1, 1, 1));
		ChangePoints(bottom1, bottom3, glm::vec3(1, 1, 1));
		ChangePoints(bottom3, bottom4, glm::vec3(1, 1, 1));
		ChangePoints(bottom2, bottom4, glm::vec3(1, 1, 1));
	}
	if (drawboundingboxworld)
	{

		glm::vec4 top1 =  glm::vec4(maxx, maxy, minz, 1.f);
		glm::vec4 top2 =  glm::vec4(maxx, maxy, maxz, 1.f);
		glm::vec4 top3 =  glm::vec4(minx, maxy, minz, 1.f);
		glm::vec4 top4 = glm::vec4(minx, maxy, maxz, 1.f);
		glm::vec4 bottom1 = glm::vec4(minx, miny, maxz, 1.f);
		glm::vec4 bottom2 =  glm::vec4(maxx, miny, maxz, 1.f);
		glm::vec4 bottom3 =  glm::vec4(minx, miny, minz, 1.f);
		glm::vec4 bottom4 =  glm::vec4(maxx, miny, minz, 1.f);
		

		//draw bounding box
		ChangePoints(top1, bottom4, glm::vec3(1, 1, 1));
		ChangePoints(top1, top2, glm::vec3(1, 1, 1));
		ChangePoints(top4, top3, glm::vec3(1, 1, 1));
		ChangePoints(top1, top3, glm::vec3(1, 1, 1));
		ChangePoints(top2, bottom2, glm::vec3(1, 1, 1));
		ChangePoints(top2, top4, glm::vec3(1, 1, 1));
		ChangePoints(top3, bottom3, glm::vec3(1, 1, 1));
		ChangePoints(top4, bottom1, glm::vec3(1, 1, 1));
		ChangePoints(bottom1, bottom2, glm::vec3(1, 1, 1));
		ChangePoints(bottom1, bottom3, glm::vec3(1, 1, 1));
		ChangePoints(bottom3, bottom4, glm::vec3(1, 1, 1));
		ChangePoints(bottom2, bottom4, glm::vec3(1, 1, 1));
	}

	


}
int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}

void Renderer::checkminmax(glm::vec4& vertices)
{
	if (vertices.x < minx)
		minx = vertices.x;
	if (vertices.x > maxx)
		maxx = vertices.x;
	if (vertices.y < miny)
		miny = vertices.y;
	if (vertices.y > maxy)
		maxy = vertices.y;
	if (vertices.z < minz)
		minz = vertices.z;
	if (vertices.z > maxz)
		maxz = vertices.z;
}