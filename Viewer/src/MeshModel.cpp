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
	localscalemat=glm::scale(glm::mat4(1), glm::vec3(scalex, scalex, scalex));
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

