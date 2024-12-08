/* Implements vk_inst.h */
#include <vk_inst.h>
#include <vulkan/vulkan_core.h>

/* Initialize vulkan instance struct */
void vk_inst_init(vk_inst_t *vk_inst) {
  vk_inst->instance = VK_NULL_HANDLE;
  vk_inst->extensions = NULL;
  vk_inst->num_extensions = 0;
  vk_inst->layers = NULL;
  vk_inst->num_layers = 0;
}
/* Add requested extension */
void vk_inst_add_extension(vk_inst_t *vk_inst, const char *extension) {
  if (vk_inst->extensions == NULL) {
    vk_inst->extensions = malloc(sizeof(char *) * 1);
    ASSERT(vk_inst->extensions != NULL);
    vk_inst->extensions[0] = extension;
    vk_inst->num_extensions = 1;
    return;
  }
  vk_inst->extensions = realloc(
      vk_inst->extensions,
      sizeof(char *) * (vk_inst->num_extensions + 1)
  );
  ASSERT(vk_inst->extensions != NULL);
  vk_inst->extensions[vk_inst->num_extensions] = extension;
  vk_inst->num_extensions++;
}
/* Add requested layer */
void vk_inst_add_layer(vk_inst_t *vk_inst, const char *layer) {
  if (vk_inst->layers == NULL) {
    vk_inst->layers = malloc(sizeof(char *) * 1);
    ASSERT(vk_inst->layers != NULL);
    vk_inst->layers[0] = layer;
    vk_inst->num_layers = 1;
    return;
  }
  vk_inst->layers = realloc(
      vk_inst->layers,
      sizeof(char *) * (vk_inst->num_layers + 1)
  );
  ASSERT(vk_inst->layers != NULL);
  vk_inst->layers[vk_inst->num_layers] = layer;
  vk_inst->num_layers++;
}

/* Create vulkan instance (if app_name is NULL, a default will be used) */
void vk_inst_create(vk_inst_t *vk_inst, const char *app_name) {
  VkExtensionProperties *supported_extensions = NULL;
  uint32_t num_supported_extensions = 0;
  VkLayerProperties *supported_layers = NULL;
  uint32_t num_supported_layers = 0;
  VkApplicationInfo app_info;
  VkInstanceCreateInfo inst_create_info;

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
  for (uint32_t i = 0; i < vk_inst->num_extensions; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < num_supported_extensions; j++) {
      if (strcmp(
            vk_inst->extensions[i],
            supported_extensions[j].extensionName) == 0
      ) {
        supported = true;
        break;
      }
    }
    ASSERT(supported);
    log_msg(LOG_LEVEL_INFO, "Using extension: %s", vk_inst->extensions[i]);
  }

  /* Check requested layers are supported */
  for (uint32_t i = 0; i < vk_inst->num_layers; i++) {
    bool supported = false;
    for (uint32_t j = 0; j < num_supported_layers; j++) {
      if (strcmp(
            vk_inst->layers[i],
            supported_layers[j].layerName) == 0
      ) {
        supported = true;
        break;
      }
    }
    ASSERT(supported);
    log_msg(LOG_LEVEL_INFO, "Using layer: %s", vk_inst->layers[i]);
  }

  /* Populate application info */
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pNext = NULL;
  if (app_name) app_info.pApplicationName = app_name;
  else app_info.pApplicationName = "vk-renderer application";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "vk-renderer";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_2;

  /* Populate instance create info */
  inst_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  inst_create_info.pNext = NULL;
  inst_create_info.flags = 0;
  inst_create_info.pApplicationInfo = &app_info;
  inst_create_info.enabledExtensionCount = vk_inst->num_extensions;
  inst_create_info.ppEnabledExtensionNames = vk_inst->extensions;
  inst_create_info.enabledLayerCount = vk_inst->num_layers;
  inst_create_info.ppEnabledLayerNames = vk_inst->layers;

  /* Create instance */
  VK_CHECK(vkCreateInstance(&inst_create_info, NULL, &vk_inst->instance));

  /* Print message */
  log_msg(LOG_LEVEL_SUCCESS, "Vulkan instance created");
}
/* Destroy vulkan instance */
void vk_inst_destroy(vk_inst_t *vk_inst) {
  if (vk_inst->num_extensions > 0) free(vk_inst->extensions);
  if (vk_inst->num_layers > 0) free(vk_inst->layers);
  vkDestroyInstance(vk_inst->instance, NULL);
  vk_inst->extensions = NULL;
  vk_inst->layers = NULL;
  vk_inst->num_extensions = 0;
  vk_inst->num_layers = 0;
  vk_inst->instance = VK_NULL_HANDLE;

  /* Print message */
  log_msg(LOG_LEVEL_SUCCESS, "Vulkan instance destroyed");
}
