/* Include guard */
#if !defined (BASE_H)
#define BASE_H

/* Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

/* Log levels */
typedef enum {
  LOG_LEVEL_SUCCESS,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARN,
  LOG_LEVEL_ERROR
} log_level_t;

/* Log a message */
extern void log_msg(log_level_t level, const char *message, ...);

/* Assertion */
#define ASSERT(condition) do {\
  if (!(condition)) {\
    log_msg(LOG_LEVEL_ERROR, "Assertion failed: \"%s\"", #condition);\
    abort();\
  }\
} while (0)

/* Check a vulkan result */
#define VK_CHECK(result) do {\
  VkResult res = (result);\
  if (res != VK_SUCCESS) {\
    log_msg(\
        LOG_LEVEL_ERROR,\
        "Vulkan expression \"%s\" failed with code: %s",\
        #result,\
        string_VkResult(res)\
    );\
    abort();\
  }\
} while (0)

/* Get the user to select an option from a list */
extern size_t get_option(
    const char **options,
    size_t count,
    const char *prompt,
    size_t default_option
);

#endif /* BASE_H */
