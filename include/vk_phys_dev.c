/* Implements vk_phys_dev.h */
#include <vk_phys_dev.h>
#include <vulkan/vulkan_core.h>

/* Get a physical device's information */
void vk_phys_dev_get_info(
    VkPhysicalDevice device,
    vk_phys_dev_info_t *info,
    const vk_surf_t *surf
) {
  VkQueueFamilyProperties *queue_families = NULL;
  uint32_t queue_family_count = 0;

  /* Get properties */
  vkGetPhysicalDeviceProperties(device, &info->properties);
  vkGetPhysicalDeviceFeatures(device, &info->features);
  vkGetPhysicalDeviceMemoryProperties(device, &info->memory_properties);

  /* Get surface capabilities */
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      device,
      *surf,
      &info->surface_capabilities
  );

  /* Get queue family indices */
  vkGetPhysicalDeviceQueueFamilyProperties(
      device,
      &queue_family_count,
      NULL
  );
  memset(&(info->queue_families), 0, sizeof(info->queue_families));
  if (queue_family_count > 0) {
    queue_families = (VkQueueFamilyProperties *)malloc(
        sizeof(VkQueueFamilyProperties) * queue_family_count
    );
    ASSERT(queue_families);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &queue_family_count,
        queue_families
    );
    for (uint32_t i = 0; i < queue_family_count; i++) {
      VkBool32 present_support = VK_FALSE;
      vkGetPhysicalDeviceSurfaceSupportKHR(
          device,
          i,
          *surf,
          &present_support
      );
      if (present_support == VK_TRUE) {
        info->queue_families.present_index = i;
        info->queue_families.present_supported = true;
      }
      if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        info->queue_families.graphics_index = i;
        info->queue_families.graphics_supported = true;
      }
      if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
        info->queue_families.compute_index = i;
        info->queue_families.compute_supported = true;
      }
      if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
        info->queue_families.transfer_index = i;
        info->queue_families.transfer_supported = true;
      }
    }
    free(queue_families);
  }

  /* Get extensions supported */
  vkEnumerateDeviceExtensionProperties(
      device,
      NULL,
      &info->extensions_supported_count,
      NULL
  );
  if (info->extensions_supported_count > 0) {
    info->extensions_supported = (VkExtensionProperties *)malloc(
        sizeof(VkExtensionProperties) * info->extensions_supported_count
    );
    ASSERT(info->extensions_supported);
    vkEnumerateDeviceExtensionProperties(
        device,
        NULL,
        &info->extensions_supported_count,
        info->extensions_supported
    );
  }
  /* Get layers supported */
  vkEnumerateDeviceLayerProperties(
      device,
      &info->layers_supported_count,
      NULL
  );
  if (info->layers_supported_count > 0) {
    info->layers_supported = (VkLayerProperties *)malloc(
        sizeof(VkLayerProperties) * info->layers_supported_count
    );
    ASSERT(info->layers_supported);
    vkEnumerateDeviceLayerProperties(
        device,
        &info->layers_supported_count,
        info->layers_supported
    );
  }
  /* Get surface formats */
  vkGetPhysicalDeviceSurfaceFormatsKHR(
      device,
      *surf,
      &info->surface_formats_count,
      NULL
  );
  if (info->surface_formats_count > 0) {
    info->surface_formats = (VkSurfaceFormatKHR *)malloc(
        sizeof(VkSurfaceFormatKHR) * info->surface_formats_count
    );
    ASSERT(info->surface_formats);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device,
        *surf,
        &info->surface_formats_count,
        info->surface_formats
    );
  }
  /* Get present modes */
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      device,
      *surf,
      &info->present_modes_count,
      NULL
  );
  if (info->present_modes_count > 0) {
    info->present_modes = (VkPresentModeKHR *)malloc(
        sizeof(VkPresentModeKHR) * info->present_modes_count
    );
    ASSERT(info->present_modes);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device,
        *surf,
        &info->present_modes_count,
        info->present_modes
    );
  }

}
/* Free a physical device information structure */
void vk_phys_dev_info_free(vk_phys_dev_info_t *info) {
  if (info->extensions_supported) free(info->extensions_supported);
  if (info->layers_supported) free(info->layers_supported);
  if (info->surface_formats) free(info->surface_formats);
  if (info->present_modes) free(info->present_modes);
  memset(info, 0, sizeof(vk_phys_dev_info_t));
}
