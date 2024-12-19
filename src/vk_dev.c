/* Implements vk_dev.h */
#include <vk_dev.h>
#include <vulkan/vulkan_core.h>

/* Add requested extension */
void vk_dev_create_info_add_extension(
    vk_dev_create_info_t *vk_dev,
    const char *extension
) {
  if (vk_dev->extensions == NULL) {
    vk_dev->extensions = malloc(sizeof(char *) * 1);
    ASSERT(vk_dev->extensions != NULL);
    vk_dev->extensions[0] = extension;
    vk_dev->num_extensions = 1;
    return;
  }
  vk_dev->extensions = realloc(
      vk_dev->extensions,
      sizeof(char *) * (vk_dev->num_extensions + 1)
  );
  ASSERT(vk_dev->extensions != NULL);
  vk_dev->extensions[vk_dev->num_extensions] = extension;
  vk_dev->num_extensions++;
}
/* Add requested layer */
void vk_dev_create_info_add_layer(
    vk_dev_create_info_t *vk_dev,
    const char *layer
) {
  if (vk_dev->layers == NULL) {
    vk_dev->layers = malloc(sizeof(char *) * 1);
    ASSERT(vk_dev->layers != NULL);
    vk_dev->layers[0] = layer;
    vk_dev->num_layers = 1;
    return;
  }
  vk_dev->layers = realloc(
      vk_dev->layers,
      sizeof(char *) * (vk_dev->num_layers + 1)
  );
  ASSERT(vk_dev->layers != NULL);
  vk_dev->layers[vk_dev->num_layers] = layer;
  vk_dev->num_layers++;
}
/* Initialize vulkan device create info */
void vk_dev_create_info_init(
    vk_dev_create_info_t *vk_dev_create_info
) {
  vk_dev_create_info->choose_dev = false;
  vk_dev_create_info->create_graphics_queue = false;
  vk_dev_create_info->create_compute_queue = false;
  vk_dev_create_info->create_present_queue = false;
  vk_dev_create_info->create_transfer_queue = false;
  vk_dev_create_info->features = (VkPhysicalDeviceFeatures){0};
  vk_dev_create_info->surface_format = (VkSurfaceFormatKHR){0};
  vk_dev_create_info->present_mode = (VkPresentModeKHR){0};
  vk_dev_create_info->extensions = NULL;
  vk_dev_create_info->num_extensions = 0;
  vk_dev_create_info->layers = NULL;
  vk_dev_create_info->num_layers = 0;
}
/* Free vulkan device create info */
void vk_dev_create_info_free(
    vk_dev_create_info_t *vk_dev_create_info
) {
  if (vk_dev_create_info->extensions != NULL)
    free(vk_dev_create_info->extensions);
  if (vk_dev_create_info->layers != NULL)
    free(vk_dev_create_info->layers);
}

