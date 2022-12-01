#include "Camera.h"

Camera::Camera()
{
	//SetCameraLookAt(glm::vec3(a, b,c), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

Camera::~Camera()
{
	
}
void Camera:: SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	view_transformation=glm::inverse(glm::lookAt(eye, at, up));

}
void Camera::SetPTransform(float left, float right, float up, float down, float near, float far)
{
	projection_transformation = glm::ortho(left, right, down, up, near, far);
}
const glm::mat4& Camera::GetProjectionTransformation()const
{
	
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}