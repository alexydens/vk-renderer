/* Implements vk_inst.h */
#include <vk_inst.h>
#include <vulkan/vulkan_core.h>
#include <wchar.h>

/* Callback */
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
/* Initialize vulkan instance create info */
void vk_inst_create_info_init(vk_inst_create_info_t *vk_inst_create_info) {
  vk_inst_create_info->app_name = NULL;
  vk_inst_create_info->use_messenger = false;
  vk_inst_create_info->extensions = NULL;
  vk_inst_create_info->num_extensions = 0;
  vk_inst_create_info->layers = NULL;
  vk_inst_create_info->num_layers = 0;
}
/* Free vulkan instance create info */
void vk_inst_create_info_free(vk_inst_create_info_t *vk_inst_create_info) {
  if (vk_inst_create_info->extensions != NULL)
    free(vk_inst_create_info->extensions);
  if (vk_inst_create_info->layers != NULL)
    free(vk_inst_create_info->layers);
}

/* Add requested extension */
void vk_inst_create_info_add_extension(
    vk_inst_create_info_t *vk_inst_create_info,
    const char *extension
) {
  if (vk_inst_create_info->extensions == NULL) {
    vk_inst_create_info->extensions = malloc(sizeof(char *) * 1);
    ASSERT(vk_inst_create_info->extensions != NULL);
    vk_inst_create_info->extensions[0] = extension;
    vk_inst_create_info->num_extensions = 1;
    return;
  }
  vk_inst_create_info->extensions = realloc(
      vk_inst_create_info->extensions,
      sizeof(char *) * (vk_inst_create_info->num_extensions + 1)
  );
  ASSERT(vk_inst_create_info->extensions != NULL);
  vk_inst_create_info->extensions[vk_inst_create_info->num_extensions]
    = extension;
  vk_inst_create_info->num_extensions++;
}
/* Add requested layer */
void vk_inst_create_info_add_layer(
    vk_inst_create_info_t *vk_inst_create_info,
    const char *layer
) {
  if (vk_inst_create_info->layers == NULL) {
    vk_inst_create_info->layers = malloc(sizeof(char *) * 1);
    ASSERT(vk_inst_create_info->layers != NULL);
    vk_inst_create_info->layers[0] = layer;
    vk_inst_create_info->num_layers = 1;
    return;
  }
  vk_inst_create_info->layers = realloc(
      vk_inst_create_info->layers,
      sizeof(char *) * (vk_inst_create_info->num_layers + 1)
  );
  ASSERT(vk_inst_create_info->layers != NULL);
  vk_inst_create_info->layers[vk_inst_create_info->num_layers] = layer;
  vk_inst_create_info->num_layers++;
}

