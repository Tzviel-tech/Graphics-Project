#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <algorithm>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene&scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	bool drawnormals=false;
	bool drawboundingboxlocal = false;
	bool drawboundingboxworld = false;
	bool rectangle = false;
	bool show_Z = false;
	float scalenormal;
	void drawtrianglebox(std::vector<glm::vec3>triangle, glm::vec3 color);
	void checkminmax(glm::vec4& vertices);
	void edgewalking(std::vector<glm::vec3>triangle,Light l,MeshModel::material m);
	float grey = 1;
	void addlines(std::vector<glm::vec3>triangle, int flag,glm::vec3 color);
private:
class compare
{
	
public:
	compare() {};
	bool operator()(const glm::vec3& p1, const glm::vec3& p2)
	{
		return p1.y > p2.y;
	}
};
	void PutPixel(const int i, const int j,const int z, const glm::vec3& color);
	void DrawLine(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color, int flag);
	void ChangePoints(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color);
	void DrawLineZ(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color, int flag, std::vector<glm::vec3>tri);
	void PutPixelZ(int i, int j, float z, const glm::vec3& color);
	void ChangePointsZ(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color,std::vector<glm::vec3>tri);
	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();
	float trianglearea(float X0, float Y0, float X1, float Y1, float X2, float Y2);
	float calcZ(float a1, float a2, float a3, float z1, float z2, float z3);
	float minx = 1000000;
	float miny = 1000000;
	float maxx = -1000000;
	float maxy = -1000000;
	float maxz = -1000000;
	float minz = 1000000;
	
	float* color_buffer;
	float* z_buffer;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;
};
