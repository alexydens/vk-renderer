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
#include <vk_surf.h>
#include <vk_phys_dev.h>
#include <vk_dev.h>

/* Window state */
struct {
  SDL_Window *window;
  bool running;
} window_state;

/* Get required instance extensions */
static void get_required_exts(const char **exts, uint32_t *count) {
  SDL_Vulkan_GetInstanceExtensions(window_state.window, count, exts);
}
/* Create surface */
static VkSurfaceKHR create_surface(const VkInstance instance) {
  VkSurfaceKHR surface;
  SDL_Vulkan_CreateSurface(window_state.window, instance, &surface);
  return surface;
}
/* Score physical device */
static uint32_t score_physical_device(const vk_phys_dev_info_t *info) {
  switch (info->properties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
      return 1000;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
      return 250;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
      return 125;
    default:
      return 0;
  }
}


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
  
  /* Vulkan initialization */
  vk_inst_t instance;
  vk_surf_t surface;
  vk_phys_dev_t physical_device;
  vk_phys_dev_info_t physical_device_info;
  vk_dev_t device;
  vk_inst_builder_t instance_builder = vk_inst_builder();
  vk_inst_builder_use_messenger(&instance_builder);
  vk_inst_builder_add_ext(&instance_builder, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  vk_inst_builder_add_layer(&instance_builder, "VK_LAYER_KHRONOS_validation");
  vk_inst_builder_add_required_exts(&instance_builder, get_required_exts);
  vk_inst_builder_set_app_name(&instance_builder, "Vulkan Renderer Test");
  vk_inst_builder_set_app_version(&instance_builder, 0, 0, 0);
  instance = vk_inst_create(&instance_builder);
  log_msg(LOG_LEVEL_SUCCESS, "Created Vulkan instance");
  surface = create_surface(instance.instance);
  log_msg(LOG_LEVEL_SUCCESS, "Created Vulkan surface");
  physical_device = vk_phys_dev_choose(
      score_physical_device,
      &instance,
      &surface
  );
  vk_phys_dev_get_info(
      physical_device,
      &physical_device_info,
      &surface
  );
  log_msg(
      LOG_LEVEL_SUCCESS,
      "Chose Vulkan physical device: %s",
      physical_device_info.properties.deviceName
  );
  vk_dev_builder_t device_builder = vk_dev_builder();
  vk_dev_builder_add_ext(&device_builder, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  vk_dev_builder_add_layer(&device_builder, "VK_LAYER_KHRONOS_validation");
  vk_dev_builder_add_graphics_queue(&device_builder, 1.0f);
  vk_dev_builder_add_present_queue(&device_builder, 1.0f);
  device = vk_dev_create(
      &physical_device,
      &physical_device_info,
      &device_builder
  );
  log_msg(LOG_LEVEL_SUCCESS, "Created Vulkan device");

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

  /* Vulkan cleanup */
  vk_dev_destroy(&device);
  vk_phys_dev_info_free(&physical_device_info);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed Vulkan device");
  vk_surf_destroy(&surface, &instance);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed Vulkan surface");
  vk_inst_destroy(&instance);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed Vulkan instance");

  /* Destroy window */
  SDL_DestroyWindow(window_state.window);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed window");
  /* Quit SDL2 */
  SDL_Quit();
  log_msg(LOG_LEVEL_SUCCESS, "Quit SDL");
  return 0;
}
