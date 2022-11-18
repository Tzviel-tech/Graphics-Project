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
//glm::mat4 &MeshModel::getLocal() 
//{
//	return local;
//}
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

