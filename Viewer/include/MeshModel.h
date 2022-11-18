#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include <memory>
#include "glm/gtx/string_cast.hpp"
#include <iostream>
class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	std::vector<glm::vec3>& GetVertecies();
	const std::string& GetModelName() const;
	void setLocalTranslate();
	void setlocalScale();
	void setLocalRotation();
	void setLocal();
	glm::mat4& getLocal() { return local; };
	
	friend std::ostream& operator<<(std::ostream& out, MeshModel &thi)
	{
		out << "Model is: " << thi.GetModelName() << "\n";
		std::vector<glm::vec3>vec = thi.GetVertecies();
		for (auto a : vec)
			out << glm::to_string(a) << "\n";
		for (int i = 0;i < thi.GetFacesCount();i++)
		{
			out << "face" << i+1 << " : ";
			for (int j = 0;j < 3;j++)
				out << thi.GetFace(i).GetVertexIndex(j) << " ";
			out << "\n";
		}
		
		return out;
	}
	float scalex=0;
	float translatex=0;
	float translatey=0;
	float translatez=0;
	float rotatex=0;
	float rotatey=0;
	float rotatez=0;
private:
	glm::mat4 local=glm::mat4(1);
	glm::mat4 world = glm::mat4(1);
	glm::mat4 localscalemat = glm::mat4(1);
	glm::mat4 localtransmat = glm::mat4(1);
	glm::mat4 localrotationMatrixx = glm::mat4(1);
	glm::mat4 localrotationMatrixy = glm::mat4(1);
	glm::mat4 localrotationMatrixz = glm::mat4(1);
	glm::mat4 localrotation = glm::mat4(1);
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
	
	
	
};
