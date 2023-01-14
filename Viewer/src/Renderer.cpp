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
	delete[] z_buffer;
}

void Renderer::PutPixel(int i, int j,int z, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;

	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}
 void Renderer::drawtrianglebox(std::vector<glm::vec3>triangle,glm::vec3 color)
{
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	

	for (auto& v : triangle) {
		minX = std::min(minX, v.x);
		minY = std::min(minY, v.y);
		maxX = std::max(maxX, v.x);
		maxY = std::max(maxY, v.y);
		
	}
	glm::vec3 top1 = glm::vec3(maxX, maxY,1);
	glm::vec3 top2 = glm::vec3(minX, maxY,1);
	glm::vec3 bot1 = glm::vec3(maxX, minY,1);
	glm::vec3 bot2 = glm::vec3(minX, minY,1);


	ChangePoints(top1, top2, color);
	ChangePoints(top1, bot1, color);
	ChangePoints(bot1, bot2, color);
	ChangePoints(bot2, top2, color);
	


}
 float Renderer:: calcZ(float a1, float a2, float a3, float z1, float z2, float z3)
 {
	 float all = a1 + a2 + a3;
	 return (a1 / all) * z1 + (a2 / all) * z2 + (a3 / all) * z3;
 }
 float Renderer::trianglearea(float X0, float Y0, float X1, float Y1, float X2, float Y2)
 {
	
	 float area = (X0 * (Y1 - Y2) + X1 * (Y2 - Y0) + X2 * (Y0 - Y1)) / 2.0;
	 return (area > 0.0) ? area : -area;
 }
 

void Renderer::DrawLine(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color, int flag)
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
	while (start_point <= end_point)
	{
		if (e > 0)
		{
			point_toadd = point_toadd + to_add;
			e = e - 2 * delta_1;
		}
		//check that we put pixel correctly(x should be first cordinate and y second)
		if (!flag)
		{
		    
			PutPixel(start_point, point_toadd,p1.z, color);

		}
		else
		{
			PutPixel(point_toadd, start_point,p1.z, color);

		}

		e = e + 2 * delta_2;
		start_point++;
	}
}

void Renderer::ChangePoints(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color)
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
void Renderer::PutPixelZ(int i, int j, float z, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;
	

	if (z>=z_buffer[INDEX(viewport_width, i, j, 0)])
	{
		z_buffer[INDEX(viewport_width, i, j, 0)] = z;
		z_buffer[INDEX(viewport_width, i, j, 1)] = z;
		z_buffer[INDEX(viewport_width, i, j, 2)] = z;
		PutPixel(i, j, z, color);
		
    }
	
	
}

void Renderer::DrawLineZ(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color, int flag,std::vector<glm::vec3>tri)
{

	int x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int start_point, end_point, point_toadd, delta_1, delta_2;
	float a1, a2, a3, d;
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
	while (start_point <= end_point)
	{
		if (e > 0)
		{
			point_toadd = point_toadd + to_add;
			e = e - 2 * delta_1;
		}
		//check that we put pixel correctly(x should be first cordinate and y second)
		if (!flag)
		{
			a1 = trianglearea(tri[1].x, tri[1].y, tri[2].x, tri[2].y, start_point, point_toadd);
			a2 = trianglearea(tri[0].x, tri[0].y, tri[2].x, tri[2].y, start_point, point_toadd);
			a3 = trianglearea(tri[1].x, tri[1].y, tri[0].x, tri[0].y, start_point, point_toadd);
			d = calcZ(a1, a2, a3, tri[0].z, tri[1].z, tri[2].z);
			glm::vec3 normal = normalInter(norn, tri, start_point, point_toadd);
			PutPixelZ(start_point, point_toadd, d, Color(normal,glm::vec3(start_point,point_toadd,d), tri, normal, curs));
			//PutPixelZ(start_point, point_toadd, d, color);
		}
		else
		{
			a1 = trianglearea(tri[1].x, tri[1].y, tri[2].x, tri[2].y, point_toadd, start_point);
			a2 = trianglearea(tri[0].x, tri[0].y, tri[2].x, tri[2].y, point_toadd, start_point);
			a3 = trianglearea(tri[1].x, tri[1].y, tri[0].x, tri[0].y, point_toadd, start_point);
			d = calcZ(a1, a2, a3, tri[0].z, tri[1].z, tri[2].z);
			glm::vec3 normal = normalInter(norn, tri, point_toadd, start_point);
			PutPixelZ(point_toadd, start_point, d, Color(normal,glm::vec3(point_toadd,start_point,d), tri, normal, curs));
			//(point_toadd, start_point,d, color);
		
		}

		e = e + 2 * delta_2;
		start_point++;
	}
}

