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
};