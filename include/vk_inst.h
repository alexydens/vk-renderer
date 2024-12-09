/* Include guard */
#if !defined(VK_INST_H)
#define VK_INST_H

/* Includes */
#include <base.h>

/* Vulkan instance */
typedef struct {
  VkInstance instance;
  const char **extensions;
  uint32_t num_extensions;
  const char **layers;
  uint32_t num_layers;
  VkDebugUtilsMessengerEXT debug_messenger;
  bool use_messenger;
} vk_inst_t;

/* Initialize vulkan instance struct */
extern void vk_inst_init(vk_inst_t *vk_inst);
/* Add requested extension */
extern void vk_inst_add_extension(vk_inst_t *vk_inst, const char *extension);
/* Add requested layer */
extern void vk_inst_add_layer(vk_inst_t *vk_inst, const char *layer);

/* Create vulkan instance (if app_name is NULL, a default will be used) */
extern void vk_inst_create(
    vk_inst_t *vk_inst,
    const char *app_name,
    bool use_messenger
);
/* Destroy vulkan instance */
extern void vk_inst_destroy(vk_inst_t *vk_inst);

#endif /* VK_INST_H */
