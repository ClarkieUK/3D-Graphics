#include "sphere.h"

sphere::sphere(const int radius, const int resolution) 
{
	float delta_theta = 2 * M_PI / resolution;
	float delta_phi = M_PI / resolution;
	float theta, phi;

	// vertex
	float x, y, z, xy;

	// entity
	int k1, k2;

	// vertex
	for (unsigned int i = 0; i <= resolution; ++i)		// each stack
	{
		phi = M_PI/ 2 - i * delta_phi;
		xy = radius * cosf(phi);
		z = radius * sinf(phi);

		for (unsigned int j = 0; j <= resolution; ++j)	// each sector 
		{
			theta = j * delta_theta;
			x = xy * cosf(theta);
			y = xy * sinf(theta);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			
		}
	}
	// entity
	for (int i = 0; i < resolution; ++i)
	{
		k1 = i * (resolution + 1);
		k2 = k1 + resolution + 1;

		for (unsigned int j = 0; j < resolution; ++j,++k1,++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);		// form a upper left triangle
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (resolution - 1))		// form a lower right triangle
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			line_indices.push_back(k1);
			line_indices.push_back(k2);

			if (i != 0)
			{
				line_indices.push_back(k1);
				line_indices.push_back(k1 + 1);
			}
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * line_indices.size(), line_indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0); 
};

sphere::~sphere()
{
	
};

void sphere::draw(shader &shader)
{
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	glDrawElements(GL_LINE_STRIP, line_indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
};

