#include <utility>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "Ray.hpp"

Ray::Ray(glm::vec3 origin, glm::vec3 direction):
	origin(origin),
	direction(direction)
{}

std::optional<Intersection> Ray::SphereIntersection(const Sphere& sphere) const
{
	const glm::vec4 pos4 = sphere.GetInverseMatrix() * glm::vec4(origin, 1.0f);
	const glm::vec4 dir4 = sphere.GetInverseMatrix() * glm::vec4(direction, 0.0f);

	const glm::vec3 pos = glm::vec3(pos4.x, pos4.y, pos4.z);
	const glm::vec3 dir = glm::vec3(dir4.x, dir4.y, dir4.z);


	const float posMag = glm::length(pos);
	const float posMagSquared = posMag * posMag;

	const float dirMag = glm::length(dir);
	const float dirMagSquared = dirMag * dirMag;

	const float posDirDot = glm::dot(pos, dir);
	const float posDirDotSquared = posDirDot * posDirDot;

	const float discriminant = posDirDotSquared - dirMagSquared * (posMagSquared - 1);

	if (discriminant <= 0) return {};

	const float first = -posDirDot / dirMagSquared;
	const float second = sqrt(discriminant) / dirMagSquared;

	const float ta = first + second;
	const float tb = first - second;

	float t = tb;

	if (tb < 0 && ta < 0) return {};
	else if (tb < 0) t = ta;

	glm::vec3 intersectPosition = GetPointAlongRay(t);
	glm::vec4 canonicalIntersectPoint4 = glm::vec4(pos + dir * t, 0);
	glm::vec3 intersectionNormal = sphere.GetInverseTransposeMatrix() * canonicalIntersectPoint4;

	if (tb < 0) intersectionNormal *= -1;

	return Intersection{ intersectPosition, glm::normalize(intersectionNormal), t};
}

glm::vec3 Ray::GetPointAlongRay(float t) const
{
	return origin + direction * t;
}