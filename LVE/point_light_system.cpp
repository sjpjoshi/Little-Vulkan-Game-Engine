#include "point_light_system.hpp"

// std
#include <stdexcept>
#include <array>
#include <cassert>
#include <iostream>
#include <chrono>
#include <map>

// libs
#define GLM_FORCE_RADIANS // forces in radians and not degrees
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // Vulkan uses 0 to 1, openGL uses 1 to 1
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {

	struct PointLightPushConstants {
		glm::vec4 position{};
		glm::vec4 color{};
		float radius;

	}; // PointLightPushConstants

	PointLightSystem::PointLightSystem(LveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : lveDevice{ device } {
		createPipelineLayout(globalSetLayout);
		createPipeline(renderPass);

	} // PointLightSystem

	void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
		VkPushConstantRange pushConstantRange{}; 
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0; // mainly used for if you are using separate ranges for the vertex and fragment shaders
		pushConstantRange.size = sizeof(PointLightPushConstants);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

		// push constants are a way to send efficiently a very small amount of data through to our shader programs
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; 

		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");

		} // if

	} // createPipelineLayout

	void PointLightSystem::createPipeline(VkRenderPass renderPass) {
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
		LvePipeline::enableAlphaBlending(pipelineConfig);

		pipelineConfig.attributeDescriptions.clear();
		pipelineConfig.bindingDescriptions.clear();

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>(
			lveDevice,
			"C:\\Users\\suraj\\OneDrive\\Documents\\Visual Studio Projects\\Little Vulkan Game Engine\\point_light.vert.spv",
			"C:\\Users\\suraj\\OneDrive\\Documents\\Visual Studio Projects\\Little Vulkan Game Engine\\point_light.frag.spv",
			pipelineConfig);

	}// createPipeline

	void PointLightSystem::update(FrameInfo& frameInfo, GlobalUbo& ubo) {
		auto rotateLight = glm::rotate(
			glm::mat4(1.f),
			frameInfo.frameTime,
			{ 0.f, -1.f, 0.f }

		); // rotateLight
		
		int lightIndex = 0;
		for (auto& kv : frameInfo.gameObject) {
			auto& obj = kv.second;
			if (obj.pointLight == nullptr)
				continue;

			assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specificed");

			// update light position 
			obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.f));

			// copy light to ubo
			ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
			ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);

			lightIndex++;

		} // for

		ubo.numLights = lightIndex; 

	} // update

	void PointLightSystem::render(FrameInfo& frameInfo) {
		// sorting the lights
		std::map<float, LveGameObject::id_t> sorted;
		for (auto& kv : frameInfo.gameObject) {
			auto& obj = kv.second;
			if (obj.pointLight == nullptr)
				continue;
		
			// calculate distance 
			auto offset = frameInfo.camera.getPosition() - obj.transform.translation;
			float distanceSquared = glm::dot(offset, offset);
			sorted[distanceSquared] = obj.getId(); 

		} // for

		lvePipeline->bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets
		(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0,
			1,
			&frameInfo.globalDescriptorSet,
			0,
			nullptr

		); // vkCmdBindDescriptorSets

		//instead of iterating through every game object, we can iterate through the map in reverse order
		for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
			// use obj ID to find light object
			// game obj are not copiable 
			auto& obj = frameInfo.gameObject.at(it->second);

			PointLightPushConstants push{};
			push.position = glm::vec4(obj.transform.translation, 1.f);
			push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
			push.radius = obj.transform.scale.x;

			vkCmdPushConstants(
				frameInfo.commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PointLightPushConstants),
				&push

			); // vkCmdPushConstants

			vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0); 

		} // for

	} // renderGameObjects

	PointLightSystem::~PointLightSystem() {
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);

	} // ~SimpleRenderSystem

} // namespace lve