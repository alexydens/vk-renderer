/* Implements vk_inst.h */
#include <vk_inst.h>

/* Debug messenger callback */
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
  const char *severity;
  const char *type;
  switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      severity = "\033[1;37mVERBOSE\033[0m";
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      severity = "\033[1;37mINFO\033[0m";
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      severity = "\033[1;33mWARNING\033[0m";
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      severity = "\033[1;31mERROR\033[0m";
      break;
    default:
      severity = "UNKNOWN";
  }
  switch (messageType) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
      type = "VALIDATION";
      break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
      type = "PERFORMANCE";
      break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
      type = "DEVICE_ADDRESS_BINDING";
      break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
      type = "GENERAL";
      break;
  }
  fprintf(
      stderr,
      "[VALIDATION] (%s,%s): %s\n",
      severity,
      type,
      pCallbackData->pMessage
  );
  (void)pUserData;
  (void)pCallbackData;
  return VK_FALSE;
}

/* Create a Vulkan instance builder */
vk_inst_builder_t vk_inst_builder(void) {
  vk_inst_builder_t builder;
  builder.use_messenger = false;
  builder.extensions = NULL;
  builder.extension_count = 0;
  builder.layers = NULL;
  builder.layer_count = 0;
  builder.app_name = "vk-renderer application";
  builder.app_version = VK_MAKE_VERSION(0, 0, 0);
  return builder;
}
/* Add a Vulkan instance extension */
void vk_inst_builder_add_ext(
    vk_inst_builder_t *builder, 
    const char *extension
) {
  if (!builder->extensions) {
    builder->extensions = (const char **)malloc(sizeof(const char *));
    ASSERT(builder->extensions);
    builder->extension_count = 1;
    builder->extensions[0] = extension;
    return;
  }
  builder->extension_count++;
  builder->extensions = (const char **)realloc(
    builder->extensions, sizeof(const char *) * builder->extension_count
  );
  ASSERT(builder->extensions);
  builder->extensions[builder->extension_count - 1] = extension;
}
/* Add required instance extensions */
void vk_inst_builder_add_required_exts(
    vk_inst_builder_t *builder,
    void (*get_required_exts)(const char **exts, uint32_t *count)
) {
  uint32_t count;
  const char **exts;
  get_required_exts(NULL, &count);
  exts = (const char **)malloc(sizeof(const char *) * count);
  ASSERT(exts);
  get_required_exts(exts, &count);
  for (uint32_t i = 0; i < count; i++) {
    vk_inst_builder_add_ext(builder, exts[i]);
  }
  free(exts);
}
/* Add a Vulkan instance layer */
void vk_inst_builder_add_layer(
    vk_inst_builder_t *builder, 
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
/* Use validation layers */
void vk_inst_builder_use_messenger(
    vk_inst_builder_t *builder
) {
  builder->use_messenger = true;
}
/* Set application name */
void vk_inst_builder_set_app_name(
    vk_inst_builder_t *builder,
    const char *name
) {
  builder->app_name = name;
}
/* Set application version */
void vk_inst_builder_set_app_version(
    vk_inst_builder_t *builder,
    uint8_t major, uint8_t minor, uint8_t patch
) {
  builder->app_version = VK_MAKE_VERSION(major, minor, patch);
}

/* Create a Vulkan instance (and free builder) */
vk_inst_t vk_inst_create(vk_inst_builder_t *builder) {
  vk_inst_t inst;
  VkApplicationInfo app_info;
  VkInstanceCreateInfo create_info;
  VkDebugUtilsMessengerCreateInfoEXT messenger_info;
  VkExtensionProperties *supported_exts = NULL;
  uint32_t supported_ext_count = 0;
  VkLayerProperties *supported_layers = NULL;
  uint32_t supported_layer_count = 0;

  /* Get supported extensions */
  vkEnumerateInstanceExtensionProperties(NULL, &supported_ext_count, NULL);
  if (supported_ext_count > 0) {
    supported_exts = (VkExtensionProperties *)malloc(
      sizeof(VkExtensionProperties) * supported_ext_count
    );
    ASSERT(supported_exts);
    vkEnumerateInstanceExtensionProperties(
      NULL, &supported_ext_count, supported_exts
    );
  }
  /* Get supported layers */
  vkEnumerateInstanceLayerProperties(&supported_layer_count, NULL);
  if (supported_layer_count > 0) {
    supported_layers = (VkLayerProperties *)malloc(
      sizeof(VkLayerProperties) * supported_layer_count
    );
    ASSERT(supported_layers);
    vkEnumerateInstanceLayerProperties(&supported_layer_count, supported_layers);
  }

  /* Check every extension is supported */
  for (uint32_t i = 0; i < builder->extension_count; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < supported_ext_count; j++) {
      if (strcmp(builder->extensions[i], supported_exts[j].extensionName) == 0) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      log_msg(
          LOG_LEVEL_ERROR,
          "Instance extension %s is not supported",
          builder->extensions[i]
      );
      abort();
    }
  }
  /* Check every layer is supported */
  for (uint32_t i = 0; i < builder->layer_count; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < supported_layer_count; j++) {
      if (strcmp(builder->layers[i], supported_layers[j].layerName) == 0) {
        supported = true;
        break;
      }
    }
    if (!supported) {
      log_msg(
          LOG_LEVEL_ERROR,
          "Instance layer %s is not supported",
          builder->layers[i]
      );
      abort();
    }
  }

  /* Populate instance */
  inst.use_messenger = builder->use_messenger;
  inst.instance = VK_NULL_HANDLE;
  inst.debug_messenger = VK_NULL_HANDLE;

  /* Populate debug messenger create info */
  if (builder->use_messenger) {
    messenger_info.sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messenger_info.pNext = NULL;
    messenger_info.flags = 0;
    messenger_info.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messenger_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    messenger_info.pfnUserCallback = debug_callback;
    messenger_info.pUserData = NULL;
  }
  
  /* Populate application info */
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pNext = NULL;
  app_info.pApplicationName = builder->app_name;
  app_info.applicationVersion = builder->app_version;
  app_info.pEngineName = "vk-renderer";
  app_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_2;

  /* Populate instance create info */
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pNext = builder->use_messenger ? &messenger_info : NULL;
  create_info.flags = 0;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledExtensionCount = builder->extension_count;
  create_info.ppEnabledExtensionNames = builder->extensions;
  create_info.enabledLayerCount = builder->layer_count;
  create_info.ppEnabledLayerNames = builder->layers;

  /* Create instance */
  VK_CHECK(vkCreateInstance(&create_info, NULL, &inst.instance));

  /* Create messenger */
  if (builder->use_messenger) {
    /* Get vkCreateDebugUtilsMessengerEXT */
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT =
    (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        inst.instance,
        "vkCreateDebugUtilsMessengerEXT"
    );
    /* Create messenger */
    VK_CHECK(
      vkCreateDebugUtilsMessengerEXT(
        inst.instance,
        &messenger_info,
        NULL,
        &inst.debug_messenger
      )
    );
  }

  /* Free builder */
  if (builder->extensions) free(builder->extensions);
  if (builder->layers) free(builder->layers);
  builder->use_messenger = false;
  builder->extensions = NULL;
  builder->extension_count = 0;
  builder->layers = NULL;
  builder->layer_count = 0;
  builder->app_name = NULL;
  builder->app_version = 0;

  return inst;
}
/* Destroy a Vulkan instance */
void vk_inst_destroy(vk_inst_t *inst) {
  if (inst->use_messenger) {
    /* Get vkDestroyDebugUtilsMessengerEXT */
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT =
    (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        inst->instance,
        "vkDestroyDebugUtilsMessengerEXT"
    );
    /* Destroy messenger */
    vkDestroyDebugUtilsMessengerEXT(
      inst->instance,
      inst->debug_messenger,
      NULL
    );
  }
  /* Destroy instance */
  vkDestroyInstance(inst->instance, NULL);
}
