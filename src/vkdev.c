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
  device->compute_queue = VK_NULL_HANDLE;
}
/* Create a vulkan device */
void vkdev_create(
    vkdev_t *device,
    vkinst_t *instance,
    vkphysdev_t *physical_device
);
/* Destroy a vulkan device */
void vkdev_destroy(vkdev_t *device, vkinst_t *instance);
