/* Include guard */
#if !defined(VKSURFACE_H)
#define VKSURFACE_H

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

/* Surface struct */
typedef struct {
  VkSurfaceKHR surface;
} vksurface_t;

/* Create surface */
extern void vksurface_create(
    vksurface_t *surface,
    vkinst_t *instance,
    SDL_Window *window
);
/* Destroy surface */
extern void vksurface_destroy(vksurface_t *surface, vkinst_t *instance);

#endif /* VKSURFACE_H */
