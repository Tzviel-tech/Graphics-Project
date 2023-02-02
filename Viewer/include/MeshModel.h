#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Face.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
protected:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textureCoords;

	std::vector<Vertex> modelVertices;

	glm::mat4x4 modelTransform;
	glm::mat4x4 worldTransform;

	std::string modelName;

	glm::vec3 color;

	GLuint vbo;
	GLuint vao;

public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName = "");
	virtual ~MeshModel();
	void PlanarTexture();
	GLuint GetVbO() const;
	
	
	const glm::vec3& GetColor() const;
	void SetColor(const glm::vec3& color);

	const std::string& GetModelName();

	const std::vector<Vertex>& GetModelVertices();

	void setLocalTranslate();
	void setlocalScale();
	void setLocalRotation();
	void setWorld();
	void setWorldTranslate();
	void setWorldScale();
	void setWorldRotation();
	void setLocal();
	glm::mat4& getLocal() { setLocal();return local; };
	glm::mat4& getWorld() { setWorld();return world; };
	glm::mat4 getTransform() { return getWorld() * getLocal(); };
	//local running prameters
	float scalex = 1;
	glm::vec3 trans = glm::vec3(0.0f);
	glm::vec3 rotate = glm::vec3(0.0f);

	//world running pramaters
	float scalexW = 1;
	glm::vec3 transW = glm::vec3(0.0f);
	glm::vec3 rotateW = glm::vec3(0.0f);
	glm::mat4 local = glm::mat4(1);
	glm::mat4 world = glm::mat4(1);
	glm::mat4 localscalemat = glm::mat4(1);
	glm::mat4 localtransmat = glm::mat4(1);
	glm::mat4 localrotation = glm::mat4(1);
	glm::mat4 Wscalemat = glm::mat4(1);
	glm::mat4 Wtransmat = glm::mat4(1);
	glm::mat4 Wrotation = glm::mat4(1);
	
	GLuint GetVAO() const;
	
};
