#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class scene
{

private:

public:
	GLFWwindow* window;

	// Constructor
	scene(const float width, const float height, const char* name);

	~scene();

	// Callback functions
	void addCallbacks(GLFWframebuffersizefun framebuffer_size_callback, GLFWcursorposfun mouse_callback, GLFWscrollfun scroll_callback);

	// Getters
	GLFWwindow* getWindow();
};


