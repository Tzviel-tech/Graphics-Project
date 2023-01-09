#include <glm/glm.hpp>
#include <glm\ext\matrix_transform.hpp>
#pragma once
class Light
{
public:
	Light()
	{
		ambient = glm::vec3(100000000, 1, 1);
		diffuse = glm::vec3(1, 0, 0);
		specular = glm::vec3(1, 0, 1);
		position = glm::vec3(0, 0, 0);

	}
	glm::vec3 GetAmbient() const;
	glm::vec3 GetDiffuse() const;
	glm::vec3 GetSpecular() const;
	;
	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);
	glm::vec3 position;

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;



};