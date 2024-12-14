/* Include guard */
#if !defined(VK_INST_H)
#define VK_INST_H

/* Includes */
#include <base.h>

/* Creation information */
typedef struct {
  const char *app_name;
  bool use_messenger;
  const char **extensions;
  uint32_t num_extensions;
  const char **layers;
  uint32_t num_layers;
} vk_inst_create_info_t;
/* Vulkan instance */
typedef struct {
  VkInstance instance;
  VkDebugUtilsMessengerEXT debug_messenger;
  bool use_messenger;
} vk_inst_t;

/* Add requested extension */
extern void vk_inst_create_info_add_extension(
    vk_inst_create_info_t *vk_inst_create_info,
    const char *extension
);
/* Add requested layer */
extern void vk_inst_create_info_add_layer(
    vk_inst_create_info_t *vk_inst_create_info,
    const char *layer
);
/* Initialize vulkan instance create info */
extern void vk_inst_create_info_init(
    vk_inst_create_info_t *vk_inst_create_info
);
/* Free vulkan instance create info */
extern void vk_inst_create_info_free(
    vk_inst_create_info_t *vk_inst_create_info
);

/* Create vulkan instance */
extern void vk_inst_create(
    vk_inst_t *vk_inst,
    vk_inst_create_info_t *vk_inst_create_info
);
/* Destroy vulkan instance */
extern void vk_inst_destroy(vk_inst_t *vk_inst);

#endif /* VK_INST_H */
