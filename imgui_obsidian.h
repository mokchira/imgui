#include "backends/imgui_impl_vulkan.h"

namespace ImGui
{
    static inline void InitObsidianBackend(Obdn_Orb* orb, VkDescriptorPool ui_desc_pool, VkRenderPass renderpass)
    {
        ImGui_ImplVulkan_InitInfo iivi = {};
        iivi.Instance                  = *obdn_GetVkInstance(orb->instance);
        iivi.PhysicalDevice            = obdn_GetPhysicalDevice(orb->instance);
        iivi.Device                    = orb->device;
        iivi.QueueFamily =
            obdn_GetQueueFamilyIndex(orb->instance, OBDN_V_QUEUE_GRAPHICS_TYPE);
        iivi.Queue = obdn_GetGraphicsQueue(orb->instance, 2); // arbitrary choice
        iivi.DescriptorPool = ui_desc_pool;
        iivi.MinImageCount  = 2;
        iivi.ImageCount     = 2;
        ImGui_ImplVulkan_Init(&iivi, renderpass);
        Obdn_Command cmd =
            obdn_CreateCommand(orb->instance, OBDN_V_QUEUE_GRAPHICS_TYPE);
        obdn_BeginCommandBufferOneTimeSubmit(cmd.buffer);
        ImGui_ImplVulkan_CreateFontsTexture(cmd.buffer);
        obdn_EndCommandBuffer(cmd.buffer);
        obdn_SubmitAndWait(&cmd, 2);
        obdn_DestroyCommand(cmd);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
};
