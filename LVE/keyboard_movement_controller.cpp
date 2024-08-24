#include "keyboard_movement_controller.hpp"

// std
#include <limits>

namespace lve {

	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject) {
		glm::vec3 rotate{ 0 };

		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) 
			rotate.y += 1.f;

		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
			rotate.y -= 1.f;

		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
			rotate.x += 1.f;

		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
			rotate.x -= 1.f;

		// Normalize rotation if it's non-zero
		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
			// the reason we need to do this check is because should be try to normalize the vector ourselves the equation will not work
			gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
	
		} // if

		// to prevent the game object from going upside down, we clamp it so the rotation is limit to about +- 85 degrees
		gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -glm::half_pi<float>(), glm::half_pi<float>());
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>()); // prevents spinning in 1 directin so the value does not overflow
		
		float yaw = gameObject.transform.rotation.y;
		const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
		const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
		const glm::vec3 upDir{ 0.f, -1.f, 0.f };

		glm::vec3 moveDir{ 0.f };

		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
			moveDir += forwardDir;

		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
			moveDir -= forwardDir;

		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
			moveDir += rightDir;

		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
			moveDir -= rightDir;

		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
			moveDir += upDir;

		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
			moveDir -= upDir;

		// remember when a vector is dot product with itself the answer is 0
		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
			// the reason we need to do this check is because should be try to normalize the vector ourselves the equation will not work
			gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);

		} // if

	} // KeyboardMovementController

} // lve