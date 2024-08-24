#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"

// lib
#include <vulkan/vulkan.h>

namespace lve {

	#define MAX_LIGHTS 10

	struct PointLight {
		glm::vec4 position{}; // ignore w
		glm::vec4 color{}; // w is intensity 

	}; // PointLight

	// we are going to use this to pass in read only data to the pipeline shaders
	struct GlobalUbo {
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 inverseView{ 1.f }; 
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f }; // w is intensity

		PointLight pointLights[MAX_LIGHTS];
		int numLights;

	}; // GlobalUbo

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		LveCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		LveGameObject::Map& gameObject;

	}; // FrameInfo

} // lve