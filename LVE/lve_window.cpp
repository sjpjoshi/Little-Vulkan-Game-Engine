#include "lve_window.hpp"
#include <stdexcept>

namespace lve {

	LveWindow::LveWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
		initWindow();

	} // LveWindow

	void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");

		} // if

	} // createWindowSurface

	void LveWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height) {
		auto lveWindow = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
		lveWindow->frameBufferResized = true;
		lveWindow->width = width;
		lveWindow->height = height;

	} // frameBufferResizedCallback

	void LveWindow::initWindow() {
	
		glfwInit();
		// a window hint is how we can set up our window
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // sets a window hint to a desired value
		// GLFW_CLIENT_API constant in the GLFW library used to specify which client API to create the vulkan context
		// GLFW_NO_API tells the compiler we do not want to use any openGL context

		// allows the window to be resized
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); 

		// creating the window, the last 2 we want no value for at this moment
		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);


	} // initWindow

	LveWindow::~LveWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();

	} // ~LveWindow

} // lve