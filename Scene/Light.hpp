#pragma once

#include <string>
#include <glm/vec3.hpp>

class Light
{
private:
	std::string name;

	glm::vec3 position;
	glm::vec3 intensity;
};