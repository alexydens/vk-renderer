/* Include guard */
#if !defined(VK_INST_H)
#define VK_INST_H

/* Includes */
#include <base.h>

/* Types */
/* Vulkan instance builder */
typedef struct {
  bool use_messenger;
  const char **extensions;
  uint32_t extension_count;
  const char **layers;
  uint32_t layer_count;
  const char *app_name;
  uint32_t app_version;
} vk_inst_builder_t;
/* Vulkan instance */
typedef struct {
  VkInstance instance;
  VkDebugUtilsMessengerEXT debug_messenger;
  bool use_messenger;
} vk_inst_t;

/* Create a Vulkan instance builder */
extern vk_inst_builder_t vk_inst_builder(void);
/* Add a Vulkan instance extension */
extern void vk_inst_builder_add_ext(
    vk_inst_builder_t *builder, 
    const char *extension
);
/* Add required instance extensions */
extern void vk_inst_builder_add_required_exts(
    vk_inst_builder_t *builder,
    void (*get_required_exts)(const char **exts, uint32_t *count)
);
/* Add a Vulkan instance layer */
extern void vk_inst_builder_add_layer(
    vk_inst_builder_t *builder, 
    const char *layer
);
/* Use validation layers */
extern void vk_inst_builder_use_messenger(
    vk_inst_builder_t *builder
);
/* Set application name */
extern void vk_inst_builder_set_app_name(
    vk_inst_builder_t *builder,
    const char *name
);
/* Set application version */
extern void vk_inst_builder_set_app_version(
    vk_inst_builder_t *builder,
    uint8_t major, uint8_t minor, uint8_t patch
);

/* Create a Vulkan instance (and free builder) */
extern vk_inst_t vk_inst_create(vk_inst_builder_t *builder);
/* Destroy a Vulkan instance */
extern void vk_inst_destroy(vk_inst_t *inst);

#endif /* VK_INST_H */
