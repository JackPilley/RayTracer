#pragma once

#include <string>
#include <glm/vec3.hpp>

class Sphere
{
private:
	std::string name;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 color;

	float ambientFactor;
	float diffuseFactor;
	float specularFactor;
	float reflectanceFactor;

	uint32_t specularExponent;

public:
	Sphere(std::string name, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float r, float g, float b, float ka, float kd, float ks, float kr, float specExp);
};