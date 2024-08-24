#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace lve {
	
	class LveWindow {
	
	public:
		LveWindow(int w, int h, std::string name);
		~LveWindow();

		// we want to destroy the copy constructors
		// if we do not destroy the copy constructors we will have a dangling pointer
		// we dont want to copy our lve window, if it is the shared glfw window will be terminated
		LveWindow(const LveWindow &) = delete;
		LveWindow &operator=(const LveWindow &) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		VkExtent2D getExtent() { return { static_cast<uint32_t> (width), static_cast<uint32_t> (height) }; } // getExtent

		bool wasWindowResized() { return frameBufferResized;  } // wasWindowResized
		void resetWindowResizedFlag() { frameBufferResized = false;  } // resetWindowResizedFlag

		GLFWwindow* getGLFWwindow() const { return window; } // getGLFWwindow

	private:
		static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);
		GLFWwindow *window;
		void initWindow();
		int width;
		int height; // we want to change dimensions
		bool frameBufferResized = false;
		std::string windowName;

	}; // LveWindow

} // lve