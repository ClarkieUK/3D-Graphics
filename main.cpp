#include "shader.h"
#include "sphere.h"
#include "camera.h"
#include "scene.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


float last_x_position = 400, last_y_position = 300;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

bool first_mouse = true;

float delta_time = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

camera ourCamera;

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
		ourCamera.ProcessKeyboard(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)				// switch case depending on world
		ourCamera.ProcessKeyboard(RIGHT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(DOWN, delta_time);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(UP, delta_time);
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

	ourCamera.ProcessMouseMovement(xoffset, yoffset,true);
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ourCamera.ProcessMouseScroll(yoffset);
};

int main()
{

	scene main_scene(const int width = 800, const int height = 600,const char* title = "3D Enviro");
	

	const float WIDTH		{ 800 };
	const float HEIGHT		{ 600 };

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // define opengl version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // working with core package

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "3D Enviro", NULL, NULL);
	glfwMakeContextCurrent(window); // create window and let it know that is our target

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl; // (?) do glad shit
		return -1;
	} 

	glViewport(0, 0, WIDTH, HEIGHT); //opengl maps between -1 -> 1 so coord/100 * WIDTH gives transformed location
									 //				-liam 22/12/23

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // pass the function we defined for window resize
																		// this must be done so glfw knows what to do in that event
	glfwSetCursorPosCallback(window, mouse_callback); 
	glfwSetScrollCallback(window, scroll_callback);

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 3 pos , 2 tex

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	/*
	glm::vec3 cubePositions[] = { 
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};*/
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  -10.0f),
		glm::vec3(0.0f,  0.0f,  -25.0f),
		glm::vec3(0.0f,  0.0f,  -50.0f),
		glm::vec3(0.0f,  0.0f,  -75.0f),
		glm::vec3(0.0f,  0.0f,  -100.0f),
		glm::vec3(0.0f,  0.0f,  -125.0f),
		glm::vec3(0.0f,  0.0f,  -150.0f),
		glm::vec3(0.0f,  0.0f,  -175.0f),
		glm::vec3(0.0f,  0.0f,  -200.0f),

	};
	
	glm::vec2 points[] = {
		glm::vec2(0.0f,0.0f),
		glm::vec2(0.0f,1.0f),
		glm::vec2(1.0f,0.0f),
		glm::vec2(0.2f,0.8f),
		glm::vec2(0.4f,0.6f),
		glm::vec2(0.6f,0.4f),
		glm::vec2(0.8f,0.2f),
	};
	for (int i = 0; i < 7; i++)
	{
		points[i] = glm::vec2(glm::normalize(points[i]).x, glm::normalize(points[i]).y);

	};
	float vertices_triangle[] = { 
		0, 0, 0.0f, // origin
		points[1].x, points[1].y, 0.0f, // top left
		points[2].x, points[2].y, 0.0f, // bottom right
		points[3].x, points[3].y, 0.0f,
		points[4].x, points[4].y, 0.0f,
		points[5].x, points[5].y, 0.0f,
		points[6].x, points[6].y, 0.0f,
	};
	unsigned int indices[] = {
		0,1,
		0,2,
		0,3,
		0,4,
		0,5,
		0,6,
	};

	// Vertex Array Object (VAO)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Buffer Object -> GPU (VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // contains n (points) * 3 (elements) * 4 (bytes) = 48

	// Indiciate openGL interpretation
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);
		glm::mat4 view = ourCamera.GetViewMatrix();
		ourShader.setMat4("view", view); // position , target , up vector 
		glm::mat4 projection = glm::perspective(glm::radians(ourCamera.Zoom), WIDTH / HEIGHT, 0.1f, 1000.0f); //fov, box left, right,close,far.
		ourShader.setMat4("projection", projection);

		sphere_one.draw(ourShader);
		//global_sphere.draw(ourShader);

		glBindVertexArray(VAO);
		std::cout << "sens :" << ourCamera.MouseSensitivity << "\n";
		std::cout << "fov :" << ourCamera.Zoom << "\n";
		std::cout << "modified sens :" << ourCamera.MouseSensitivity * (ourCamera.Zoom/50) << "\n";
		
		for (unsigned int i=0; i < 10; i++)
		{
			float time = float(glfwGetTime());
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(time)*(i+1), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36); //GL_POINTS, GL_TRIANGLES, GL_LINE_STRIP.
		}

		glBindVertexArray(0);
		glfwPollEvents(); // do shit
		glfwSwapBuffers(window); // try to remove artifacts by moving front buffer to back then back to front etc...
	}
	glfwTerminate(); // terminate then close out of program
	return 0;
}