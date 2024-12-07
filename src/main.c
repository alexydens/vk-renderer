/* Includes */
#include <SDL2/SDL.h>         /* Windowing */
#include <SDL2/SDL_vulkan.h>  /* Sdl2 vulkan */
#include <stdint.h>           /* Integer types */
#include <stddef.h>           /* Definitions */
#include <stdbool.h>          /* Booleans */
#include <stdlib.h>           /* Memory */
#include <string.h>           /* Strings */
#include <stdio.h>            /* Terminal I/O */
/* Project includes */
#include <vkinst.h>
#include <vkmessenger.h>
#include <vksurface.h>
#include <vkphysdev.h>
#include <vkdev.h>

/* Window state */
struct {
  SDL_Window *window;
  bool running;
} window_state;

/* Entry point */
int main(void) {
  /* Make sure to use x11 */
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11");
  /* Initialize SDL2 */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }
  /* Create window */
  window_state.window = SDL_CreateWindow(
      "VK Renderer",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      800,
      600,
      SDL_WINDOW_VULKAN
      | SDL_WINDOW_SHOWN
      | SDL_WINDOW_ALLOW_HIGHDPI
      | SDL_WINDOW_RESIZABLE
  );
  if (window_state.window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return 1;
  }

  /* Create vulkan objects */
  vkinst_t vkinst;
  vkmessenger_t vkmessenger;
  vksurface_t vksurface;
  vkphysdev_t vkphysdev;
  vkdev_t vkdev;
  vkinst_init(&vkinst);
  vkinst_add_extension(&vkinst, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  vkinst_add_layer(&vkinst, "VK_LAYER_KHRONOS_validation");
  vkinst_create(&vkinst, window_state.window);
  vkmessenger_create(&vkmessenger, vkinst.instance);
  vksurface_create(&vksurface, &vkinst, window_state.window);
  vkphysdev_pick(&vkphysdev, &vkinst, &vksurface);
  vkdev_init(&vkdev);
  vkdev_add_extension(&vkdev, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  vkdev_add_layer(&vkdev, "VK_LAYER_KHRONOS_validation");
  vkdev_create(&vkdev, &vkphysdev);

  /* Main loop */
  window_state.running = true;
  while (window_state.running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        window_state.running = false;
      }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
          window_state.running = false;
        }
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          uint32_t width = event.window.data1;
          uint32_t height = event.window.data2;
          printf("INFO: Window resized to %dx%d\n", width, height);
        }
      }
    }
  }

  /* Destroy vulkan objects */
  vkdev_destroy(&vkdev);
  vksurface_destroy(&vksurface, &vkinst);
  vkmessenger_destroy(&vkmessenger, vkinst.instance);
  vkinst_destroy(&vkinst);

  /* Destroy window */
  SDL_DestroyWindow(window_state.window);
  /* Quit SDL2 */
  SDL_Quit();
  return 0;
}
