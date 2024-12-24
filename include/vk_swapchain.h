/* Include guard */
#include <vulkan/vulkan_core.h>
#if !defined(VK_SWAPCHAIN_H)
#define VK_SWAPCHAIN_H

/* Includes */
#include <base.h>
#include <vk_surf.h>
#include <vk_phys_dev.h>
#include <vk_dev.h>

/* Types */
/* Vulkan swapchain builder */
typedef struct {
  uint32_t image_count;
  VkSurfaceFormatKHR format;
  VkExtent2D extent;
  uint32_t *queue_family_indices;
  uint32_t queue_family_index_count;
} vk_swapchain_builder_t;
/* Vulkan swapchain */
typedef VkSwapchainKHR vk_swapchain_t;

/* Create a Vulkan swapchain builder */
extern vk_swapchain_builder_t vk_swapchain_builder(void);
/* Set the image count */
extern void vk_swapchain_builder_set_image_count(
  vk_swapchain_builder_t *builder, uint32_t image_count
);
/* Set the image format */
extern void vk_swapchain_builder_set_format(
  vk_swapchain_builder_t *builder, VkSurfaceFormatKHR format
);
/* Set the image extent */
extern void vk_swapchain_builder_set_extent(
  vk_swapchain_builder_t *builder, VkExtent2D extent
);
/* Add a queue family index */
extern void vk_swapchain_builder_add_queue_family_index(
  vk_swapchain_builder_t *builder, uint32_t queue_family_index
);
/* Create a Vulkan swapchain (and free builder) */
extern vk_swapchain_t vk_swapchain_create(
  vk_dev_t *dev,
  vk_surf_t *surf,
  vk_swapchain_builder_t *builder
);
/* Destroy a Vulkan swapchain */
extern void vk_swapchain_destroy(vk_swapchain_t *swapchain, vk_dev_t *dev);

#endif /* VK_SWAPCHAIN_H */
