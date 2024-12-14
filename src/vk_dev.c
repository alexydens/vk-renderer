/* Implements vk_dev.h */
#include <vk_dev.h>

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
