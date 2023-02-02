#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	const glm::mat4x4& GetProjectionTransformation()const;
	void SetPTransform(float left, float right, float up, float down, float near, float far);
	const glm::mat4x4& GetViewTransformation()const;
	float a = 0;
	float b = 0;
	float c = 1;
	void setLocalTranslate();
	void setlocalScale();
	void setLocalRotation();
	void setWorld();
	void setWorldTranslate();
	void setWorldScale();
	void setWorldRotation();
	void setLocal();
	glm::mat4& getLocal() { setLocal();return local; };
	glm::mat4& getWorld() { setWorld();return world; };
	glm::mat4 getTransform() { return getWorld() * getLocal(); };
	//local running prameters
	float scalex = 1;
	glm::vec3 trans = glm::vec3(0.0f);
	glm::vec3 rotate = glm::vec3(0.0f);
	float windowsheight=1280;
	float windowswidth=720;
	glm::vec3 eye = glm::vec3(0, 0, 10);
	glm::vec3 GetCameraEye()
	{
		return glm::vec3(a, b, c);
	}
	//world running pramaters
	float scalexW = 1;
	glm::vec3 transW = glm::vec3(0.0f);
	glm::vec3 rotateW = glm::vec3(0.0f);
	bool pres;

private:
	glm::mat4 view_transformation;
	glm::mat4 projection_transformation = glm::mat4(1);
	glm::mat4 local = glm::mat4(1);
	glm::mat4 world = glm::mat4(1);
	glm::mat4 localscalemat = glm::mat4(1);
	glm::mat4 localtransmat = glm::mat4(1);
	glm::mat4 localrotation = glm::mat4(1);
	glm::mat4 Wscalemat = glm::mat4(1);
	glm::mat4 Wtransmat = glm::mat4(1);
	glm::mat4 Wrotation = glm::mat4(1);

};
