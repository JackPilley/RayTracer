#pragma once

#include <optional>
#include <glm/vec3.hpp>
#include "Sphere.hpp"

struct Intersection
{
	glm::vec3 position;
	glm::vec3 normal;
	float t;
};

class Ray
{
private:
	glm::vec3 direction;
	glm::vec3 origin;

public:
	Ray(glm::vec3 origin, glm::vec3 direction);

	//Returns the closest forward intersection point with a sphere
	std::optional<Intersection> SphereIntersection(const Sphere& sphere) const;
	glm::vec3 GetPointAlongRay(float t) const;

	const glm::vec3& GetDirection() const { return direction; }
	const glm::vec3& GetOrigin() const { return origin; }
};