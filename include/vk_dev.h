/* Include guard */
#if !defined(VK_DEV_H)
#define VK_DEV_H

/* Includes */
#include <base.h>
#include <vk_inst.h>
#include <vk_phys_dev.h>

/* Types */
/* Vulkan device builder */
typedef struct {
  const char **extensions;
  uint32_t extension_count;
  const char **layers;
  uint32_t layer_count;
} vk_dev_builder_t;
/* Vulkan device */
typedef VkDevice vk_dev_t;

/* Create a Vulkan device builder */
extern vk_dev_builder_t vk_dev_builder(void);
/* Add a Vulkan device extension */
extern void vk_dev_builder_add_ext(
    vk_dev_builder_t *builder,
    const char *ext
);
/* Add a Vulkan device layer */
extern void vk_dev_builder_add_layer(
    vk_dev_builder_t *builder,
    const char *layer
);
/* Create a Vulkan device (and free builder) */
extern vk_dev_t vk_dev_create(
    vk_inst_t *inst,
    vk_phys_dev_t *phys_dev,
    vk_dev_builder_t *builder
);
/* Destroy a Vulkan device */
extern void vk_dev_destroy(vk_dev_t *dev);

#endif /* VK_DEV_H */
