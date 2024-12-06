/* Implements vkinst.h */
#include <vkinst.h>

/* Add another requested extension */
void vkinst_add_extension(
    vkinst_t *instance,
    const char *extension
) {
  if (instance->extensions == NULL) {
    instance->extensions = (const char **)malloc(sizeof(char *));
    if (!instance->extensions) {
      fprintf(
          stderr,
          "ERROR: malloc() failed: %s\n",
          strerror(errno)
      );
      exit(1);
    }
    instance->extensions[0] = extension;
    instance->num_extensions = 1;
    return;
  }
  instance->extensions = (const char **)realloc(
      instance->extensions,
      (instance->num_extensions + 1) * sizeof(char *)
  );
  if (!instance->extensions) {
    fprintf(
        stderr,
        "ERROR: realloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  instance->extensions[instance->num_extensions] = extension;
  instance->num_extensions++;
}
/* Add another requested layer */
void vkinst_add_layer(
    vkinst_t *instance,
    const char *layer
) {
  if (instance->layers == NULL) {
    instance->layers = (const char **)malloc(sizeof(char *));
    if (!instance->layers) {
      fprintf(
          stderr,
          "ERROR: malloc() failed: %s\n",
          strerror(errno)
      );
      exit(1);
    }
    instance->layers[0] = layer;
    instance->num_layers = 1;
    return;
  }
  instance->layers = (const char **)realloc(
      instance->layers,
      (instance->num_layers + 1) * sizeof(char *)
  );
  if (!instance->layers) {
    fprintf(
        stderr,
        "ERROR: realloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  instance->layers[instance->num_layers] = layer;
  instance->num_layers++;
}

/* Initialize a vulkan instance */
void vkinst_init(vkinst_t *instance) {
  instance->instance = NULL;
  instance->extensions = NULL;
  instance->num_extensions = 0;
  instance->layers = NULL;
  instance->num_layers = 0;
}
/* Create a vulkan instance */
void vkinst_create(vkinst_t *instance, SDL_Window *window) {
  /* The extensions vulkan supports */
  VkExtensionProperties *vk_supported_extensions;
  uint32_t num_vk_supported_extensions;
  /* The extensions SDL requires */
  const char **sdl_required_extensions;
  uint32_t num_sdl_required_extensions;
  /* The layers vulkan supports */
  VkLayerProperties *vk_supported_layers;
  uint32_t num_vk_supported_layers;
  /* The information for the creation of the instance */
  VkInstanceCreateInfo create_info;
  /* The application information */
  VkApplicationInfo app_info;

  /* Get the extensions SDL requires */
  SDL_Vulkan_GetInstanceExtensions(window, &num_sdl_required_extensions, NULL);
  sdl_required_extensions = (const char **)malloc(
      num_sdl_required_extensions * sizeof(char *)
  );
  if (!sdl_required_extensions) {
    fprintf(
        stderr,
        "ERROR: malloc() failed: %s\n",
        strerror(errno)
    );
    exit(1);
  }
  SDL_Vulkan_GetInstanceExtensions(
      window,
      &num_sdl_required_extensions,
      sdl_required_extensions
  );

  /* Add the required extensions */
  for (uint32_t i = 0; i < num_sdl_required_extensions; i++) {
    vkinst_add_extension(instance, sdl_required_extensions[i]);
  }

  /* Get the extensions vulkan supports */
  vkEnumerateInstanceExtensionProperties(
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
  vkEnumerateInstanceExtensionProperties(
      NULL,
      &num_vk_supported_extensions,
      vk_supported_extensions
  );

  /* Check for extension support */
  for (uint32_t i = 0; i < instance->num_extensions; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < num_vk_supported_extensions; j++) {
      if (strcmp(
            vk_supported_extensions[j].extensionName,
            instance->extensions[i]
      ) == 0) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      fprintf(
          stderr,
          "ERROR: Extension %s not supported\n",
          instance->extensions[i]
      );
      exit(1);
    }
    printf("INFO: Extension %s supported and used\n", instance->extensions[i]);
  }

  /* Get the layers vulkan supports */
  vkEnumerateInstanceLayerProperties(
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
  vkEnumerateInstanceLayerProperties(
      &num_vk_supported_layers,
      vk_supported_layers
  );

  /* Check for layer support */
  for (uint32_t i = 0; i < instance->num_layers; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < num_vk_supported_layers; j++) {
      if (strcmp(
            vk_supported_layers[j].layerName,
            instance->layers[i]
      ) == 0) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      fprintf(
          stderr,
          "ERROR: Layer %s not supported\n",
          instance->layers[i]
      );
      exit(1);
    }
    printf("INFO: Layer %s supported and used\n", instance->layers[i]);
  }

  /* Populate the application info */
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pNext = NULL;
  app_info.pApplicationName = "vk-renderer project";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "vk-renderer";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  /* Populate the instance create info */
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pNext = NULL;
  create_info.flags = 0;
  create_info.pApplicationInfo = &app_info;
  create_info.ppEnabledExtensionNames = instance->extensions;
  create_info.enabledExtensionCount = instance->num_extensions;
  create_info.ppEnabledLayerNames = instance->layers;
  create_info.enabledLayerCount = instance->num_layers;

  /* Attempt to create the instance */
  if (vkCreateInstance(
        &create_info,
        NULL,
        &instance->instance
  ) != VK_SUCCESS) {
    fprintf(stderr, "ERROR: Failed to create vulkan instance\n");
    exit(1);
  }

  /* Print message */
  printf("INFO: Successfully created vulkan instance\n");
}
/* Destroy a vulkan instance */
void vkinst_destroy(vkinst_t *instance) {
  vkDestroyInstance(instance->instance, NULL);
  if (instance->extensions) free(instance->extensions);
  instance->num_extensions = 0;
  if (instance->layers) free(instance->layers);
  instance->num_layers = 0;
  instance->instance = VK_NULL_HANDLE;

  /* Print message */
  printf("INFO: Successfully destroyed vulkan instance\n");
}
