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

Color Scene::Raytrace(Ray ray)
{
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
		return Color{ backgroundColor };
	}

	const Sphere& intersectedSphere = closestIntersect->first;
	const Intersection& intersection = closestIntersect->second;

	const glm::vec3 baseColor = intersectedSphere.GetColor();

	glm::vec3 color = baseColor * ambientColor * intersectedSphere.GetAmbientFactor();

	for (const Light& light : lights)
	{
		glm::vec3 lightDir = glm::normalize(light.GetPosition() - intersection.position);
		float lightDot = glm::dot(lightDir, intersection.normal);

		if (lightDot > 0)
		{
			// Add diffuse component
			color += light.GetIntensity() * baseColor * intersectedSphere.GetDiffuseFactor() * lightDot;

			glm::vec3 viewDir = glm::normalize(-intersection.position);
			glm::vec3 reflected = glm::reflect(-lightDir, intersection.normal);

			// Add specular component
			color += std::powf(glm::dot(reflected, viewDir), intersectedSphere.GetSpecularExponent()) *
				light.GetIntensity() *
				intersectedSphere.GetSpecularFactor();
		}
	}

	return color;
}

Image Scene::Render()
{
	Image image;
	image.x = resolution.x;
	image.y = resolution.y;
	image.name = outputName;

	//Casting to a 64 bit integers shouldn't really be needed, but it stops VS from complaining about possible overflows
	image.pixels = new Color[static_cast<uint64_t>(image.x) * static_cast<uint64_t>(image.y)];

	float width = camera.right - camera.left;
	float height = camera.top - camera.bottom;

	float xStep = width / resolution.x;
	float yStep = width / resolution.y;

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