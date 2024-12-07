/* Implements vksurface.h */
#include <vksurface.h>

/* Create surface */
void vksurface_create(
    vksurface_t *surface,
    vkinst_t *instance,
    SDL_Window *window
) {
  /* Create surface */
  SDL_Vulkan_CreateSurface(window, instance->instance, &surface->surface);

  /* Print message */
  printf("INFO: Successfully created vulkan surface\n");
}
/* Destroy surface */
void vksurface_destroy(vksurface_t *surface, vkinst_t *instance) {
  /* Destroy surface */
  vkDestroySurfaceKHR(instance->instance, surface->surface, NULL);

  /* Print message */
  printf("INFO: Successfully destroyed vulkan surface\n");
}
