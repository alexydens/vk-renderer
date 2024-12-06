/* Implements vkmessenger.h */
#include <vkmessenger.h>

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
      severity = "VERBOSE";
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      severity = "INFO";
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      severity = "WARNING";
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      severity = "ERROR";
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
      "VALIDATION: %s: %s: %s\n",
      severity,
      type,
      pCallbackData->pMessage
  );
  (void)pUserData;
  (void)pCallbackData;
  return VK_FALSE;
}

/* Create a debug messenger */
void vkmessenger_create(
    vkmessenger_t *messenger,
    VkInstance instance
) {
  VkDebugUtilsMessengerCreateInfoEXT create_info;
  PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;

  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.pNext = NULL;
  create_info.flags = 0;
  create_info.messageSeverity = 
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = 
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
      | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
  create_info.pfnUserCallback = debug_callback;
  create_info.pUserData = NULL;

  vkCreateDebugUtilsMessengerEXT =
    (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance,
      "vkCreateDebugUtilsMessengerEXT"
  );
  if (!vkCreateDebugUtilsMessengerEXT) {
    fprintf(
        stderr,
        "ERROR: Failed to load vkCreateDebugUtilsMessengerEXT\n"
    );
    exit(1);
  }
  if (vkCreateDebugUtilsMessengerEXT(
        instance,
        &create_info,
        NULL,
        &messenger->messenger
  ) != VK_SUCCESS) {
    fprintf(
        stderr,
        "ERROR: Failed to create debug messenger\n"
    );
    exit(1);
  }

  /* Print message */
  printf("INFO: Successfully created debug messenger\n");
}
/* Destroy a debug messenger */
void vkmessenger_destroy(
    vkmessenger_t *messenger,
    VkInstance instance
) {
  PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;

  vkDestroyDebugUtilsMessengerEXT =
    (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance,
      "vkDestroyDebugUtilsMessengerEXT"
  );
  if (!vkDestroyDebugUtilsMessengerEXT) {
    fprintf(
        stderr,
        "ERROR: Failed to load vkDestroyDebugUtilsMessengerEXT\n"
    );
    exit(1);
  }
  vkDestroyDebugUtilsMessengerEXT(instance, messenger->messenger, NULL);
  messenger->messenger = VK_NULL_HANDLE;

  /* Print message */
  printf("INFO: Successfully destroyed debug messenger\n");
}