/* Create vulkan instance */
void vk_inst_create(
    vk_inst_t *vk_inst,
    vk_inst_create_info_t *vk_inst_create_info
) {
  VkExtensionProperties *supported_extensions = NULL;
  uint32_t num_supported_extensions = 0;
  VkLayerProperties *supported_layers = NULL;
  uint32_t num_supported_layers = 0;
  VkApplicationInfo app_info;
  VkInstanceCreateInfo inst_create_info;
  VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;

  if (vk_inst_create_info->use_messenger) {
    /* Populate debug messenger create info */
    debug_messenger_create_info.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_create_info.pNext = NULL;
    debug_messenger_create_info.flags = 0;
    debug_messenger_create_info.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_create_info.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    debug_messenger_create_info.pfnUserCallback = debug_callback;
    debug_messenger_create_info.pUserData = NULL;
    /* Set in struct */
    vk_inst->use_messenger = true;
    /* Add extensions */
    vk_inst_create_info_add_extension(
        vk_inst_create_info,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    );
    /* Add layers */
    vk_inst_create_info_add_layer(
        vk_inst_create_info,
        "VK_LAYER_KHRONOS_validation"
    );
  }


  /* Get supported extensions */
  VK_CHECK(vkEnumerateInstanceExtensionProperties(
      NULL,
      &num_supported_extensions,
      NULL
  ));
  if (num_supported_extensions > 0) {
    supported_extensions =
      malloc(sizeof(VkExtensionProperties) * num_supported_extensions);
    ASSERT(supported_extensions != NULL);
    VK_CHECK(vkEnumerateInstanceExtensionProperties(
        NULL,
        &num_supported_extensions,
        supported_extensions
    ));
  }

  /* Get supported layers */
  VK_CHECK(vkEnumerateInstanceLayerProperties(
      &num_supported_layers,
      NULL
  ));
  if (num_supported_layers > 0) {
    supported_layers =
      malloc(sizeof(VkLayerProperties) * num_supported_layers);
    ASSERT(supported_layers != NULL);
    VK_CHECK(vkEnumerateInstanceLayerProperties(
        &num_supported_layers,
        supported_layers
    ));
  }

  /* Check requested extensions are supported */
  for (uint32_t i = 0; i < vk_inst_create_info->num_extensions; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < num_supported_extensions; j++) {
      if (strcmp(
            vk_inst_create_info->extensions[i],
            supported_extensions[j].extensionName) == 0
      ) {
        supported = true;
        break;
      }
    }
    ASSERT(supported);
    log_msg(
        LOG_LEVEL_INFO,
        "Using extension: %s",
        vk_inst_create_info->extensions[i]
    );
  }

  /* Check requested layers are supported */
  for (uint32_t i = 0; i < vk_inst_create_info->num_layers; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < num_supported_layers; j++) {
      if (strcmp(
            vk_inst_create_info->layers[i],
            supported_layers[j].layerName) == 0
      ) {
        supported = true;
        break;
      }
    }
    ASSERT(supported);
    log_msg(LOG_LEVEL_INFO, "Using layer: %s", vk_inst_create_info->layers[i]);
  }

  /* Populate application info */
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pNext = NULL;
  if (vk_inst_create_info->app_name)
    app_info.pApplicationName = vk_inst_create_info->app_name;
  else app_info.pApplicationName = "vk-renderer application";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "vk-renderer";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_2;

  /* Populate instance create info */
  inst_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  if (vk_inst->use_messenger)
    inst_create_info.pNext = &debug_messenger_create_info;
  else
    inst_create_info.pNext = NULL;
  inst_create_info.flags = 0;
  inst_create_info.pApplicationInfo = &app_info;
  inst_create_info.enabledExtensionCount = vk_inst_create_info->num_extensions;
  inst_create_info.ppEnabledExtensionNames = vk_inst_create_info->extensions;
  inst_create_info.enabledLayerCount = vk_inst_create_info->num_layers;
  inst_create_info.ppEnabledLayerNames = vk_inst_create_info->layers;

  /* Create instance */
  VK_CHECK(vkCreateInstance(&inst_create_info, NULL, &vk_inst->instance));

  /* Print message */
  log_msg(LOG_LEVEL_SUCCESS, "Vulkan instance created");

  /* Create debug messenger */
  if (vk_inst->use_messenger) {
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          vk_inst->instance,
          "vkCreateDebugUtilsMessengerEXT"
      );
    if (vkCreateDebugUtilsMessengerEXT == NULL) {
      log_msg(LOG_LEVEL_ERROR, "Failed to get vkCreateDebugUtilsMessengerEXT");
      return;
    }
    VK_CHECK(vkCreateDebugUtilsMessengerEXT(
        vk_inst->instance,
        &debug_messenger_create_info,
        NULL,
        &vk_inst->debug_messenger
    ));
    /* Print message */
    log_msg(LOG_LEVEL_SUCCESS, "Vulkan debug messenger created");
  }
}
/* Destroy vulkan instance */
void vk_inst_destroy(vk_inst_t *vk_inst) {
  if (vk_inst->use_messenger) {
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          vk_inst->instance,
          "vkDestroyDebugUtilsMessengerEXT"
      );
    if (vkDestroyDebugUtilsMessengerEXT == NULL) {
      log_msg(LOG_LEVEL_ERROR, "Failed to get vkDestroyDebugUtilsMessengerEXT");
      return;
    }
    vkDestroyDebugUtilsMessengerEXT(
        vk_inst->instance,
        vk_inst->debug_messenger,
        NULL
    );

    /* Print message */
    log_msg(LOG_LEVEL_SUCCESS, "Vulkan debug messenger destroyed");
  }

  vkDestroyInstance(vk_inst->instance, NULL);
  vk_inst->instance = VK_NULL_HANDLE;
  vk_inst->debug_messenger = VK_NULL_HANDLE;
  vk_inst->use_messenger = false;

  /* Print message */
  log_msg(LOG_LEVEL_SUCCESS, "Vulkan instance destroyed");
}
