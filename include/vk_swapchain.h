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
  VkExtent2D extent;
  uint32_t image_count;
  uint32_t image_array_layers;
  VkSurfaceFormatKHR format;
  VkPresentModeKHR present_mode;
  VkSwapchainKHR old_swapchain;
  VkImageUsageFlags image_usage;
  VkSurfaceTransformFlagBitsKHR pre_transform;
  VkCompositeAlphaFlagsKHR composite_alpha;
  bool clipped;
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
/* Set the present mode */
extern void vk_swapchain_builder_set_present_mode(
  vk_swapchain_builder_t *builder, VkPresentModeKHR present_mode
);
/* Set the image extent */
extern void vk_swapchain_builder_set_extent(
  vk_swapchain_builder_t *builder, uint32_t width, uint32_t height
);
/* Add a queue family index */
extern void vk_swapchain_builder_add_queue_family_index(
  vk_swapchain_builder_t *builder, uint32_t queue_family_index
);
/* Set the image array layer count */
extern void vk_swapchain_builder_set_image_array_layers(
  vk_swapchain_builder_t *builder, uint32_t image_array_layers
);
/* Set the old swapchain handle */
extern void vk_swapchain_builder_set_old_swapchain(
  vk_swapchain_builder_t *builder, VkSwapchainKHR old_swapchain
);
/* Set the image usage */
extern void vk_swapchain_builder_set_image_usage(
  vk_swapchain_builder_t *builder, VkImageUsageFlags image_usage
);
/* Set the clipped parameter */
extern void vk_swapchain_builder_set_clipped(
  vk_swapchain_builder_t *builder, bool clipped
);
/* Set the pre transform operation */
extern void vk_swapchain_builder_set_pre_transform(
  vk_swapchain_builder_t *builder, VkSurfaceTransformFlagBitsKHR pre_transform
);
/* Set the composite alpha operation */
extern void vk_swapchain_builder_set_composite_alpha(
  vk_swapchain_builder_t *builder, VkCompositeAlphaFlagsKHR composite_alpha
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
