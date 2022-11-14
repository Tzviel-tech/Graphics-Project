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
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
};
