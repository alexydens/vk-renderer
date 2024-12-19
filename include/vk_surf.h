/* Include guard */
#if !defined(VK_SURF_H)
#define VK_SURF_H

/* Inclues */
#include <base.h>
#include <vk_inst.h>

/* Vulkan surface */
typedef struct {
  VkSurfaceKHR surface;
} vk_surf_t;

/* Destroy surface */
extern void vk_surf_destroy(vk_surf_t *vk_surf, vk_inst_t *vk_inst);

#endif /* VK_SURF_H */
