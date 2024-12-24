/* Implements vk_dev.h */
#include <vk_dev.h>

/* Create a Vulkan device builder */
vk_dev_builder_t vk_dev_builder(void) {
  vk_dev_builder_t builder;
  builder.extensions = NULL;
  builder.extension_count = 0;
  builder.layers = NULL;
  builder.layer_count = 0;
  builder.graphics_queues = 0;
  builder.graphics_queue_priorities = NULL;
  builder.present_queues = 0;
  builder.present_queue_priorities = NULL;
  builder.compute_queues = 0;
  builder.compute_queue_priorities = NULL;
  builder.transfer_queues = 0;
  builder.transfer_queue_priorities = NULL;
  memset(&builder.features, 0, sizeof(VkPhysicalDeviceFeatures));
  return builder;
}
/* Add a Vulkan device extension */
void vk_dev_builder_add_ext(
    vk_dev_builder_t *builder,
    const char *ext
) {
  if (!builder->extensions) {
    builder->extensions = (const char **)malloc(sizeof(const char *));
    ASSERT(builder->extensions);
    builder->extension_count = 1;
    builder->extensions[0] = ext;
    return;
  }
  builder->extension_count++;
  builder->extensions = (const char **)realloc(
    builder->extensions, sizeof(const char *) * builder->extension_count
  );
  ASSERT(builder->extensions);
  builder->extensions[builder->extension_count - 1] = ext;
}
/* Add a Vulkan device layer */
void vk_dev_builder_add_layer(
    vk_dev_builder_t *builder,
    const char *layer
) {
  if (!builder->layers) {
    builder->layers = (const char **)malloc(sizeof(const char *));
    ASSERT(builder->layers);
    builder->layer_count = 1;
    builder->layers[0] = layer;
    return;
  }
  builder->layer_count++;
  builder->layers = (const char **)realloc(
    builder->layers, sizeof(const char *) * builder->layer_count
  );
  ASSERT(builder->layers);
  builder->layers[builder->layer_count - 1] = layer;
}
/* Add a Vulkan device graphics queue */
void vk_dev_builder_add_graphics_queue(
    vk_dev_builder_t *builder,
    float priority
) {
  if (!builder->graphics_queue_priorities) {
    builder->graphics_queue_priorities = (float *)malloc(sizeof(float));
    ASSERT(builder->graphics_queue_priorities);
    builder->graphics_queues = 1;
    builder->graphics_queue_priorities[0] = priority;
    return;
  }
  builder->graphics_queues++;
  builder->graphics_queue_priorities = (float *)realloc(
    builder->graphics_queue_priorities,
    sizeof(float) * builder->graphics_queues
  );
  ASSERT(builder->graphics_queue_priorities);
  builder->graphics_queue_priorities[builder->graphics_queues - 1] = priority;
}
/* Add a Vulkan device present queue */
void vk_dev_builder_add_present_queue(
    vk_dev_builder_t *builder,
    float priority
) {
  if (!builder->present_queue_priorities) {
    builder->present_queue_priorities = (float *)malloc(sizeof(float));
    ASSERT(builder->present_queue_priorities);
    builder->present_queues = 1;
    builder->present_queue_priorities[0] = priority;
    return;
  }
  builder->present_queues++;
  builder->present_queue_priorities = (float *)realloc(
    builder->present_queue_priorities,
    sizeof(float) * builder->present_queues
  );
  ASSERT(builder->present_queue_priorities);
  builder->present_queue_priorities[builder->present_queues - 1] = priority;
}
/* Add a Vulkan device compute queue */
void vk_dev_builder_add_compute_queue(
    vk_dev_builder_t *builder,
    float priority
) {
  if (!builder->compute_queue_priorities) {
    builder->compute_queue_priorities = (float *)malloc(sizeof(float));
    ASSERT(builder->compute_queue_priorities);
    builder->compute_queues = 1;
    builder->compute_queue_priorities[0] = priority;
    return;
  }
  builder->compute_queues++;
  builder->compute_queue_priorities = (float *)realloc(
    builder->compute_queue_priorities,
    sizeof(float) * builder->compute_queues
  );
  ASSERT(builder->compute_queue_priorities);
  builder->compute_queue_priorities[builder->compute_queues - 1] = priority;
}
/* Add a Vulkan device transfer queue */
void vk_dev_builder_add_transfer_queue(
    vk_dev_builder_t *builder,
    float priority
) {
  if (!builder->transfer_queue_priorities) {
    builder->transfer_queue_priorities = (float *)malloc(sizeof(float));
    ASSERT(builder->transfer_queue_priorities);
    builder->transfer_queues = 1;
    builder->transfer_queue_priorities[0] = priority;
    return;
  }
  builder->transfer_queues++;
  builder->transfer_queue_priorities = (float *)realloc(
    builder->transfer_queue_priorities,
    sizeof(float) * builder->transfer_queues
  );
  ASSERT(builder->transfer_queue_priorities);
  builder->transfer_queue_priorities[builder->transfer_queues - 1] = priority;
}
/* Add Vulkan device features */
void vk_dev_builder_add_features(
    vk_dev_builder_t *builder,
    VkPhysicalDeviceFeatures features
) {
  builder->features = features;
}
/* Create a Vulkan device (and free builder) */
vk_dev_t vk_dev_create(
    vk_phys_dev_t *phys_dev,
    vk_phys_dev_info_t *phys_dev_info,
    vk_dev_builder_t *builder
) {
  VkDeviceCreateInfo dev_create_info;
  VkDeviceQueueCreateInfo queue_create_infos[4];
  uint32_t cur = 0;
  vk_dev_t dev;

  /* Check extensions are present */
  for (uint32_t i = 0; i < builder->extension_count; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < phys_dev_info->extensions_supported_count; j++) {
      if (strcmp(
            builder->extensions[i],
            phys_dev_info->extensions_supported[j].extensionName
      ) == 0) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      log_msg(
        LOG_LEVEL_ERROR,
        "Device extension %s is not supported",
        builder->extensions[i]
      );
      abort();
    }
  }
  /* Check layers are present */
  for (uint32_t i = 0; i < builder->layer_count; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < phys_dev_info->layers_supported_count; j++) {
      if (strcmp(
            builder->layers[i],
            phys_dev_info->layers_supported[j].layerName
      ) == 0) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      log_msg(
        LOG_LEVEL_ERROR,
        "Device layer %s is not supported",
        builder->layers[i]
      );
      abort();
    }
  }

  /* Populate device */
  dev.device = VK_NULL_HANDLE;
  dev.graphics_queues = NULL;
  dev.graphics_queue_count = 0;
  dev.present_queues = NULL;
  dev.present_queue_count = 0;
  dev.compute_queues = NULL;
  dev.compute_queue_count = 0;
  dev.transfer_queues = NULL;
  dev.transfer_queue_count = 0;

  /* Check there aren't too many requested queues */
  if (
      builder->graphics_queues
      > phys_dev_info->queue_families.max_graphics_queues
  ) {
    log_msg(LOG_LEVEL_ERROR, "Too many graphics queues requested");
    abort();
  }
  if (
      builder->present_queues
      > phys_dev_info->queue_families.max_present_queues
  ) {
    log_msg(LOG_LEVEL_ERROR, "Too many present queues requested");
    abort();
  }
  if (
      builder->compute_queues
      > phys_dev_info->queue_families.max_compute_queues
  ) {
    log_msg(LOG_LEVEL_ERROR, "Too many compute queues requested");
    abort();
  }
  if (
      builder->transfer_queues
      > phys_dev_info->queue_families.max_transfer_queues
  ) {
    log_msg(LOG_LEVEL_ERROR, "Too many transfer queues requested");
    abort();
  }

  /* Populate queue create infos */
  if (builder->graphics_queues > 0) {
    queue_create_infos[cur].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[cur].pNext = NULL;
    queue_create_infos[cur].flags = 0;
    queue_create_infos[cur].queueFamilyIndex
      = phys_dev_info->queue_families.graphics_index;
    queue_create_infos[cur].queueCount = builder->graphics_queues;
    queue_create_infos[cur].pQueuePriorities =
      builder->graphics_queue_priorities;
    cur++;
  }
  if (builder->present_queues > 0) {
    queue_create_infos[cur].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[cur].pNext = NULL;
    queue_create_infos[cur].flags = 0;
    queue_create_infos[cur].queueFamilyIndex
      = phys_dev_info->queue_families.present_index;
    queue_create_infos[cur].queueCount = builder->present_queues;
    queue_create_infos[cur].pQueuePriorities =
      builder->present_queue_priorities;
    cur++;
  }
  if (builder->compute_queues > 0) {
    queue_create_infos[cur].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[cur].pNext = NULL;
    queue_create_infos[cur].flags = 0;
    queue_create_infos[cur].queueFamilyIndex
      = phys_dev_info->queue_families.compute_index;
    queue_create_infos[cur].queueCount = builder->compute_queues;
    queue_create_infos[cur].pQueuePriorities =
      builder->compute_queue_priorities;
    cur++;
  }
  if (builder->transfer_queues > 0) {
    queue_create_infos[cur].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[cur].pNext = NULL;
    queue_create_infos[cur].flags = 0;
    queue_create_infos[cur].queueFamilyIndex
      = phys_dev_info->queue_families.transfer_index;
    queue_create_infos[cur].queueCount = builder->transfer_queues;
    queue_create_infos[cur].pQueuePriorities =
      builder->transfer_queue_priorities;
    cur++;
  }

  /* Populate device create info */
  dev_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  dev_create_info.pNext = NULL;
  dev_create_info.flags = 0;
  dev_create_info.queueCreateInfoCount = cur;
  dev_create_info.pQueueCreateInfos = queue_create_infos;
  dev_create_info.enabledLayerCount = builder->layer_count;
  dev_create_info.ppEnabledLayerNames = builder->layers;
  dev_create_info.enabledExtensionCount = builder->extension_count;
  dev_create_info.ppEnabledExtensionNames = builder->extensions;
  dev_create_info.pEnabledFeatures = &builder->features;

  /* Create device */
  VK_CHECK(vkCreateDevice(
      *phys_dev,
      &dev_create_info,
      NULL,
      &dev.device
  ));

  /* Allocate space for queues */
  if (builder->graphics_queues > 0) {
    dev.graphics_queues =
      (VkQueue *)malloc(sizeof(VkQueue) * builder->graphics_queues);
    ASSERT(dev.graphics_queues);
  }
  if (builder->present_queues > 0) {
    dev.present_queues =
      (VkQueue *)malloc(sizeof(VkQueue) * builder->present_queues);
    ASSERT(dev.present_queues);
  }
  if (builder->compute_queues > 0) {
    dev.compute_queues =
      (VkQueue *)malloc(sizeof(VkQueue) * builder->compute_queues);
    ASSERT(dev.compute_queues);
  }
  if (builder->transfer_queues > 0) {
    dev.transfer_queues =
      (VkQueue *)malloc(sizeof(VkQueue) * builder->transfer_queues);
    ASSERT(dev.transfer_queues);
  }

  /* Get queues */
  for (uint32_t i = 0; i < builder->graphics_queues; i++) {
    vkGetDeviceQueue(
        dev.device,
        phys_dev_info->queue_families.graphics_index,
        i,
        &dev.graphics_queues[i]
    );
  }
  for (uint32_t i = 0; i < builder->present_queues; i++) {
    vkGetDeviceQueue(
        dev.device,
        phys_dev_info->queue_families.present_index,
        i,
        &dev.present_queues[i]
    );
  }
  for (uint32_t i = 0; i < builder->compute_queues; i++) {
    vkGetDeviceQueue(
        dev.device,
        phys_dev_info->queue_families.compute_index,
        i,
        &dev.compute_queues[i]
    );
  }
  for (uint32_t i = 0; i < builder->transfer_queues; i++) {
    vkGetDeviceQueue(
        dev.device,
        phys_dev_info->queue_families.transfer_index,
        i,
        &dev.transfer_queues[i]
    );
  }

  /* Free builder */
  if (builder->extensions) free(builder->extensions);
  if (builder->layers) free(builder->layers);
  if (builder->graphics_queue_priorities)
    free(builder->graphics_queue_priorities);
  if (builder->present_queue_priorities)
    free(builder->present_queue_priorities);
  if (builder->compute_queue_priorities)
    free(builder->compute_queue_priorities);
  if (builder->transfer_queue_priorities)
    free(builder->transfer_queue_priorities);
  memset(builder, 0, sizeof(vk_dev_builder_t));

  return dev;
}
/* Destroy a Vulkan device */
void vk_dev_destroy(vk_dev_t *dev) {
  vkDestroyDevice(dev->device, NULL);
  if (dev->graphics_queues) free(dev->graphics_queues);
  if (dev->present_queues) free(dev->present_queues);
  if (dev->compute_queues) free(dev->compute_queues);
  if (dev->transfer_queues) free(dev->transfer_queues);
  memset(dev, 0, sizeof(vk_dev_t));
}
