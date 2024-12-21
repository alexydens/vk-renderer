/* Include guard */
#if !defined(VK_SURF_H)
#define VK_SURF_H

/* Includes */
#include <base.h>
#include <vk_inst.h>

/* Types */
/* Vulkan surface */
typedef VkSurfaceKHR vk_surf_t;

/* Destroy a surface */
extern void vk_surf_destroy(vk_surf_t *surf, const vk_inst_t *inst);

#endif /* VK_SURF_H */
