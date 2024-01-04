#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <list>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include "shader.h"
class cube
{
	float RADIUS = 5.0f;
	unsigned int RESOLUTION = 10;

private:

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	std::vector<float> interleaved_vertices;
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<int> line_indices;
	std::vector<float> normals;
	std::vector<float> tex_coords;
	glm::vec3 position;

public:

	cube(const int radius, const int resolution);
	~cube();
	void draw(shader& shader);
	void update();
};