/* Include guard */
#include <vulkan/vulkan_core.h>
#if !defined(VK_PHYS_DEV_H)
#define VK_PHYS_DEV_H

/* Includes */
#include <base.h>
#include <vk_surf.h>

/* Types */
/* Vulkan physical device */
typedef VkPhysicalDevice vk_phys_dev_t;
/* Vulkan physical device information */
typedef struct {
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceFeatures features;
  VkPhysicalDeviceMemoryProperties memory_properties;
  VkSurfaceCapabilitiesKHR surface_capabilities;
  struct {
    uint32_t graphics_index;
    uint32_t present_index;
    uint32_t compute_index;
    uint32_t transfer_index;
    bool graphics_supported;
    bool present_supported;
    bool compute_supported;
    bool transfer_supported;
  } queue_families;
  VkExtensionProperties *extensions_supported;
  uint32_t extensions_supported_count;
  VkLayerProperties *layers_supported;
  uint32_t layers_supported_count;
  VkSurfaceFormatKHR *surface_formats;
  uint32_t surface_formats_count;
  VkPresentModeKHR *present_modes;
  uint32_t present_modes_count;
} vk_phys_dev_info_t;

/* Get a physical device's information */
extern void vk_phys_dev_get_info(
    VkPhysicalDevice device,
    vk_phys_dev_info_t *info,
    const vk_surf_t *surf
);
/* Free a physical device information structure */
extern void vk_phys_dev_info_free(vk_phys_dev_info_t *info);

#endif /* VK_PHYS_DEV_H */
