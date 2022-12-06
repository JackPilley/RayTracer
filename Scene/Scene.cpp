#include "Scene.hpp"
#include <fstream>
#include <iostream>
#include <glm/vec3.hpp>

Scene::Scene(std::string filePath) :
	camera(), resolution(), ambientColor(), backgroundColor()
{
	spheres.reserve(14);
	lights.reserve(14);

	std::ifstream file(filePath);
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string type;
		ss >> type;

		if (type == "SPHERE")
		{
			BuildSphere(ss);
		}
		else if (type == "LIGHT")
		{
			BuildLight(ss);
		}
		else if (type == "NEAR")
		{
			ss >> camera.near;
		}
		else if (type == "LEFT")
		{
			ss >> camera.left;
		}
		else if (type == "RIGHT")
		{
			ss >> camera.right;
		}
		else if (type == "BOTTOM")
		{
			ss >> camera.bottom;
		}
		else if (type == "TOP")
		{
			ss >> camera.top;
		}
		else if (type == "BACK")
		{
			float r, g, b;
			ss >> r >> g >> b;
			backgroundColor = glm::vec3(r, g, b);
		}
		else if (type == "RES")
		{
			float x, y;
			ss >> x >> y;
			resolution = glm::vec2(x, y);
		}
		else if (type == "AMBIENT")
		{
			float r, g, b;
			ss >> r >> g >> b;
			ambientColor = glm::vec3(r, g, b);
		}
		else if (type == "OUTPUT")
		{
			ss >> outputName;
		}
	}
}

void Scene::BuildSphere(std::stringstream& ss)
{
	std::string name;
	float px, py, pz;
	float sx, sy, sz;
	float r, g, b;
	float ka, kd, ks, kr;
	uint32_t exp;

	ss >> name >> px >> py >> pz >> sx >> sy >> sz >> r >> g >> b >> ka >> kd >> ks >> kr >> exp;
	spheres.emplace_back(Sphere(name, px, py, pz, sx, sy, sz, r, g, b, ka, kd, ks, kr, exp));
}

void Scene::BuildLight(std::stringstream& ss)
{
	std::string name;
	float px, py, pz;
	float r, g, b;

	ss >> name >> px >> py >> pz >> r >> g >> b;
	lights.emplace_back(name, px, py, pz, r, g, b);
}

glm::vec3 Scene::RaytraceHelper(Ray ray, uint32_t depth, bool firstIteration) const
{
	if (depth == 0) return { 0,0,0 };

	std::optional<std::pair<Sphere, Intersection>> closestIntersect{};

	for (const Sphere& sphere : spheres)
	{
		std::optional<Intersection> intersect = ray.SphereIntersection(sphere);
		if (intersect)
		{
			if (!closestIntersect || closestIntersect->second.t > intersect->t)
			{
				closestIntersect.emplace(std::pair{ sphere, intersect.value() });
			}
		}
	}

	if (!closestIntersect)
	{
		if (firstIteration)
			return { backgroundColor };
		else
			return { 0,0,0 };
	}

	const Sphere& intersectedSphere = closestIntersect->first;
	const Intersection& intersection = closestIntersect->second;

	const glm::vec3 baseColor = intersectedSphere.GetColor();

	glm::vec3 color = baseColor * ambientColor * intersectedSphere.GetAmbientFactor();

	for (const Light& light : lights)
	{
		glm::vec3 lightDir = light.GetPosition() - intersection.position;

		// Check for shadows
		bool shadow = false;
		for (const Sphere& sphere : spheres)
		{
			Ray shadowRay{ intersection.position + lightDir * 0.0001f, lightDir };
			std::optional<Intersection> shadowIntersect = shadowRay.SphereIntersection(sphere);

			if (shadowIntersect && shadowIntersect.value().t < 1.0f)
			{
				shadow = true;
				break;
			}
		}
		// Skip this light if it's blocked
		if (shadow) continue;

		lightDir = glm::normalize(lightDir);

		const float lightDot = glm::dot(lightDir, intersection.normal);

		if (lightDot > 0)
		{
			// Add diffuse component
			color += light.GetIntensity() * baseColor * intersectedSphere.GetDiffuseFactor() * lightDot;

			glm::vec3 viewDir = glm::normalize(-intersection.position);
			glm::vec3 reflected = glm::reflect(-lightDir, intersection.normal);

			// Add specular component
			color += std::powf(std::max(glm::dot(reflected, viewDir), 0.0f), intersectedSphere.GetSpecularExponent()) *
				light.GetIntensity() *
				intersectedSphere.GetSpecularFactor();
		}
	}

	if (intersectedSphere.GetReflectanceFactor() > 0)
	{
		glm::vec3 reflectedDir = glm::reflect(ray.GetDirection(), intersection.normal);
		glm::vec3 reflectionStart = intersection.position + reflectedDir * 0.0001f;
		color += RaytraceHelper(Ray(reflectionStart, reflectedDir), depth - 1, false) * intersectedSphere.GetReflectanceFactor();
	}

	return color;
}

Color Scene::Raytrace(Ray ray) const
{
	return RaytraceHelper(ray, 4, true);
}

Image Scene::Render() const
{
	Image image;
	image.x = resolution.x;
	image.y = resolution.y;
	image.name = outputName;

	//Casting to a 64 bit integers shouldn't really be needed, but it stops VS from complaining about possible overflows
	image.pixels = new Color[static_cast<uint64_t>(image.x) * static_cast<uint64_t>(image.y)];

	const float width = camera.right - camera.left;
	const float height = camera.top - camera.bottom;

	const float xStep = width / resolution.x;
	const float yStep = width / resolution.y;

	float posX = camera.left;
	float posY = camera.bottom;
	float posZ = -camera.near;

	for (uint32_t y = 0; y < resolution.y; ++y)
	{
		for (uint32_t x = 0; x < resolution.x; ++x)
		{
			glm::vec3 pos(posX, posY, posZ);
			Ray ray(pos, pos);

			Color color = Raytrace(ray);

			image.pixels[x + y * resolution.x] = color;
			
			posX += xStep;
		}
		posY += yStep;
		posX = camera.left;
	}

	return image;
}