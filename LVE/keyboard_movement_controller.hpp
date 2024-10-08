#pragma once

#include "lve_game_object.hpp"
#include "lve_window.hpp"

namespace lve {

	// we wont put lve in front of the class name for now, may change this later
	class KeyboardMovementController {
	public:
        struct KeyMapping {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;

        }; // KeyMapping

        KeyMapping keys{};
        float moveSpeed{ 3.f  };
        float lookSpeed{ 1.5f };

        void moveInPlaneXZ(GLFWwindow *window, float dt, LveGameObject& gameObject);

	}; // KeyboardMovementController

} // lve