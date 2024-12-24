/* Implements vk_swapchain.h */
#include <vk_swapchain.h>

/* Create a Vulkan swapchain builder */
vk_swapchain_builder_t vk_swapchain_builder(void) {
  vk_swapchain_builder_t builder;
  builder.queue_family_indices = NULL;
  builder.queue_family_index_count = 0;
  builder.image_count = 0;
  builder.format.format = VK_FORMAT_UNDEFINED;
  builder.format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  builder.extent.width = 0;
  builder.extent.height = 0;
  return builder;
}
/* Set the image count */
void vk_swapchain_builder_set_image_count(
  vk_swapchain_builder_t *builder, uint32_t image_count
) {
  builder->image_count = image_count;
}
/* Set the image format */
void vk_swapchain_builder_set_format(
  vk_swapchain_builder_t *builder, VkSurfaceFormatKHR format
) {
  builder->format = format;
}
/* Set the image extent */
void vk_swapchain_builder_set_extent(
  vk_swapchain_builder_t *builder, VkExtent2D extent
) {
  builder->extent = extent;
}
/* Add a queue family index */
void vk_swapchain_builder_add_queue_family_index(
  vk_swapchain_builder_t *builder, uint32_t queue_family_index
) {
  if (!builder->queue_family_indices) {
    builder->queue_family_indices = (uint32_t *)malloc(sizeof(uint32_t));
    ASSERT(builder->queue_family_indices);
    builder->queue_family_index_count = 1;
    builder->queue_family_indices[0] = queue_family_index;
    return;
  }
  builder->queue_family_index_count++;
  builder->queue_family_indices = (uint32_t *)realloc(
    builder->queue_family_indices,
    sizeof(uint32_t) * builder->queue_family_index_count
  );
  ASSERT(builder->queue_family_indices);
  builder->queue_family_indices[builder->queue_family_index_count - 1] =
    queue_family_index;
}
/* Create a Vulkan swapchain (and free builder) */
vk_swapchain_t vk_swapchain_create(
  vk_dev_t *dev,
  vk_surf_t *surf,
  vk_swapchain_builder_t *builder
) {
  VkSwapchainCreateInfoKHR swapchain_create_info;
  vk_swapchain_t swapchain;

  /* Populate swapchain create info */
  swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchain_create_info.pNext = NULL;
  swapchain_create_info.flags = 0;
  swapchain_create_info.surface = *surf;
  swapchain_create_info.minImageCount = builder->image_count;
  swapchain_create_info.imageFormat = builder->format.format;
  swapchain_create_info.imageColorSpace = builder->format.colorSpace;
  swapchain_create_info.imageExtent = builder->extent;
  swapchain_create_info.imageArrayLayers = 1;
  if (builder->queue_family_index_count > 0) {
    swapchain_create_info.pQueueFamilyIndices = builder->queue_family_indices;
    swapchain_create_info.queueFamilyIndexCount =
      builder->queue_family_index_count;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
  } else {
    swapchain_create_info.pQueueFamilyIndices = NULL;
    swapchain_create_info.queueFamilyIndexCount = 0;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }
  /* Create swapchain */
  VK_CHECK(vkCreateSwapchainKHR(
        dev->device,
        &swapchain_create_info,
        NULL,
        &swapchain
  ));

  /* Free builder */
  if (builder->queue_family_indices) free(builder->queue_family_indices);
  memset(builder, 0, sizeof(vk_swapchain_builder_t));

  return swapchain;
}
/* Destroy a Vulkan swapchain */
void vk_swapchain_destroy(vk_swapchain_t *swapchain, vk_dev_t *dev) {
  vkDestroySwapchainKHR(dev->device, *swapchain, NULL);
}
