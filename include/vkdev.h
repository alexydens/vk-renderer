/* Include guard */
#if !defined(VKDEV_H)
#define VKDEV_H

/* Includes */
#include <vulkan/vulkan.h>    /* Vulkan */
#include <SDL2/SDL_vulkan.h>  /* Sdl2 vulkan */
#include <stdint.h>           /* Integer types */
#include <stddef.h>           /* Definitions */
#include <stdbool.h>          /* Booleans */
#include <stdlib.h>           /* Memory */
#include <string.h>           /* Strings */
#include <stdio.h>            /* Terminal I/O */
#include <errno.h>            /* Error handling */
/* Project includes */
#include <vkinst.h>
#include <vkphysdev.h>

/* Vulkan device struct */
typedef struct {
  VkDevice device;
  VkPhysicalDevice physical_device;
  const char **extensions;
  uint32_t num_extensions;
  const char **layers; 
  uint32_t num_layers;
  VkQueue graphics_queue;
  VkQueue present_queue;
  VkQueue compute_queue;
} vkdev_t;

/* Add another requested extension */
extern void vkdev_add_extension(
    vkdev_t *device,
    const char *extension
);
/* Add another requested layer */
extern void vkdev_add_layer(
    vkdev_t *device,
    const char *layer
);

/* Initialize a vulkan device */
extern void vkdev_init(vkdev_t *device);
/* Create a vulkan device */
extern void vkdev_create(
    vkdev_t *device,
    vkinst_t *instance,
    vkphysdev_t *physical_device
);
/* Destroy a vulkan device */
extern void vkdev_destroy(vkdev_t *device, vkinst_t *instance);

#endif /* VKDEV_H */
