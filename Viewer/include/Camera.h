#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

private:
	glm::mat4x4 view_transformation = glm::mat4(1);;
	glm::mat4x4 projection_transformation = glm::mat4(1);
};
