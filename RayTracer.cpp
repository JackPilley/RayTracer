#include <iostream>
#include <fstream>
#include <string>
#include <optional>

#include "Scene/Scene.hpp"
#include "Scene/Ray.hpp"
#include "Scene/Sphere.hpp"

int main(int argc, char** argv)
{
    //std::string filename(argv[1]);
    //Scene scene(filename);

    Sphere sphere("s", 3, 1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0);
    Ray ray(glm::vec3(1, 3, 0), glm::normalize(glm::vec3(1, -0.75, 0)));

    std::optional<Intersection> result = ray.SphereIntersection(sphere);

    if (result)
    {
        Intersection intersect = result.value();
        std::cout << result->t << " At " << intersect.position.x << " " << intersect.position.y << " " << intersect.position.z 
            << " Normal " << intersect.normal.x << " " << intersect.normal.y << " " << intersect.normal.z << "\n";
    }
    else 
    {
        std::cout << "No Intersection" << "\n";
    }

    return 0;
}
