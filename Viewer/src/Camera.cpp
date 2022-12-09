#include "Camera.h"

Camera::Camera()
{
	SetCameraLookAt(glm::vec3(300, 300,300), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
void Camera::setLocalTranslate()
{
	localtransmat = glm::inverse(glm::translate(glm::mat4(1), glm::vec3(trans.x, trans.y, trans.z)));
}

void Camera::setlocalScale()
{
	localscalemat = glm::inverse(glm::scale(glm::mat4(1), glm::vec3(scalex, scalex, scalex)));
}
void Camera::setLocalRotation()
{
	glm::mat4 localrotationMatrixx = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 localrotationMatrixy = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 localrotationMatrixz = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	localrotation = glm::inverse(localrotationMatrixz * localrotationMatrixy * localrotationMatrixx);
}
void Camera::setLocal()
{
	setLocalRotation();
	setlocalScale();
	setLocalTranslate();
	local = localtransmat * localscalemat * localrotation;

}
void Camera::setWorldTranslate()
{
	Wtransmat = glm::inverse(glm::translate(glm::mat4(1), glm::vec3(transW.x, transW.y, transW.z)));
}

void Camera::setWorldScale()
{
	Wscalemat = glm::inverse(glm::scale(glm::mat4(1), glm::vec3(scalexW, scalexW, scalexW)));
}
void Camera::setWorldRotation()
{
	glm::mat4  WrotationMatrixx = glm::rotate(glm::mat4(1.0f), glm::radians(rotateW.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4  WrotationMatrixy = glm::rotate(glm::mat4(1.0f), glm::radians(rotateW.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4  WrotationMatrixz = glm::rotate(glm::mat4(1.0f), glm::radians(rotateW.z), glm::vec3(0.0f, 0.0f, 1.0f));
	Wrotation = glm::inverse(WrotationMatrixz * WrotationMatrixy * WrotationMatrixx);
}
void Camera::setWorld()
{
	setWorldRotation();
	setWorldScale();
	setWorldTranslate();
	world = Wtransmat * Wscalemat * Wrotation;

}


Camera::~Camera()
{
	
}
void Camera:: SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{

	view_transformation= getTransform()*glm::lookAt(eye, at, up);

}
void Camera::SetPTransform(float left, float right, float down, float up, float near, float far)
{
	float aspect = 1280 / 720;
	//projection_transformation = glm::ortho(left, right, down, up, near, far);
	projection_transformation = glm::perspective(glm::radians(45.0f), aspect, near, far);
}
const glm::mat4& Camera::GetProjectionTransformation()const
{
	
	return projection_transformation;
}


const glm::mat4x4& Camera::GetViewTransformation()
{
	
	return view_transformation;
}