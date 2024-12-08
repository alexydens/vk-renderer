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
#include <base.h>

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

  /* Destroy window */
  SDL_DestroyWindow(window_state.window);
  /* Quit SDL2 */
  SDL_Quit();
  return 0;
}
