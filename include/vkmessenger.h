/* Include guard */
#if !defined(VKMESSENGER_H)
#define VKMESSENGER_H

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

/* Vulkan debug messenger struct */
typedef struct {
  VkDebugUtilsMessengerEXT messenger;
} vkmessenger_t;

/* Create a debug messenger */
extern void vkmessenger_create(
    vkmessenger_t *messenger,
    VkInstance instance
);
/* Destroy a debug messenger */
extern void vkmessenger_destroy(
    vkmessenger_t *messenger,
    VkInstance instance
);

#endif /* VKMESSENGER_H */
