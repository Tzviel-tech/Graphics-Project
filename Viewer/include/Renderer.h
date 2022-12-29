#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

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
	float scalenormal;
	void drawtrianglebox(std::vector<glm::vec3>triangle, glm::vec3 color);
	void checkminmax(glm::vec4& vertices);

private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color, int flag);
	void ChangePoints(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();
	
	float minx = 1000000;
	float miny = 1000000;
	float maxx = -1000000;
	float maxy = -1000000;
	float maxz = -1000000;
	float minz = 1000000;
	float* color_buffer;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;
};
