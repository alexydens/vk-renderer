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
#include <vk_inst.h>

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
    log_msg(LOG_LEVEL_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
    return 1;
  }
  log_msg(LOG_LEVEL_SUCCESS, "Intialized SDL");
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
    log_msg(LOG_LEVEL_ERROR, "Failed to create window: %s", SDL_GetError());
    return 1;
  }
  log_msg(LOG_LEVEL_SUCCESS, "Created window");

  /* Get required extensions */
  const char **extensions_required = NULL;
  uint32_t num_extensions_required = 0;
  SDL_Vulkan_GetInstanceExtensions(
      window_state.window,
      &num_extensions_required,
      NULL
  );
  if (num_extensions_required > 0) {
    extensions_required = malloc(sizeof(char *) * num_extensions_required);
    SDL_Vulkan_GetInstanceExtensions(
        window_state.window,
        &num_extensions_required,
        extensions_required
    );
  }

  /* Create vulkan objects */
  vk_inst_create_info_t vk_inst_create_info;
  vk_inst_t vk_inst;
  vk_inst_create_info_init(&vk_inst_create_info);
  for (uint32_t i = 0; i < num_extensions_required; i++) {
    vk_inst_create_info_add_extension(&vk_inst_create_info, extensions_required[i]);
  }
  vk_inst_create_info.app_name = "Test Application";
  vk_inst_create_info.use_messenger = true;
  vk_inst_create(&vk_inst, &vk_inst_create_info);
  vk_inst_create_info_free(&vk_inst_create_info);

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
          log_msg(LOG_LEVEL_INFO, "Window resized to %dx%d", width, height);
        }
      }
    }
  }

  /* Destroy vulkan objects */
  vk_inst_destroy(&vk_inst);

  /* Other cleanup */
  if (extensions_required != NULL) free(extensions_required);
  /* Destroy window */
  SDL_DestroyWindow(window_state.window);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed window");
  /* Quit SDL2 */
  SDL_Quit();
  log_msg(LOG_LEVEL_SUCCESS, "Quit SDL");
  return 0;
}
