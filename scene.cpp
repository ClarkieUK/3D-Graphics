#include "scene.h"

scene::scene(const float width, const float height, const char* name)
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // define opengl version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // working with core package

	window = glfwCreateWindow(width, height, name, NULL, NULL);

	glfwMakeContextCurrent(window); // create window and let it know that is our target

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl; // (?) do glad shit
	}
};

scene::~scene()
{

};

GLFWwindow* scene::getWindow() {
	return window;
}


void scene::addCallbacks(GLFWframebuffersizefun framebuffer_size_callback, GLFWcursorposfun mouse_callback, GLFWscrollfun scroll_callback) {

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // pass the function we defined for window resize
	// this must be done so glfw knows what to do in that event
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

}