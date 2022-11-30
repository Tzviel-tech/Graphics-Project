#include "Camera.h"

Camera::Camera()
{
	
}

Camera::~Camera()
{
	
}
void Camera:: SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	view_transformation=glm::inverse(glm::lookAt(eye, at, up));


}
const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}