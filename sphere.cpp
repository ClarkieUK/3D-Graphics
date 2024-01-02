#include "sphere.h"

sphere::sphere(const int radius, const int resolution) 
{
	float delta_theta = 2 * M_PI / resolution;
	float delta_phi = M_PI / resolution;
	float theta, phi;

	// vertex
	float x, y, z, xy;
	float nx, ny, nz, lengthInv = 1.0f / radius;
	float s, t;

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

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			s = (float)j / resolution;
			t = (float)i / resolution;
			tex_coords.push_back(s);
			tex_coords.push_back(t);
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
	std::vector<float> result;
	/*
	for (size_t i = 0; i <= vertices.size() % 3; i++) {

		result.push_back(vertices[3 * i]);
		result.push_back(vertices[3 * i + 1]);
		result.push_back(vertices[3 * i + 2]);

		result.push_back(tex_coords[2 * i]);
		result.push_back(tex_coords[2 * i + 1]);

		result.push_back(normals[3 * i]);
		result.push_back(normals[3 * i + 1]);
		result.push_back(normals[3 * i + 2]);

	}*/
	for (size_t i = 0; i < vertices.size()/3; i += 1) {
		// Vertices (p1x, p1y, p1z)
		result.push_back(vertices[3*i]);
		result.push_back(vertices[3*i + 1]);
		result.push_back(vertices[3*i + 2]);

		// Texture coordinates (t1x, t1y)
		result.push_back(tex_coords[2*i]);
		result.push_back(tex_coords[2*i + 1]);

		// Normals (n1x, n1y, n1z)
		result.push_back(normals[3*i]);
		result.push_back(normals[3*i + 1]);
		result.push_back(normals[3*i + 2]);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * result.size(), result.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	std::cout<<vertices.size() * sizeof(float) ;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);						// position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));		// texture
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*5));		// norm

	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};

sphere::~sphere()
{
	
};

void sphere::draw(shader &shader)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	
};

