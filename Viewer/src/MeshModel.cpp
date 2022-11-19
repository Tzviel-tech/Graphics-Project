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
	localtransmat= glm::translate(glm::mat4(1), glm::vec3(translatex, translatey, translatez));
}

void MeshModel::setlocalScale()
{
	localscalemat=glm::scale(glm::mat4(1), glm::vec3(scalex, scalex, scalex));
}
void MeshModel::setLocalRotation()
{
	glm::mat4 rotationMatrixx = glm::rotate(glm::mat4(1.0f), glm::radians(rotatex), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationMatrixy = glm::rotate(glm::mat4(1.0f), glm::radians(rotatey), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMatrixz = glm::rotate(glm::mat4(1.0f), glm::radians(rotatez), glm::vec3(0.0f, 0.0f, 1.0f));
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
	Wtransmat = glm::translate(glm::mat4(1), glm::vec3(translatexW, translateyW, translatezW));
}

void MeshModel::setWorldScale()
{
	Wscalemat = glm::scale(glm::mat4(1), glm::vec3(scalexW, scalexW, scalexW));
}
void MeshModel::setWorldRotation()
{
	glm::mat4 rotationMatrixxW = glm::rotate(glm::mat4(1.0f), glm::radians(rotatexW), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationMatrixyW = glm::rotate(glm::mat4(1.0f), glm::radians(rotateyW), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMatrixzW = glm::rotate(glm::mat4(1.0f), glm::radians(rotatezW), glm::vec3(0.0f, 0.0f, 1.0f));
	Wrotation = glm::mat4(1);
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

