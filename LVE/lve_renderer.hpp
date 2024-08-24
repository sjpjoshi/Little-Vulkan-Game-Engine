#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>

namespace lve {

    class LveRenderer {
    public:
        LveRenderer(LveWindow &window, LveDevice &device);
        ~LveRenderer();

        LveRenderer(const LveRenderer&) = delete;
        LveRenderer& operator=(const LveRenderer&) = delete;

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        bool isFrameInProgress() const { return isFrameStarted; } // isFrameInProgress

        VkCommandBuffer getCurrentCommandBuffer() const { 
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex]; 

        } // getCurrentCommandBuffer

        VkRenderPass getSwapChainRenderPass() const {
            return lveSwapChain->getRenderPass();

        } // getSwapChainRenderPass

        float getAspectRatio() const { return lveSwapChain->extentAspectRatio(); } // getAspectRatio

        int getFrameIndex() const { 
            assert(isFrameStarted && "Cannot get a frame index when frame not in progress");
            return currentFrameIndex;

        } // getFrameIndex

    private:

        void createCommandBuffers();
        void recreateSwapChain();
        void freeCommandBuffers();

        LveWindow& lveWindow;
        LveDevice& lveDevice;

        std::unique_ptr<LveSwapChain> lveSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex; 
        int currentFrameIndex;
        bool isFrameStarted = false;

    }; // FirstApp

} // namespace lve