void Renderer::ChangePointsZ(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color,std::vector<glm::vec3>tri)
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
			DrawLineZ(p2, p1, color, 0,tri);
		else
			DrawLineZ(p1, p2, color, 0, tri);
	}
	//same as last one but for A bigger than 1 or lesser than -1
	else
	{
		if (p2.y < p1.y)
			DrawLineZ(p2, p1, color, 1, tri);
		else
			DrawLineZ(p1, p2, color, 1, tri);
	}
}



void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	z_buffer = new float[3 * w * h];
	for (int i = 0;i < 3 * w * h;i++)
	{
		z_buffer[i]= -1000000.00f;
	}
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
			PutPixel(i, j,1, color);
		}
	}
	for (int i = 0;i < 3 * viewport_height * viewport_width;i++)
	{
		z_buffer[i] = -1000000.00f;
	}
}

void Renderer:: addlines(std::vector<glm::vec3>triangle,int flag,glm::vec3 color)
{
	
	//glm::vec3 color(1, 0, 0);
	float deltax1, deltax2,startpoint,endpoint,d1=1,d2=1;
	int y1, y2;
	if (flag)
	{
		deltax1 = -1*(triangle[1].x - triangle[0].x) / (triangle[1].y - triangle[0].y);
		deltax2 = -1*(triangle[2].x - triangle[0].x) / (triangle[2].y - triangle[0].y);
		startpoint = endpoint = triangle[0].x;
		y1 = triangle[0].y;
		y2 = triangle[2].y;
	
	}
	if (!flag)
	{
		deltax1 = (triangle[2].x - triangle[0].x) / (triangle[2].y - triangle[0].y);
		deltax2 = (triangle[2].x - triangle[1].x) / (triangle[2].y - triangle[1].y);
		startpoint = endpoint = triangle[2].x;
		y1 = triangle[2].y;
		y2 = triangle[0].y;
		
	}
	
	while (y2 != y1)
	{
		ChangePointsZ(glm::vec3(startpoint, y1,1), glm::vec3(endpoint, y1,1), color, triangle);
		if (flag)
		{
			y1=y1-1.f;
		}
		else
			y1++;

		startpoint+=deltax1;
		endpoint+=deltax2;
	}
	
	

	return;
}
glm::vec3 Renderer:: normalInter(std::vector<glm::vec3>& Normals, std::vector<glm::vec3>& Pos, float px, float py)
{
	float A3 = trianglearea(Pos.at(0).x, Pos.at(0).y, Pos.at(1).x, Pos.at(1).y, px, py);
	float A2 = trianglearea(Pos.at(0).x, Pos.at(0).y, Pos.at(2).x, Pos.at(2).y, px, py);
	float A1 = trianglearea(Pos.at(1).x, Pos.at(1).y, Pos.at(2).x, Pos.at(2).y, px, py);

	float A = A1 + A2 + A3;
	weight = glm::vec3(A1 / A, A2 / A, A3 / A);
		glm::vec3 Normal = (A1 / A)* Normals.at(0) + (A2 / A) * Normals.at(1) +(A3 /A) * Normals.at(2);
	return Normal;
}
glm::vec3 Renderer::Color(glm::vec3 nprojnor, glm::vec3 point, std::vector<glm::vec3>& triangle, glm::vec3 facenormal, Scene s)

