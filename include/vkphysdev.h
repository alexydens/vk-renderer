/* Include guard */
#if !defined(VKPHYSDEV_H)
#define VKPHYSDEV_H

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
#include <vksurface.h>

/* Vulkan physical device struct */
typedef struct {
  VkPhysicalDevice physical_device;
  VkPhysicalDeviceProperties properties;
  VkPhysicalDeviceMemoryProperties memory_properties;
  VkPhysicalDeviceFeatures features;
  uint32_t graphics_queue_index;
  uint32_t present_queue_index;
} vkphysdev_t;

/* Pick a physical devices */
extern void vkphysdev_pick(
    vkphysdev_t *physical_device,
    vkinst_t *instance,
    vksurface_t *surface
);

#endif /* VKPHYSDEV_H */
