/* Include guard */
#if !defined(VKINST_H)
#define VKINST_H

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

/* Vulkan instance struct */
typedef struct {
  VkInstance instance;
  const char **extensions;
  uint32_t num_extensions;
  const char **layers; 
  uint32_t num_layers;
} vkinst_t;

/* Add another requested extension */
extern void vkinst_add_extension(
    vkinst_t *instance,
    const char *extension
);
/* Add another requested layer */
extern void vkinst_add_layer(
    vkinst_t *instance,
    const char *layer
);

/* Initialize a vulkan instance */
extern void vkinst_init(vkinst_t *instance);
/* Create a vulkan instance */
extern void vkinst_create(vkinst_t *instance, SDL_Window *window);
/* Destroy a vulkan instance */
extern void vkinst_destroy(vkinst_t *instance);

#endif /* VKINST_H */
