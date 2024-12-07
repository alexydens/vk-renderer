/* Implements vkphysdev.h */
#include <time.h>
#include <vkphysdev.h>
#include <vulkan/vulkan_core.h>

/* Score a device */
static inline uint32_t vkphysdev_score(
  VkPhysicalDevice device
) {
  uint32_t score = 0;
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceFeatures features;
  VkPhysicalDeviceMemoryProperties memory_properties;

  /* Get information */
  vkGetPhysicalDeviceProperties(device, &properties);
  vkGetPhysicalDeviceFeatures(device, &features);
  vkGetPhysicalDeviceMemoryProperties(device, &memory_properties);

  /* Easy to discount */
  if (device == VK_NULL_HANDLE) return 0;
  if (!features.geometryShader) return 0;

  /* GPU type */
  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 2000;
  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
    score += 1000;

  /* Other */
  score += memory_properties.memoryHeapCount * 100;
  score += properties.limits.maxFramebufferWidth * 100;
  score += properties.limits.maxFramebufferHeight * 100;
  score += properties.limits.maxImageDimension2D * 10;
  score += properties.limits.maxImageDimension3D * 1;

  return score;
}

/* Pick a physical devices */
void vkphysdev_pick(
    vkphysdev_t *physical_device,
    vkinst_t *instance,
    vksurface_t *surface
) {
  VkPhysicalDevice *devices;
  uint32_t num_devices;
  uint32_t max_score = 0;
  uint32_t max_index = 0;
  VkQueueFamilyProperties *queue_families;
  uint32_t queue_family_count;

  /* Get all physical devices */
  vkEnumeratePhysicalDevices(
      instance->instance,
      &num_devices,
      NULL
  );
  if (!num_devices) {
    fprintf(
        stderr,
        "ERROR: No physical devices found\n"
    );
    exit(1);
  }
  devices = malloc(num_devices * sizeof(VkPhysicalDevice));
  if (!devices) {
    fprintf(
        stderr,
        "ERROR: malloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  vkEnumeratePhysicalDevices(
      instance->instance,
      &num_devices,
      devices
  );

  /* Choose device */
  for (uint32_t i = 0; i < num_devices; i++) {
    uint32_t score = vkphysdev_score(devices[i]);
    if (score > max_score) {
      max_score = score;
      max_index = i;
    }
  }
  if (!max_score) {
    fprintf(
        stderr,
        "ERROR: No suitable physical device found\n"
    );
    exit(1);
  }
  physical_device->physical_device = devices[max_index];

  /* Get everything in the struct */
  vkGetPhysicalDeviceProperties(
      physical_device->physical_device,
      &physical_device->properties
  );
  vkGetPhysicalDeviceMemoryProperties(
      physical_device->physical_device,
      &physical_device->memory_properties
  );
  vkGetPhysicalDeviceFeatures(
      physical_device->physical_device,
      &physical_device->features
  );
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      physical_device->physical_device,
      surface->surface,
      &physical_device->surf_capabilities
  );

  /* Get surface formats */
  vkGetPhysicalDeviceSurfaceFormatsKHR(
      physical_device->physical_device,
      surface->surface,
      &physical_device->surf_formats_count,
      NULL
  );
  physical_device->surf_formats = malloc(
      physical_device->surf_formats_count * sizeof(VkSurfaceFormatKHR)
  );
  if (!physical_device->surf_formats) {
    fprintf(
        stderr,
        "ERROR: malloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  vkGetPhysicalDeviceSurfaceFormatsKHR(
      physical_device->physical_device,
      surface->surface,
      &physical_device->surf_formats_count,
      physical_device->surf_formats
  );

  /* Get present modes */
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      physical_device->physical_device,
      surface->surface,
      &physical_device->present_modes_count,
      NULL
  );
  physical_device->present_modes = malloc(
      physical_device->present_modes_count * sizeof(VkPresentModeKHR)
  );
  if (!physical_device->present_modes) {
    fprintf(
        stderr,
        "ERROR: malloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      physical_device->physical_device,
      surface->surface,
      &physical_device->present_modes_count,
      physical_device->present_modes
  );

  /* Get queue families */
  vkGetPhysicalDeviceQueueFamilyProperties(
      physical_device->physical_device,
      &queue_family_count,
      NULL
  );
  queue_families = malloc(
      queue_family_count * sizeof(VkQueueFamilyProperties)
  );
  if (!queue_families) {
    fprintf(
        stderr,
        "ERROR: malloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  vkGetPhysicalDeviceQueueFamilyProperties(
      physical_device->physical_device,
      &queue_family_count,
      queue_families
  );
  bool found_graphics = false;
  bool found_present = false;
  for (uint32_t i = 0; i < queue_family_count; i++) {
    if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      physical_device->graphics_queue_index = i;
      found_graphics = true;
    }
    VkBool32 present_support = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        physical_device->physical_device,
        i,
        surface->surface,
        &present_support
    );
    if (present_support) {
      physical_device->present_queue_index = i;
      found_present = true;
    }
  }
  free(queue_families);
  if (!found_graphics || !found_present) {
    fprintf(
        stderr,
        "ERROR: No suitable queue families found\n"
    );
    exit(1);
  }

  /* Print message */
  printf("INFO: Chose device: %s\n", physical_device->properties.deviceName);
}
