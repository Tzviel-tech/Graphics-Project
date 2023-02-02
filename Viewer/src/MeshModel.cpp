#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName) :
	modelTransform(1),
	worldTransform(1),
	modelName(modelName)
{
	worldTransform = glm::mat4x4(1);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	color = glm::vec3(dist(mt), dist(mt), dist(mt));

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			
			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			vertex.normal = normals[vertexIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}

			modelVertices.push_back(vertex);
		}
	}


	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}
void MeshModel::PlanarTexture()
{
	for (Vertex& vertex : modelVertices)
	{
		vertex.textureCoords = glm::vec2(vertex.position.x, vertex.position.z);
	}

	glBindVertexArray(GetVAO());
	glBindBuffer(GL_VERTEX_ARRAY, GetVbO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, modelVertices.size() * sizeof(Vertex), &modelVertices[0]);
	glBindVertexArray(0);
}
MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const glm::vec3& MeshModel::GetColor() const
{
	return color;
}

void MeshModel::SetColor(const glm::vec3& color)
{
	this->color = color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}



GLuint MeshModel::GetVAO() const
{
	return vao;
}
GLuint MeshModel::GetVbO() const
{
	return vbo;
}

const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}
void MeshModel::setLocalTranslate()
{
	localtransmat = glm::translate(glm::mat4(1), glm::vec3(trans.x, trans.y, trans.z));
}

void MeshModel::setlocalScale()
{
	localscalemat = glm::scale(glm::mat4(1), glm::vec3(scalex * 0.1, scalex * 0.1, scalex * 0.1));
}
void MeshModel::setLocalRotation()
{
	glm::mat4 localrotationMatrixx = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 localrotationMatrixy = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 localrotationMatrixz = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	localrotation = localrotationMatrixz * localrotationMatrixy * localrotationMatrixx;
}
void MeshModel::setLocal()
{
	setLocalRotation();
	setlocalScale();
	setLocalTranslate();
	local = localtransmat * localscalemat * localrotation;

}
void MeshModel::setWorldTranslate()
{
	Wtransmat = glm::translate(glm::mat4(1), glm::vec3(transW.x, transW.y, transW.z));
}

void MeshModel::setWorldScale()
{
	Wscalemat = glm::scale(glm::mat4(1), glm::vec3(scalexW, scalexW, scalexW));
}
void MeshModel::setWorldRotation()
{
	glm::mat4  WrotationMatrixx = glm::rotate(glm::mat4(1.0f), glm::radians(rotateW.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4  WrotationMatrixy = glm::rotate(glm::mat4(1.0f), glm::radians(rotateW.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4  WrotationMatrixz = glm::rotate(glm::mat4(1.0f), glm::radians(rotateW.z), glm::vec3(0.0f, 0.0f, 1.0f));
	Wrotation = WrotationMatrixz * WrotationMatrixy * WrotationMatrixx;
}
void MeshModel::setWorld()
{
	setWorldRotation();
	setWorldScale();
	setWorldTranslate();
	world = Wtransmat * Wscalemat * Wrotation;

}





