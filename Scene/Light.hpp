#pragma once

#include <string>
#include <glm/vec3.hpp>

class Light
{
private:
	std::string name;

	glm::vec3 position;
	glm::vec3 intensity;

public:
	Light(std::string name, float posX, float posY, float posZ, float r, float g, float b);
};