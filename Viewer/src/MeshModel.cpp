#include "MeshModel.h"
#include "glm/gtx/string_cast.hpp"
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
	,
	model_name(model_name)
{

}
void MeshModel::setLocalTranslate()
{
	localtransmat= glm::translate(glm::mat4(1), glm::vec3(trans.x, trans.y, trans.z));
}

void MeshModel::setlocalScale()
{
	localscalemat=glm::scale(glm::mat4(1), glm::vec3(scalex*0.1, scalex*0.1, scalex*0.1));
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

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}
std::vector<glm::vec3>& MeshModel:: GetVertecies()
{
	return vertices;
}

glm::vec4 MeshModel::center()
{
	    float minx = vertices[0].x;
		float miny = vertices[0].y;
		float maxx = vertices[0].x;
		float maxy = vertices[0].x;
		float maxz = vertices[0].z;
		float minz = vertices[0].z;
		for (int i = 0;i < vertices.size();i++)
		{
			if (vertices[i].x < minx)
				minx = vertices[i].x;
			if (vertices[i].x > maxx)
				maxx = vertices[i].x;
			if (vertices[i].y < miny)
				miny = vertices[i].y;
			if (vertices[i].y > maxy)
				maxy = vertices[i].y;
			if (vertices[i].z < minz)
				minz = vertices[i].z;
			if (vertices[i].z > maxz)
				maxz = vertices[i].z;
		}
		maxX = maxx;
		maxY = maxy;
		minX = minx;
		minY = miny;
		maxZ = maxz;
		minZ = minz;

		return glm::vec4((minx + maxx) / 2.0f, (miny + maxy) / 2.0f, 1, 1);
}

//glm::vec4 MeshModel::getFaceCenter(int index)
//{
//	int a= faces.at(index).GetVertexIndex(0);
//	int b = faces.at(index).GetVertexIndex(1);
//	int c = faces.at(index).GetVertexIndex(2);
//	glm::vec3 center = vertices.at(a - 1) + vertices.at(b - 1) + vertices.at(c - 1);
//	center.x /= 3;
//	center.y /= 3;
//	center.z /= 3;
//	glm::vec4 ret = glm::vec4(center, 1.0f);
//	return ret;
//
//}

glm::vec4 MeshModel::getFaceNormal(int index)
{
	glm::vec3 v1 = vertices.at(faces.at(index).GetVertexIndex(0) - 1);
	glm::vec3 v2 = vertices.at(faces.at(index).GetVertexIndex(1) - 1);
	glm::vec3 v3 = vertices.at(faces.at(index).GetVertexIndex(2) - 1);

	 glm::vec4 ret=glm::vec4(glm::cross(v2 - v1, v3 - v1), 1.f);
	 double norm = sqrt(pow(ret[0], 2) + pow(ret[1], 2) + pow(ret[2], 2));

	 ret[0] /= norm;
	 ret[1] /= norm;
	 ret[2] /= norm;
	 return ret;

}