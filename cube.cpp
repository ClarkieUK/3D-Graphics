#include "cube.h"

cube::cube(const int radius, const int resolution)
{
	
	/*
	for (size_t i = 0; i < vertices.size() / 3; i += 1) {
		// Vertices (p1x, p1y, p1z)
		interleaved_vertices.push_back(vertices[3 * i]);
		interleaved_vertices.push_back(vertices[3 * i + 1]);
		interleaved_vertices.push_back(vertices[3 * i + 2]);

		// Texture coordinates (t1x, t1y)
		interleaved_vertices.push_back(tex_coords[2 * i]);
		interleaved_vertices.push_back(tex_coords[2 * i + 1]);

		// Normals (n1x, n1y, n1z)
		interleaved_vertices.push_back(normals[3 * i]);
		interleaved_vertices.push_back(normals[3 * i + 1]);
		interleaved_vertices.push_back(normals[3 * i + 2]);
	}
	*/

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * interleaved_vertices.size(), interleaved_vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	std::cout << vertices.size() * sizeof(float);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);						// position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));		// texture
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));		// norm

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};

cube::~cube()
{

};

void cube::update()
{

};

void cube::draw(shader& shader)
{
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
};

