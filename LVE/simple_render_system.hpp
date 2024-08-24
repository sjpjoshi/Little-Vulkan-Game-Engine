#pragma once

#include "lve_pipline.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_camera.hpp"
#include "lve_frame_info.hpp"

// std
#include <memory>
#include <vector>

namespace lve {

    class SimpleRenderSystem {
    public:

        SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout); 
        ~SimpleRenderSystem();
        void renderGameObjects(FrameInfo &frameInfo);

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

    private: 
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout); 
        void createPipeline(VkRenderPass renderPass);

        LveDevice& lveDevice;

        std::unique_ptr<LvePipeline> lvePipeline;
        VkPipelineLayout pipelineLayout;
        std::unique_ptr<LveModel> lveModel;

    }; // SimpleRenderSystem

} // namespace lve