/* Rate a physical device: 0 is unsuitable */
static uint32_t rate_physical_device(
    vk_dev_create_info_t *vk_dev_create_info,
    vk_surf_t *vk_surf,
    VkPhysicalDevice physical_device
) {
  VkExtensionProperties *supported_extensions = NULL;
  uint32_t num_supported_extensions = 0;
  VkLayerProperties *supported_layers = NULL;
  uint32_t num_supported_layers = 0;
  VkQueueFamilyProperties *queue_families = NULL;
  uint32_t num_queue_families = 0;
  VkSurfaceFormatKHR *surface_formats = NULL;
  uint32_t num_surface_formats = 0;
  VkPresentModeKHR *present_modes = NULL;
  uint32_t num_present_modes = 0;
  VkPhysicalDeviceFeatures supported_features;
  bool found_surface_format = false;
  bool found_present_mode = false;
  bool found_graphics_family = false;
  bool found_present_family = false;
  bool found_compute_family = false;
  bool found_transfer_family = false;
  VkPhysicalDeviceProperties properties;
  uint32_t score = 0;

  /* Check for support of extensions */
  VK_CHECK(vkEnumerateDeviceExtensionProperties(
      physical_device,
      NULL,
      &num_supported_extensions,
      NULL
  ));
  if (num_supported_extensions > 0) {
    supported_extensions =
      malloc(sizeof(VkExtensionProperties) * num_supported_extensions);
    ASSERT(supported_extensions != NULL);
    VK_CHECK(vkEnumerateDeviceExtensionProperties(
        physical_device,
        NULL,
        &num_supported_extensions,
        supported_extensions
    ));
  }
  for (uint32_t i = 0; i < vk_dev_create_info->num_extensions; i++) {
    bool found = false;
    for (uint32_t j = 0; j < num_supported_extensions; j++) {
      if (strcmp(
          vk_dev_create_info->extensions[i],
          supported_extensions[j].extensionName
      ) == 0) {
        found = true;
        break;
      }
    }
    if (!found) {
      return 0;
    }
  }
  free(supported_extensions);

  /* Check for support of layers */
  VK_CHECK(vkEnumerateDeviceLayerProperties(
      physical_device,
      &num_supported_layers,
      NULL
  ));
  if (num_supported_layers > 0) {
    supported_layers =
      malloc(sizeof(VkLayerProperties) * num_supported_layers);
    ASSERT(supported_layers != NULL);
    VK_CHECK(vkEnumerateDeviceLayerProperties(
        physical_device,
        &num_supported_layers,
        supported_layers
    ));
  }
  for (uint32_t i = 0; i < vk_dev_create_info->num_layers; i++) {
    bool found = false;
    for (uint32_t j = 0; j < num_supported_layers; j++) {
      if (strcmp(
          vk_dev_create_info->layers[i],
          supported_layers[j].layerName
      ) == 0) {
        found = true;
        break;
      }
    }
    if (!found) {
      return 0;
    }
  }
  free(supported_layers);

  /* Check features are all supported */
  vkGetPhysicalDeviceFeatures(
      physical_device,
      &supported_features
  );
  /* -> loop over both structures' bytes, and-ing them */
  for (uint32_t i = 0; i < sizeof(VkPhysicalDeviceFeatures); i++) {
    if ((
        ((uint8_t *)&vk_dev_create_info->features)[i]
        & ((uint8_t *)&supported_features)[i]
    ) == 0) {
      return 0;
    }
  }

  /* Check for queue families */
  vkGetPhysicalDeviceQueueFamilyProperties(
      physical_device,
      &num_queue_families,
      NULL
  );
  if (num_queue_families > 0) {
    queue_families =
      malloc(sizeof(VkQueueFamilyProperties) * num_queue_families);
    ASSERT(queue_families != NULL);
    vkGetPhysicalDeviceQueueFamilyProperties(
        physical_device,
        &num_queue_families,
        queue_families
    );
  }
  for (uint32_t i = 0; i < num_queue_families; i++) {
    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        physical_device,
        i,
        vk_surf->surface,
        &present_support
    );
    if (present_support) {
      found_present_family = true;
    }
    if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      found_graphics_family = true;
    }
    if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
      found_compute_family = true;
    }
    if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
      found_transfer_family = true;
    }
  }
  free(queue_families);
  if (
      !found_graphics_family
      || !found_present_family
      || !found_compute_family
      || !found_transfer_family
  ) {
    return 0;
  }

  /* Check for required surface format */
  vkGetPhysicalDeviceSurfaceFormatsKHR(
      physical_device,
      vk_surf->surface,
      &num_surface_formats,
      NULL
  );
  if (num_surface_formats > 0) {
    surface_formats =
      malloc(sizeof(VkSurfaceFormatKHR) * num_surface_formats);
    ASSERT(surface_formats != NULL);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        physical_device,
        vk_surf->surface,
        &num_surface_formats,
        surface_formats
    );
  }
  for (uint32_t i = 0; i < num_surface_formats; i++) {
    if (
        surface_formats[i].format == vk_dev_create_info->surface_format.format
        && surface_formats[i].colorSpace
        == vk_dev_create_info->surface_format.colorSpace
    ) {
      found_surface_format = true;
      break;
    }
  }
  free(surface_formats);
  if (!found_surface_format) {
    return 0;
  }

  /* Check for required present mode */
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      physical_device,
      vk_surf->surface,
      &num_present_modes,
      NULL
  );
  if (num_present_modes > 0) {
    present_modes =
      malloc(sizeof(VkPresentModeKHR) * num_present_modes);
    ASSERT(present_modes != NULL);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        physical_device,
        vk_surf->surface,
        &num_present_modes,
        present_modes
    );
  }
  for (uint32_t i = 0; i < num_present_modes; i++) {
    if (present_modes[i] == vk_dev_create_info->present_mode) {
      found_present_mode = true;
      break;
    }
  }
  free(present_modes);
  if (!found_present_mode) {
    return 0;
  }

  /* Get properties */
  vkGetPhysicalDeviceProperties(physical_device, &properties);
  switch (properties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
      score += 1000;
      break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
      score += 500;
      break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
      score += 250;
      break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
      break;
    case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
      log_msg(LOG_LEVEL_WARN, "One of the devices doesn't seem right...");
      break;
  }

  return score;
}

/* Create vulkan device */
void vk_dev_create(
    vk_dev_t *vk_dev,
    vk_inst_t *vk_inst,
    vk_surf_t *vk_surf,
    vk_dev_create_info_t *vk_dev_create_info
);
/* Destroy vulkan device */
void vk_dev_destroy(
    vk_dev_t *vk_dev
) {
  vkDestroyDevice(vk_dev->device, NULL);
  vk_dev->physical_device = VK_NULL_HANDLE;
  vk_dev->device = VK_NULL_HANDLE;
  vk_dev->graphics_queue = VK_NULL_HANDLE;
  vk_dev->present_queue = VK_NULL_HANDLE;
  vk_dev->compute_queue = VK_NULL_HANDLE;
  vk_dev->transfer_queue = VK_NULL_HANDLE;
  vk_dev->graphics_queue_family = 0;
  vk_dev->present_queue_family = 0;
  vk_dev->compute_queue_family = 0;
  vk_dev->transfer_queue_family = 0;

  /* Print message */
  log_msg(LOG_LEVEL_SUCCESS, "Vulkan device destroyed");
}
