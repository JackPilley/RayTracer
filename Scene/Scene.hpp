#pragma once

#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Camera.hpp"
#include "Light.hpp"
#include "Sphere.hpp"

class Scene
{
private:
	Camera camera;

	glm::u32vec2 resolution;

	glm::vec3 backgroundColor;
	glm::vec3 ambientColor;

	std::vector<Sphere> spheres;
	std::vector<Light> lights;

	std::string outputName;

public:
	Scene(std::string filePath);
};