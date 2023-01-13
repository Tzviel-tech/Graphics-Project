#include <glm/glm.hpp>
#include <glm\ext\matrix_transform.hpp>
#pragma once
class Light
{
public:

	Light()
	{
		ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		diffuse = glm::vec3(1.f, 1.f, 1.f);
		specular = glm::vec3(0.2f, 0.2f, 0.2f);
		position =glm::vec3(10, 10, 0);

	}
	glm::vec3& GetAmbient() ;
	glm::vec3& GetDiffuse();
	glm::vec3& GetSpecular();

	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);
	glm::vec3 position;
	float Ascale;
	float Dscale;
	float Sscale;
private:
	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;



};