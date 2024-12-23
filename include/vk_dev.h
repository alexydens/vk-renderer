/* Include guard */
#if !defined(VK_DEV_H)
#define VK_DEV_H

/* Includes */
#include <base.h>
#include <vk_phys_dev.h>

/* Types */
/* Vulkan device builder */
typedef struct {
  const char **extensions;
  uint32_t extension_count;
  const char **layers;
  uint32_t layer_count;
  uint32_t graphics_queues;
  float *graphics_queue_priorities;
  uint32_t present_queues;
  float *present_queue_priorities;
  uint32_t compute_queues;
  float *compute_queue_priorities;
  uint32_t transfer_queues;
  float *transfer_queue_priorities;
  VkPhysicalDeviceFeatures features;
} vk_dev_builder_t;
/* Vulkan device */
typedef struct {
  VkDevice device;
  VkQueue *graphics_queues;
  uint32_t graphics_queue_count;
  VkQueue *present_queues;
  uint32_t present_queue_count;
  VkQueue *compute_queues;
  uint32_t compute_queue_count;
  VkQueue *transfer_queues;
  uint32_t transfer_queue_count;
} vk_dev_t;

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
/* Add a Vulkan device graphics queue */
extern void vk_dev_builder_add_graphics_queue(
    vk_dev_builder_t *builder,
    float priority
);
/* Add a Vulkan device present queue */
extern void vk_dev_builder_add_present_queue(
    vk_dev_builder_t *builder,
    float priority
);
/* Add a Vulkan device compute queue */
extern void vk_dev_builder_add_compute_queue(
    vk_dev_builder_t *builder,
    float priority
);
/* Add a Vulkan device transfer queue */
extern void vk_dev_builder_add_transfer_queue(
    vk_dev_builder_t *builder,
    float priority
);
/* Add Vulkan device features */
extern void vk_dev_builder_add_features(
    vk_dev_builder_t *builder,
    VkPhysicalDeviceFeatures features
);
/* Create a Vulkan device (and free builder) */
extern vk_dev_t vk_dev_create(
    vk_phys_dev_t *phys_dev,
    vk_phys_dev_info_t *phys_dev_info,
    vk_dev_builder_t *builder
);
/* Destroy a Vulkan device */
extern void vk_dev_destroy(vk_dev_t *dev);

#endif /* VK_DEV_H */
