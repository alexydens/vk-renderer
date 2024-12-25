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
#include <vk_swapchain.h>
#include <vulkan/vulkan_core.h>

/* App state */
static struct {
  SDL_Window *window;
  bool running;
  bool same_queue_families;
  uint32_t width, height;
  vk_inst_t instance;
  vk_surf_t surface;
  vk_phys_dev_t physical_device;
  vk_phys_dev_info_t physical_device_info;
  vk_dev_t device;
  vk_swapchain_t swapchain;
} app_state;

/* Get required instance extensions */
static void get_required_exts(const char **exts, uint32_t *count) {
  SDL_Vulkan_GetInstanceExtensions(app_state.window, count, exts);
}
/* Create surface */
static VkSurfaceKHR create_surface(const VkInstance instance) {
  VkSurfaceKHR surface;
  SDL_Vulkan_CreateSurface(app_state.window, instance, &surface);
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
static void app_create_instance(void) {
  vk_inst_builder_t builder = vk_inst_builder();
  vk_inst_builder_use_messenger(&builder);
  vk_inst_builder_add_required_exts(&builder, get_required_exts);
  vk_inst_builder_set_app_name(&builder, "vk-renderer test");
  vk_inst_builder_set_app_version(&builder, 0, 0, 1);
  vk_inst_builder_add_layer(&builder, "VK_LAYER_KHRONOS_validation");
  vk_inst_builder_add_ext(&builder, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  app_state.instance = vk_inst_create(&builder);
  log_msg(LOG_LEVEL_SUCCESS, "Created Vulkan instance");
}
static void app_create_surface(void) {
  app_state.surface = create_surface(app_state.instance.instance); 
  log_msg(LOG_LEVEL_SUCCESS, "Created Vulkan surface");
}
static void app_create_device(void) {
  app_state.physical_device = vk_phys_dev_choose(
      score_physical_device,
      &app_state.instance,
      &app_state.surface
  );
  vk_phys_dev_get_info(
      app_state.physical_device,
      &app_state.physical_device_info,
      &app_state.surface
  );
  log_msg(
      LOG_LEVEL_INFO,
      "Chose physical device: %s",
      app_state.physical_device_info.properties.deviceName
  );
  app_state.same_queue_families =
    app_state.physical_device_info.queue_families.graphics_index
    == app_state.physical_device_info.queue_families.present_index;
  if (app_state.same_queue_families)
    log_msg(
        LOG_LEVEL_INFO,
        "Same queue family used for graphics and presentation"
    );
  vk_dev_builder_t builder = vk_dev_builder();
  vk_dev_builder_add_ext(&builder, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  vk_dev_builder_add_layer(&builder, "VK_LAYER_KHRONOS_validation");
  if (app_state.same_queue_families)
    vk_dev_builder_add_present_queue(&builder, 1.0f);
  else {
    vk_dev_builder_add_present_queue(&builder, 1.0f);
    vk_dev_builder_add_graphics_queue(&builder, 1.0f);
  }
  app_state.device = vk_dev_create(
      &app_state.physical_device,
      &app_state.physical_device_info,
      &builder
  );
  log_msg(LOG_LEVEL_SUCCESS, "Created Vulkan device");
}
static void app_create_swapchain(void) {
  vk_swapchain_builder_t builder = vk_swapchain_builder();
  vk_swapchain_builder_set_extent(&builder, app_state.width, app_state.height);
  vk_swapchain_builder_set_present_mode(&builder, VK_PRESENT_MODE_FIFO_KHR);
  uint32_t image_count =
    app_state.physical_device_info.surface_capabilities.minImageCount + 1;
  if (
    app_state.physical_device_info.surface_capabilities.maxImageCount > 0 &&
    image_count
    > app_state.physical_device_info.surface_capabilities.maxImageCount
  ) image_count =
      app_state.physical_device_info.surface_capabilities.maxImageCount;
  for (
      uint32_t i = 0;
      i < app_state.physical_device_info.surface_formats_count;
      i++
  ) {
    if (
      app_state.physical_device_info.surface_formats[i].format
      == VK_FORMAT_B8G8R8A8_SRGB
    ) {
      vk_swapchain_builder_set_format(
          &builder, 
          app_state.physical_device_info.surface_formats[i]
      );
      break;
    }
  }
  vk_swapchain_builder_set_image_count(&builder, image_count);
  app_state.swapchain = vk_swapchain_create(
      &app_state.device,
      &app_state.surface,
      &builder
  );
  log_msg(LOG_LEVEL_INFO, "Swapchain image count: %d", app_state.swapchain.image_count);
  log_msg(LOG_LEVEL_SUCCESS, "Created Vulkan swapchain");
}
static void app_cleanup_vulkan(void) {
  vk_swapchain_destroy(&app_state.swapchain, &app_state.device);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed Vulkan swapchain");
  vk_dev_destroy(&app_state.device);
  vk_phys_dev_info_free(&app_state.physical_device_info);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed Vulkan device");
  vk_surf_destroy(&app_state.surface, &app_state.instance);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed Vulkan surface");
  vk_inst_destroy(&app_state.instance);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed Vulkan instance");
}

/* Entry point */
int main(void) {
  /* Choose backend */
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11");
  /* Initialize SDL2 */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    log_msg(LOG_LEVEL_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
    return 1;
  }
  log_msg(LOG_LEVEL_SUCCESS, "Intialized SDL");
  /* Create window */
  app_state.width = 800;
  app_state.height = 600;
  app_state.window = SDL_CreateWindow(
      "VK Renderer",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      app_state.width,
      app_state.height,
      SDL_WINDOW_VULKAN
      | SDL_WINDOW_SHOWN
      | SDL_WINDOW_ALLOW_HIGHDPI
      | SDL_WINDOW_RESIZABLE
  );
  if (app_state.window == NULL) {
    log_msg(LOG_LEVEL_ERROR, "Failed to create window: %s", SDL_GetError());
    return 1;
  }
  log_msg(LOG_LEVEL_SUCCESS, "Created window");

  /* Initialize vulkan */
  app_create_instance();
  app_create_surface();
  app_create_device();
  app_create_swapchain();
  
  /* Main loop */
  app_state.running = true;
  while (app_state.running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
          app_state.running = false;
          break;
        case SDL_WINDOWEVENT:
          switch (event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
              app_state.running = false;
              break;
            case SDL_WINDOWEVENT_RESIZED:
              {
                app_state.width = event.window.data1;
                app_state.height = event.window.data2;
                log_msg(
                    LOG_LEVEL_INFO,
                    "Window resized to %dx%d",
                    app_state.width,
                    app_state.height
                );
                vk_swapchain_destroy(&app_state.swapchain, &app_state.device);
                app_create_swapchain();
              } break;
          } break;
        default: break;
      }
    }
  }

  /* Cleanup */
  app_cleanup_vulkan();
  /* Destroy window */
  SDL_DestroyWindow(app_state.window);
  log_msg(LOG_LEVEL_SUCCESS, "Destroyed window");
  /* Quit SDL2 */
  SDL_Quit();
  log_msg(LOG_LEVEL_SUCCESS, "Quit SDL");
  return 0;
}
