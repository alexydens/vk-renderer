/* Implements vk_swapchain.h */
#include <vk_swapchain.h>
#include <vulkan/vulkan_core.h>

/* Create a Vulkan swapchain builder */
vk_swapchain_builder_t vk_swapchain_builder(void) {
  vk_swapchain_builder_t builder;
  builder.queue_family_indices = NULL;
  builder.queue_family_index_count = 0;
  builder.image_count = 0;
  builder.format.format = VK_FORMAT_UNDEFINED;
  builder.format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  builder.present_mode = VK_PRESENT_MODE_FIFO_KHR;
  builder.extent.width = 0;
  builder.extent.height = 0;
  builder.image_array_layers = 1;
  builder.old_swapchain = VK_NULL_HANDLE;
  builder.image_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  builder.clipped = true;
  builder.pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  builder.composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
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
/* Set the present mode */
void vk_swapchain_builder_set_present_mode(
  vk_swapchain_builder_t *builder, VkPresentModeKHR present_mode
) {
  builder->present_mode = present_mode;
}
/* Set the image extent */
void vk_swapchain_builder_set_extent(
  vk_swapchain_builder_t *builder, uint32_t width, uint32_t height
) {
  builder->extent.width = width;
  builder->extent.height = height;
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
/* Set the image array layer count */
void vk_swapchain_builder_set_image_array_layers(
  vk_swapchain_builder_t *builder, uint32_t image_array_layers
) {
  builder->image_array_layers = image_array_layers;
}
/* Set the old swapchain handle */
void vk_swapchain_builder_set_old_swapchain(
  vk_swapchain_builder_t *builder, VkSwapchainKHR old_swapchain
) {
  builder->old_swapchain = old_swapchain;
}
/* Set the image usage */
void vk_swapchain_builder_set_image_usage(
  vk_swapchain_builder_t *builder, VkImageUsageFlags image_usage
) {
  builder->image_usage = image_usage;
}
/* Set the clipped parameter */
void vk_swapchain_builder_set_clipped(
  vk_swapchain_builder_t *builder, bool clipped
) {
  builder->clipped = clipped;
}
/* Set the pre transform operation */
void vk_swapchain_builder_set_pre_transform(
  vk_swapchain_builder_t *builder, VkSurfaceTransformFlagBitsKHR pre_transform
) {
  builder->pre_transform = pre_transform;
}
/* Set the composite alpha operation */
void vk_swapchain_builder_set_composite_alpha(
  vk_swapchain_builder_t *builder, VkCompositeAlphaFlagsKHR composite_alpha
) {
  builder->composite_alpha = composite_alpha;
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
  swapchain_create_info.presentMode = builder->present_mode;
  swapchain_create_info.minImageCount = builder->image_count;
  swapchain_create_info.imageFormat = builder->format.format;
  swapchain_create_info.imageColorSpace = builder->format.colorSpace;
  swapchain_create_info.imageExtent = builder->extent;
  swapchain_create_info.imageArrayLayers = builder->image_array_layers;
  swapchain_create_info.oldSwapchain = builder->old_swapchain;
  swapchain_create_info.imageUsage = builder->image_usage;
  swapchain_create_info.clipped = builder->clipped ? VK_TRUE : VK_FALSE;
  swapchain_create_info.preTransform = builder->pre_transform;
  swapchain_create_info.compositeAlpha = builder->composite_alpha;
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
        &swapchain.swapchain
  ));

  /* Get all images */
  VK_CHECK(vkGetSwapchainImagesKHR(
        dev->device,
        swapchain.swapchain,
        &swapchain.image_count,
        NULL
  ));
  swapchain.images = (VkImage *)malloc(sizeof(VkImage) * swapchain.image_count);
  ASSERT(swapchain.images);
  VK_CHECK(vkGetSwapchainImagesKHR(
        dev->device,
        swapchain.swapchain,
        &swapchain.image_count,
        swapchain.images
  ));

  /* Free builder */
  if (builder->queue_family_indices) free(builder->queue_family_indices);
  memset(builder, 0, sizeof(vk_swapchain_builder_t));

  return swapchain;
}
/* Destroy a Vulkan swapchain */
void vk_swapchain_destroy(vk_swapchain_t *swapchain, vk_dev_t *dev) {
  vkDestroySwapchainKHR(dev->device, swapchain->swapchain, NULL);
  if (swapchain->images) free(swapchain->images);
  memset(swapchain, 0, sizeof(vk_swapchain_t));
}
