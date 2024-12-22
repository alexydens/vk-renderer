/* Implements vk_dev.h */
#include <vk_dev.h>

/* Create a Vulkan device builder */
vk_dev_builder_t vk_dev_builder(void) {
  vk_dev_builder_t builder;
  builder.extensions = NULL;
  builder.extension_count = 0;
  builder.layers = NULL;
  builder.layer_count = 0;
  return builder;
}
/* Add a Vulkan device extension */
void vk_dev_builder_add_ext(
    vk_dev_builder_t *builder,
    const char *ext
) {
  if (!builder->extensions) {
    builder->extensions = (const char **)malloc(sizeof(const char *));
    ASSERT(builder->extensions);
    builder->extension_count = 1;
    builder->extensions[0] = ext;
    return;
  }
  builder->extension_count++;
  builder->extensions = (const char **)realloc(
    builder->extensions, sizeof(const char *) * builder->extension_count
  );
  ASSERT(builder->extensions);
  builder->extensions[builder->extension_count - 1] = ext;
}
/* Add a Vulkan device layer */
void vk_dev_builder_add_layer(
    vk_dev_builder_t *builder,
    const char *layer
) {
  if (!builder->layers) {
    builder->layers = (const char **)malloc(sizeof(const char *));
    ASSERT(builder->layers);
    builder->layer_count = 1;
    builder->layers[0] = layer;
    return;
  }
  builder->layer_count++;
  builder->layers = (const char **)realloc(
    builder->layers, sizeof(const char *) * builder->layer_count
  );
  ASSERT(builder->layers);
  builder->layers[builder->layer_count - 1] = layer;
}
/* Create a Vulkan device (and free builder) */
vk_dev_t vk_dev_create(
    vk_inst_t *inst,
    vk_phys_dev_t *phys_dev,
    vk_dev_builder_t *builder
);
/* Destroy a Vulkan device */
void vk_dev_destroy(vk_dev_t *dev);
