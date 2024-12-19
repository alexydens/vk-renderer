/* Implements vk_surf.h */
#include <vk_surf.h>
#include <vulkan/vulkan_core.h>

/* Destroy surface */
void vk_surf_destroy(vk_surf_t *vk_surf, vk_inst_t *vk_inst) {
  vkDestroySurfaceKHR(vk_inst->instance, vk_surf->surface, NULL);
  vk_surf->surface = VK_NULL_HANDLE;
  /* Print message */
  log_msg(LOG_LEVEL_SUCCESS, "Vulkan surface destroyed");
}
