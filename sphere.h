#define _USE_MATH_DEFINES
#include <cmath>
#include <list>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <random>
#include "shader.h"
class sphere
{
	float RADIUS = 5.0f;
	unsigned int RESOLUTION = 10;
	shader sphere_shader(const char* vertexPath, const char* fragmentPath);

private :
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<int> line_indices;
	std::vector<float> normals;
	std::vector<float> tex_coords;
	glm::vec3 position;

public:

	sphere(const int radius, const int resolution);
	~sphere();
	void draw(shader &shader);
};