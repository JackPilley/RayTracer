#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Camera.hpp"
#include "Light.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Image.h"

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

	void BuildSphere(std::stringstream& ss);
	void BuildLight(std::stringstream& ss);

public:
	Scene(std::string filePath);
	// Recursively simulate a single ray
	Color Raytrace(Ray ray);
	// Render the full scene image
	Image Render();

	const glm::u32vec2& GetResolution() const { return resolution; };
	const std::string& GetOutputName() const { return outputName; };
};