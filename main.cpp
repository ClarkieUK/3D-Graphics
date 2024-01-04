#include "shader.h"
#include "sphere.h"
#include "camera.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "scene.h"
#include "text.h"

float last_x_position = 400, last_y_position = 300;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

bool first_mouse = true;

float delta_time = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

camera main_camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); // we create this function that glfw will call whenever the window is resized, 
									 // it takes the window pointer and the new dimensions
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // we check to see if the escape is pressed in the context of the
	{													   // window, if true then we flag the closing of glfw window
		glfwSetWindowShouldClose(window, true);			   // GetKey returns either GLFW_RELEASE or GLFW_PRESS
	}

	const float cameraSpeed = 5.0f * delta_time; // adjust accordingly 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		main_camera.ProcessKeyboard(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		main_camera.ProcessKeyboard(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		main_camera.ProcessKeyboard(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		main_camera.ProcessKeyboard(RIGHT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		main_camera.ProcessKeyboard(DOWN, delta_time);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		main_camera.ProcessKeyboard(UP, delta_time);
};

void mouse_callback(GLFWwindow* window, double x_position, double y_position)
{
	if (first_mouse)
	{
		last_x_position = x_position;
		last_y_position = y_position;
		first_mouse = false;
	}

	float xoffset = x_position - last_x_position;
	float yoffset = last_y_position - y_position;
	last_x_position = x_position;
	last_y_position = y_position;

	main_camera.ProcessMouseMovement(xoffset, yoffset,true);
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	main_camera.ProcessMouseScroll(yoffset);
};

int main()
{
	const float WIDTH		{ 800 };
	const float HEIGHT		{ 600 };

	const char* SCENE_NAME = "3d Enviro";

	scene main_scene(WIDTH, HEIGHT, SCENE_NAME);

	GLFWwindow* window = main_scene.getWindow();

	glViewport(0, 0, WIDTH, HEIGHT); //opengl maps between -1 -> 1 so coord/100 * WIDTH gives transformed location
									 //				-liam 22/12/23

	main_scene.addCallbacks(framebuffer_size_callback, mouse_callback, scroll_callback);


	// Textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // repeat texture for out of bounds
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // s,t axes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);     // select nearest color for texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	   // select like nearest blend
	// could also select GL_LINEAR_MIPMAP_LINEAR when zooming

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	shader ourShader("sphere_shader.vs", "sphere_shader.fs");
	ourShader.use(); // Activate textures before setting uniforms
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	
	sphere sphere_one(3, 25);
	sphere global_sphere(50, 75);

	text test_text("This is sample Text", 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		// Inputs
		process_input(window);

		float currentFrame = glfwGetTime(); 
		delta_time = currentFrame - lastFrame; 
		lastFrame = currentFrame; 

		glClearColor(0.11f, 0.11f, 0.11f, 1.0f); // State-set
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		  // State-use

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);
		glm::mat4 view = main_camera.GetViewMatrix();
		ourShader.setMat4("view", view);	 // position , target , up vector 
		glm::mat4 projection = glm::perspective(glm::radians(main_camera.Zoom), WIDTH / HEIGHT, 0.1f, 1000.0f); //fov, box left, right,close,far.
		ourShader.setMat4("projection", projection);

		sphere_one.draw(ourShader);

		global_sphere.draw(ourShader);

		std::cout << "sens :" << main_camera.MouseSensitivity << "\n";
		std::cout << "fov :" << main_camera.Zoom << "\n";
		std::cout << "modified sens :" << main_camera.MouseSensitivity * (main_camera.Zoom/50) << "\n";

		test_text.Render();

		glBindVertexArray(0);
		glfwPollEvents();					// do shit
		glfwSwapBuffers(window);			// try to remove artifacts by moving front buffer to back then back to front etc...
	}
	glfwTerminate();						// terminate then close out of program
	return 0;
}