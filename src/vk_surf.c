/* Implements vk_surf.h */
#include <vk_surf.h>

/* Destroy a surface */
void vk_surf_destroy(vk_surf_t *surf, const vk_inst_t *inst) {
  vkDestroySurfaceKHR(inst->instance, *surf, NULL);
}