{
	glm::vec3 c = point;
	glm::normalize(point);
	facenormal = glm::normalize(nprojnor);
	MeshModel::material &m = s.GetActiveModel().m;
	Light &l = s.GetLight(0);
	MeshModel& model = s.GetActiveModel();

	// Ambient:
	glm::vec3 color1 = glm::vec3(1.f, 1.0f, 1.0);
	glm::vec3 ambient = m.ambient_ * l.GetAmbient() * color1;

	//Diffuse:
	glm::vec3 diffuseStrength = m.diffuse_;
	glm::vec3 lightDir = glm::normalize(l.position-c);
	

	float diff = fmax(glm::dot(facenormal, lightDir), 0.0);
	
	glm::vec3 diffuse = diffuseStrength * diff * color1;
	//Specular:
    glm::vec3 specularStrength = m.specular_;
	glm::vec3 viewDir = glm::normalize(glm::vec3(0,0,1) - c);
	glm::vec3 reflectDir = glm::normalize(glm::reflect(-lightDir, facenormal));


	float spec = pow(fmax(glm::dot(viewDir, reflectDir), 0.0), 32);
	glm::vec3 specular = specularStrength * spec * color1;
	glm::vec3 color = l.Ascale * ambient + l.Dscale * diffuse + l.Sscale * specular;



	return color;
}
bool Renderer:: insidetri(std::vector<glm::vec3>& tri,int x,int y)
{
	glm::vec3 v1 = tri[0];
	glm::vec3 v2 = tri[1];
	glm::vec3 v3 = tri[2];
	float deltax = v2.x - v1.x;
	float deltay = v2.y - v1.y;
	float deltaz = v2.z - v1.z;
	float deltax1 = v3.x - v1.x;
	float deltay2 = v3.y - v1.y;
	float deltaz2 = v3.z - v1.z;
	float a = deltay * deltaz2 - deltay2 * deltaz;
	float b = deltax1 * deltaz - deltax * deltaz2;
	float c = deltax * deltay2 - deltay * deltax1;
	float d = (-a * v1.x - b * v1.y - c * v1.z);
	

	glm::vec2 vv1 = glm::vec2(v2.x - v1.x, v2.y - v1.y);
	glm::vec2 vv2 = glm::vec2(v3.x - v1.x, v3.y - v1.y);
	glm::vec2 m = glm::vec2(x - v1.x, y - v1.y);
	float aa = (m.x * vv2.y - m.y * vv2.x) / (vv1.x * vv2.y - vv1.y * vv2.x);
	float bb = (vv1.x * m.y - vv1.y * m.x) / (vv1.x * vv2.y - vv1.y * vv2.x);
	if ((aa >= 0) && (bb >= 0) && (aa + bb <= 1))
		return true;
	return false;
}
void Renderer::edgewalking(glm::vec3 nprojnor,std::vector<glm::vec3>&noproj,std::vector<glm::vec3>&triangle,glm::vec3 facenormal,Scene s,float& dif)
{
	glm::vec3 color = Color(nprojnor,triangle[0], triangle, facenormal, s);
	std::sort(triangle.begin(),triangle.end(),compare());
	if (fabs(triangle[1].y-triangle[2].y)<DBL_EPSILON)
		addlines(triangle, 1,color);
	else if(fabs(triangle[0].y-triangle[1].y)<DBL_EPSILON)
		addlines(triangle, 0,color);
	else
	{
	glm::vec3 cutpoint;
	
	cutpoint.y = triangle[1].y;
	cutpoint.x = triangle[0].x+(((triangle[1].y - triangle[0].y) / (triangle[2].y - triangle[0].y)) * (triangle[2].x - triangle[0].x));
	float a1, a2, a3, d;
	a1 = trianglearea(triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, cutpoint.x, cutpoint.y);
	a2 = trianglearea(triangle[0].x, triangle[0].y, triangle[2].x, triangle[2].y, cutpoint.x, cutpoint.y);
	a3 = trianglearea(triangle[1].x, triangle[1].y, triangle[0].x, triangle[0].y, cutpoint.x, cutpoint.y);
	d = calcZ(a1, a2, a3, triangle[0].z, triangle[1].z, triangle[2].z);
	cutpoint.z = d;
	std::vector<glm::vec3> tri1{ triangle[0],triangle[1],cutpoint};
	std::vector<glm::vec3> tri2{ triangle[1],cutpoint,triangle[2]};
	glm::vec3 temp = norn[2];
	std::vector<glm::vec3>copy = norn;
	glm::vec3 newnormal = glm::normalize(normalInter(norn, triangle, cutpoint.x, cutpoint.y));
	norn[2] = newnormal;
	addlines(tri1, 1,color);
	norn = { norn[1],newnormal,temp};
	addlines(tri2, 0,color);
	norn = copy;
	ChangePointsZ(triangle[1], cutpoint, color,triangle);
	norn = copy;
	}
	
	ChangePointsZ(triangle[0], triangle[1], color, triangle);
	ChangePointsZ(triangle[0], triangle[2],color, triangle);
	ChangePointsZ(triangle[2], triangle[1],color, triangle);

}
void Renderer::Shadetriangle(std::vector<glm::vec3>& tri, Scene& scene, glm::vec3 lightPoint, std::vector <glm::vec3> Normals)
{
	glm::vec3 v1 = tri[0];
	glm::vec3 v2 = tri[1];
	glm::vec3 v3 = tri[2];
	int maxX = fmax(v1.x, fmax(v2.x, v3.x));
	int minX = fmin(v1.x, fmin(v2.x, v3.x));
	int maxY = fmax(v1.y, fmax(v2.y, v3.y));
	int minY = fmin(v1.y, fmin(v2.y, v3.y));
	float z;
	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			//check point inside triangle used (det(v,v1)-det(v0,v1))/det(v1,v2) when det==ux*vy-uy*vx
			if (insidetri(tri, x, y))
			{
				//find z by interpolation 
				float a1 = trianglearea(tri[1].x, tri[1].y, tri[2].x, tri[2].y, x, y);
				float a2 = trianglearea(tri[0].x, tri[0].y, tri[2].x, tri[2].y, x, y);
				float a3 = trianglearea(tri[1].x, tri[1].y, tri[0].x, tri[0].y, x, y);
				if (x > 0 && y > 0 && y < viewport_height && x < viewport_width)
				{
					z= calcZ(a1, a2, a3, tri[0].z, tri[1].z, tri[2].z);
					if (z >= z_buffer[INDEX(viewport_width, x, y, 0)])
					{
						z_buffer[INDEX(viewport_width, x, y, 0)] = z;
						z_buffer[INDEX(viewport_width, x, y, 1)] = z;
						z_buffer[INDEX(viewport_width, x, y, 2)] = z;
						glm::vec3 Normal = glm::normalize(normalInter(Normals, tri, x, y));
						glm::vec3 color = Color(Normal, glm::vec3(x, y, z),tri,Normal,curs);
						PutPixel(x, y,1.f, color);

					}


				}
			}
		}
	}
}
void Renderer::Render( Scene& scene)
{
	
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
	Scene &s = scene;
	Camera camera = s.GetCamera(0);
	camera.SetCameraLookAt(glm::vec3(camera.a, camera.b, camera.c), glm::vec3(0, 0,0), glm::vec3(0, 1, 0));
	
	MeshModel &mod = scene.GetModel(0);
	MeshModel::material matiral = mod.m;
	Light &l = scene.GetLight(0);
	glm::mat4 matrix = mod.getTransform();
	centerM = mod.center();
	std::vector<glm::vec3>normals = mod.getNormals();
	std::vector<glm::vec4>facenormals;
	std::vector<glm::vec3> vec = mod.GetVertecies();
	glm::mat4 finaltran =  camera.GetProjectionTransformation() * camera.GetViewTransformation() * matrix;
	std::vector<vector<glm::vec3>>alltri;
	
	for (int i = 0;i < mod.GetFacesCount();i++)
	{
		
		int a = mod.GetFace(i).GetVertexIndex(0);
		int b = mod.GetFace(i).GetVertexIndex(1);
		int c = mod.GetFace(i).GetVertexIndex(2);
		glm::vec4 p1 = glm::vec4(vec.at(a - 1), 1.f);
		glm::vec4 p2 = glm::vec4(vec.at(b - 1), 1.f);
		glm::vec4 p3 = glm::vec4(vec.at(c - 1), 1.f);
		int a11 = mod.GetFace(i).GetNormalIndex(0);
		int b1 = mod.GetFace(i).GetNormalIndex(1);
		int c1 = mod.GetFace(i).GetNormalIndex(2);
		glm::vec4 p1n = glm::vec4(normals.at(a11-1),1.f);
		glm::vec4 p2n = glm::vec4(normals.at(b1 - 1), 1.f);
		glm::vec4 p3n = glm::vec4(normals.at(c1 - 1), 1.f);
		glm::mat4 k = glm::transpose(glm::inverse(matrix));
		p1n = k *p1n;
		p2n = k*p2n;
		p3n = k*p3n ;
		p1n /= p1n.w;
		p2n /= p2n.w;
		p2n /= p2n.w;
		
		glm::normalize(p1n);
		glm::normalize(p2n);
		glm::normalize(p3n);
		
		curs = scene;


		glm::vec4 centerF = p1 + p2 + p3;
		mod.center();
		glm::vec4 p1noproject = matrix * p1;
		glm::vec4 p2noproject = matrix * p2;
		glm::vec4 p3noproject = matrix * p3;

		std::vector<glm::vec3>noproject{p1noproject,p2noproject,p3noproject};
		
		glm::vec4 templ = glm::vec4(l.position, 1.f);
		glm::vec3 save = l.position;
		templ = camera.GetProjectionTransformation() * camera.GetViewTransformation() * templ;
		templ /= templ.w;
		templ.x = (templ.x * half_width) + half_width;
		templ.y = (templ.y * half_height) + half_height;
		l.position = templ;
		centerF.x /= 3;
		centerF.y /= 3;
		centerF.z /= 3;
		centerF.w = 1;
		//vertex normal
		float r = 22.f;
		float a22 = 50.f;
		for (int i = 0;i < 1000;i++)
		{
			ChangePoints(templ, glm::vec3(templ.x + r * sin((360 * i) / a22),
				templ.y + r * cos((360 * i) / a22), 1.f), glm::ivec3(1, 1, 1));


		}
		normalx = glm::vec4(glm::cross(vec[b - 1] - vec[a - 1], vec[c - 1] - vec[a - 1]), 1.f);
		normaly = glm::vec4(glm::cross(vec[b - 1] - vec[a - 1], vec[c - 1] - vec[a - 1]), 1.f);
		
		glm::normalize(normalx);
		glm::vec4 centerf = centerF;
		//scale normal while keep his direction
	/*	normalx = glm::vec4(glm::vec3(p1.x + scalenormal * (normalx.x - p1.x),
			p1.y + scalenormal * (normalx.y - p1.y),
			p1.z + scalenormal * (normalx.z - p1.z)), 1.f);*/
		normalx.w = 1;
		glm::vec3 nornoproj = matrix * normalx;
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
		normalx.x = (normalx.x * half_width) + half_width;
		normalx.y = (normalx.y * half_height) + half_height;
		normaly.x /= normaly.w;
		normaly.y /= normaly.w;
		normaly.z /= normaly.w;
		normaly.x = (normaly.x * half_width) + half_width;
		normaly.y = (normaly.y * half_height) + half_height;
		p1.x = p1.x * half_width + half_width;
		p1.y = p1.y * half_height + half_height;
		p2.x = p2.x * half_width + half_width;
		p2.y = p2.y * half_height + half_height;
		p3.x = p3.x * half_width + half_width;
		p3.y = p3.y * half_height + half_height;
		norn = { p1n,p2n,p3n };
		centerF.x = (centerF.x * half_width) + half_width;
		centerF.y = (centerF.y * half_height) + half_height;
		checkminmax(p1);
		checkminmax(p2);
		checkminmax(p3);
		float dif;
		std::vector <glm::vec3>tri{ p1,p2,p3 };
		alltri.push_back(tri);
		//draw triangle;
     //	edgewalking(normalx,tri,tri,normalx,scene,dif);
		Shadetriangle(tri, curs, l.position, norn);
		l.position = save;
		/*glm::vec4 lightdir = glm::vec4(tri[3], 1.f);
		glm::vec4 reflectlight = glm::vec4(tri[4], 1.f);
		lightdir += centerf;
		reflectlight += centerf;
		
		lightdir= glm::vec4(glm::vec3(centerf.x + scalenormal * (lightdir.x - centerf.x),
			centerf.y + scalenormal * (lightdir.y - centerf.y),
			centerf.z + scalenormal * (lightdir.z - centerf.z)), 1.f);

		reflectlight = glm::vec4(glm::vec3(centerf.x + scalenormal * (reflectlight.x - centerf.x),
			centerf.y + scalenormal * (reflectlight.y - centerf.y),
			centerf.z + scalenormal * (reflectlight.z - centerf.z)), 1.f);
		reflectlight = finaltran * reflectlight;
		lightdir = finaltran * lightdir;
		lightdir /= lightdir.w;
		reflectlight /= reflectlight.w;
		reflectlight.x = (reflectlight.x * half_width) + half_width;
		reflectlight.y = (reflectlight.y * half_height) + half_height;
		lightdir.x = (lightdir.x * half_width) + half_width;
		lightdir.y = (lightdir.y * half_height) + half_height;*/


		//draw normals
		if (drawnormals)
		{
		  ChangePoints(p1, normalx, glm::vec3(1, 1, 0));
	      ChangePoints(centerF, normaly, glm::vec3(0, 0, 1));
		}
		/*if (drawlightray&&dif!=0.0)
		{
			ChangePoints(centerF, lightdir, glm::vec3(1, 1, 0));
			ChangePoints(centerF, reflectlight, glm::vec3(0, 0, 1));
		}
	
	*/
		
	}
	if (show_Z)
	{
		float min = 10000000;
		float max = -1000000;
		for (int i = 0;i < 3 * viewport_height * viewport_width;i++)
		{
			if (min > z_buffer[i] && z_buffer[i] > -100000.f)
				min = z_buffer[i];
			if (max < z_buffer[i] && z_buffer[i] > -100000.f)
				max = z_buffer[i];
		}

		for (int i = 0;i < 3 * viewport_height * viewport_width;i++)
		{
			if (z_buffer[i] > -100000.f)
			{
				if (max > 0)
					color_buffer[i] = (z_buffer[i] - min) / (max);
				else
					color_buffer[i] = (z_buffer[i] - min) / (-1.f * max);
			}
		}
	}
	if (rectangle)
	{
		for (auto& vec : alltri)
		{
			float avrage = (vec[0].z + vec[1].z + vec[2].z) / 3.f;
			glm::vec3 color  (avrage / (maxz+minz), avrage / (maxz + minz), avrage / (maxz + minz));
			drawtrianglebox(vec, color);
		}
		
	}

	//model asexs
	/*glm::vec4 x11(centerM.x , mod.minY, 1, 1);
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
	ChangePoints(x33, x44, glm::vec3(1, 1, 1));*/
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