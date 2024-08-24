#include "lve_renderer.hpp"

// std
#include <stdexcept>
#include <array>
#include <cassert>
#include <iostream>

namespace lve {
	LveRenderer::LveRenderer(LveWindow& window, LveDevice& device) : lveWindow{ window }, lveDevice{device} {
		recreateSwapChain();
		createCommandBuffers();

	} // lveRenderer

	void LveRenderer::recreateSwapChain() {
		auto extent = lveWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = lveWindow.getExtent();
			glfwWaitEvents();

		} // while

		vkDeviceWaitIdle(lveDevice.device());
		lveSwapChain = nullptr;

		if (lveSwapChain == nullptr) {
			lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);

		} else {

			std::shared_ptr<LveSwapChain> oldSwapChain = std::move(lveSwapChain);
			lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*lveSwapChain.get())) {
				// instead of throwing an error it would be better to make a call back notifying the app that a change has been made
				// will be done at a later date
				throw std::runtime_error("Swap chain image(or depth) format has changed");

			} // if

		} // else


		// .....

	} // recreateSwapChain

	void LveRenderer::createCommandBuffers() {
		commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);


		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = lveDevice.getCommandPool();
		// an application may need to delete an create command buffers frequently so to reduce the cost of resource creation Vulkan has us allocate and free command buffers from command pools
		// this way the most expensive stuff of requiring memory can be done once and be reused as command buffers are created and destroyed
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");

		} // if

	} // createCommandBuffers


	void LveRenderer::freeCommandBuffers() {
		vkFreeCommandBuffers(lveDevice.device(), lveDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();

	} // freeCommandBuffers


	VkCommandBuffer LveRenderer::beginFrame() {
		assert(!isFrameStarted && "Can't call begin frame while a frame is already in progress!");
		auto result = lveSwapChain->acquireNextImage(&currentImageIndex); // handles all the synchronization surronding double and triple buffering 

		// VK_ERROR_OUT_OF_DATE_KHR: A surface that changed in such a way that is no longer compatible with the swap chain
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr; // frame has not successfully started

		} // if 

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to aquire the next swapchain image");

		} // if

		isFrameStarted = true;
		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");

		} // if

		return commandBuffer;

	} // beginFrame

	void LveRenderer::endFrame() {
		assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin record command buffer!");

		} // if

		auto result = lveSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized()) {
			lveWindow.resetWindowResizedFlag();
			recreateSwapChain();

		} else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to aquire the submit command buffers");

		} // if 

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;

	} // endFrame

	void LveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "Can't call beginSwapChainRenderPass while frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin renderpass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = lveSwapChain->getRenderPass();
 		renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(currentImageIndex);

 		renderPassInfo.renderArea.offset = { 0, 0 };
 		renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();
		 
 		std::array<VkClearValue, 2> clearValues{};
 		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f }; // remeber index 0 is color and 1 is depth stencil
 		clearValues[1].depthStencil = { 1.0f, 0 };
		 
 		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
 		renderPassInfo.pClearValues = clearValues.data();
 		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

 		VkViewport viewport{};
 		viewport.x = 0.0f;
 		viewport.y = 0.0f;
 		viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
 		viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
 		viewport.minDepth = 0.0f;
 		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, lveSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


	} // beginSwapChainRenderPass

	void LveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "Can't call endSwapChainRenderPass while frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin renderpass on command buffer from a different frame");
			
		vkCmdEndRenderPass(commandBuffer);

	} // endSwapChainRenderPass

	LveRenderer::~LveRenderer() {
		freeCommandBuffers();

	} // ~LveRenderer

} // namespace lve