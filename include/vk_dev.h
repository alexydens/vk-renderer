/* Include guard */
#if !defined(VK_DEV_H)
#define VK_DEV_H

/* Includes */
#include <base.h>
#include <vk_inst.h>
#include <vk_surf.h>

/* Creation information */
typedef struct {
  bool choose_dev;                    /* Ask user to choose device */
  bool create_graphics_queue;         /* Do you want a graphics queue? */
  bool create_present_queue;          /* Do you want a present queue? */
  bool create_compute_queue;          /* Do you want a compute queue? */
  bool create_transfer_queue;         /* Do you want a transfer queue? */
  VkPhysicalDeviceFeatures features;  /* Requested physical device features */
  VkSurfaceFormatKHR surface_format;  /* Requested surface format */
  VkPresentModeKHR present_mode;      /* Requested present mode */
  const char **extensions;            /* Requested extensions */
  uint32_t num_extensions;            /* Number of requested extensions */
  const char **layers;                /* Requested layers */
  uint32_t num_layers;                /* Number of requested layers */
} vk_dev_create_info_t;
/* Vulkan device */
typedef struct {
  VkPhysicalDevice physical_device;   /* Vulkan physical device */
  VkDevice device;                    /* Vulkan device */
  uint32_t graphics_queue_family;     /* Graphics queue family index */
  uint32_t present_queue_family;      /* Present queue family index */
  uint32_t compute_queue_family;      /* Compute queue family index */
  uint32_t transfer_queue_family;     /* Transfer queue family index */
  VkQueue graphics_queue;             /* Vulkan graphics queue */
  VkQueue present_queue;              /* Vulkan present queue */
  VkQueue compute_queue;              /* Vulkan compute queue */
  VkQueue transfer_queue;             /* Vulkan transfer queue */
} vk_dev_t;

/* Add requested extension */
extern void vk_dev_create_info_add_extension(
    vk_dev_create_info_t *vk_dev,
    const char *extension
);
/* Add requested layer */
extern void vk_dev_create_info_add_layer(
    vk_dev_create_info_t *vk_dev,
    const char *layer
);
/* Initialize vulkan device create info */
extern void vk_dev_create_info_init(
    vk_dev_create_info_t *vk_dev_create_info
);
/* Free vulkan device create info */
extern void vk_dev_create_info_free(
    vk_dev_create_info_t *vk_dev_create_info
);

/* Create vulkan device */
extern void vk_dev_create(
    vk_dev_t *vk_dev,
    vk_inst_t *vk_inst,
    vk_surf_t *vk_surf,
    vk_dev_create_info_t *vk_dev_create_info
);
/* Destroy vulkan device */
extern void vk_dev_destroy(
    vk_dev_t *vk_dev
);

#endif /* VK_DEV_H */
