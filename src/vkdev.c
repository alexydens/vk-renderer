/* Implements vkdev.h */
#include <vkdev.h>

/* Add another requested extension */
void vkdev_add_extension(
    vkdev_t *device,
    const char *extension
) {
  if (device->extensions == NULL) {
    device->extensions = (const char **)malloc(sizeof(char *));
    if (!device->extensions) {
      fprintf(
          stderr,
          "ERROR: malloc() failed: %s\n",
          strerror(errno)
      );
      exit(1);
    }
    device->extensions[0] = extension;
    device->num_extensions = 1;
    return;
  }
  device->extensions = (const char **)realloc(
      device->extensions,
      (device->num_extensions + 1) * sizeof(char *)
  );
  if (!device->extensions) {
    fprintf(
        stderr,
        "ERROR: realloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  device->extensions[device->num_extensions] = extension;
  device->num_extensions++;
}
/* Add another requested layer */
void vkdev_add_layer(
    vkdev_t *device,
    const char *layer
) {
  if (device->layers == NULL) {
    device->layers = (const char **)malloc(sizeof(char *));
    if (!device->layers) {
      fprintf(
          stderr,
          "ERROR: malloc() failed: %s\n",
          strerror(errno)
      );
      exit(1);
    }
    device->layers[0] = layer;
    device->num_layers = 1;
    return;
  }
  device->layers = (const char **)realloc(
      device->layers,
      (device->num_layers + 1) * sizeof(char *)
  );
  if (!device->layers) {
    fprintf(
        stderr,
        "ERROR: realloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  device->layers[device->num_layers] = layer;
  device->num_layers++;
}

/* Initialize a vulkan device */
void vkdev_init(vkdev_t *device) {
  device->device = VK_NULL_HANDLE;
  device->physical_device = VK_NULL_HANDLE;
  device->extensions = NULL;
  device->num_extensions = 0;
  device->layers = NULL;
  device->num_layers = 0;
  device->graphics_queue = VK_NULL_HANDLE;
  device->present_queue = VK_NULL_HANDLE;
}
/* Create a vulkan device */
void vkdev_create(
    vkdev_t *device,
    vkphysdev_t *physical_device
) {
  VkDeviceQueueCreateInfo queue_create_infos[2];
  VkDeviceCreateInfo create_info;
  VkPhysicalDeviceFeatures features_used;
  float queue_priority = 1.0f;
  VkExtensionProperties *vk_supported_extensions;
  uint32_t num_vk_supported_extensions;
  VkLayerProperties *vk_supported_layers;
  uint32_t num_vk_supported_layers;

  /* Get the extensions vulkan supports */
  vkEnumerateDeviceExtensionProperties(
      physical_device->physical_device,
      NULL,
      &num_vk_supported_extensions,
      NULL
  );
  vk_supported_extensions = (VkExtensionProperties *)malloc(
      num_vk_supported_extensions * sizeof(VkExtensionProperties)
  );
  if (!vk_supported_extensions) {
    fprintf(
        stderr,
        "ERROR: malloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  vkEnumerateDeviceExtensionProperties(
      physical_device->physical_device,
      NULL,
      &num_vk_supported_extensions,
      vk_supported_extensions
  );

  /* Check support */
  for (uint32_t i = 0; i < device->num_extensions; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < num_vk_supported_extensions; j++) {
      if (strcmp(
          device->extensions[i],
          vk_supported_extensions[j].extensionName
      ) == 0) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      fprintf(
          stderr,
          "ERROR: Extension '%s' not supported\n",
          device->extensions[i]
      );
      exit(1);
    }
    printf("INFO: Device extension '%s' supported and used\n", device->extensions[i]);
  }

  /* Get the layers vulkan supports */
  vkEnumerateDeviceLayerProperties(
      physical_device->physical_device,
      &num_vk_supported_layers,
      NULL
  );
  vk_supported_layers = (VkLayerProperties *)malloc(
      num_vk_supported_layers * sizeof(VkLayerProperties)
  );
  if (!vk_supported_layers) {
    fprintf(
        stderr,
        "ERROR: malloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  vkEnumerateDeviceLayerProperties(
      physical_device->physical_device,
      &num_vk_supported_layers,
      vk_supported_layers
  );

  /* Check support */
  for (uint32_t i = 0; i < device->num_layers; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < num_vk_supported_layers; j++) {
      if (strcmp(
          device->layers[i],
          vk_supported_layers[j].layerName
      ) == 0) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      fprintf(
          stderr,
          "ERROR: Layer '%s' not supported\n",
          device->layers[i]
      );
      exit(1);
    }
    printf("INFO: Device layer '%s' supported and used\n", device->layers[i]);
  }

  /* Populate queue create infos */
  queue_create_infos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_infos[0].pNext = NULL;
  queue_create_infos[0].flags = 0;
  queue_create_infos[0].queueFamilyIndex =
    physical_device->graphics_queue_index;
  queue_create_infos[0].pQueuePriorities = &queue_priority;
  queue_create_infos[0].queueCount = 1;
  queue_create_infos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_infos[1].pNext = NULL;
  queue_create_infos[1].flags = 0;
  queue_create_infos[1].queueFamilyIndex =
    physical_device->present_queue_index;
  queue_create_infos[1].pQueuePriorities = &queue_priority;
  queue_create_infos[1].queueCount = 1;

  /* Populate features used */
  if (memset(
        &features_used,
        0,
        sizeof(VkPhysicalDeviceFeatures)
  ) != &features_used) {
    fprintf(
        stderr,
        "ERROR: memset() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }

  /* Populate device create info */
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pNext = NULL;
  create_info.flags = 0;
  create_info.pQueueCreateInfos = queue_create_infos;
  create_info.queueCreateInfoCount = 2;
  create_info.pEnabledFeatures = &features_used;
  create_info.ppEnabledExtensionNames = device->extensions;
  create_info.enabledExtensionCount = device->num_extensions;
  create_info.ppEnabledLayerNames = device->layers;
  create_info.enabledLayerCount = device->num_layers;

  /* Create the device */
  if (vkCreateDevice(
        physical_device->physical_device,
        &create_info,
        NULL,
        &device->device
  ) != VK_SUCCESS) {
    fprintf(
        stderr,
        "ERROR: vkCreateDevice() failed\n"
    );
    exit(1);
  }

  /* Print messsage */
  printf("INFO: Vulkan device created\n");

  /* Get queues */
  vkGetDeviceQueue(
      device->device,
      physical_device->graphics_queue_index,
      0,
      &device->graphics_queue
  );
  vkGetDeviceQueue(
      device->device,
      physical_device->present_queue_index,
      0,
      &device->present_queue
  );

  /* Print message */
  printf("INFO: Successfully got device queues\n");
}
/* Destroy a vulkan device */
void vkdev_destroy(vkdev_t *device) {
  vkDestroyDevice(device->device, NULL);
  if (device->extensions) free(device->extensions);
  device->num_extensions = 0;
  if (device->layers) free(device->layers);
  device->num_layers = 0;
  device->device = VK_NULL_HANDLE;

  /* Print message */
  printf("INFO: Successfully destroyed vulkan device\n");
}
