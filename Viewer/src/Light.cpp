#include "Light.h"
glm::vec3 Light::GetAmbient() const
{
	return ambient;
}

glm::vec3 Light::GetDiffuse() const
{
	return diffuse;
}

glm::vec3 Light::GetSpecular() const
{
	return specular;
}

void Light::SetAmbient(glm::vec3 ambient)
{
	this->ambient= ambient;
}

void Light::SetDiffuse(glm::vec3 diffuse)
{
	this->diffuse= diffuse;
}

void Light::SetSpecular(glm::vec3 specular)
{
	this->specular= specular;
}