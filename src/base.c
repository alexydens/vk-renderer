/* Implements base.h */
#include <base.h>
#include <stdarg.h>

/* Log a message */
void log_msg(log_level_t level, const char *message, ...) {
  va_list args;
  va_start(args, message);
  switch (level) {
    case LOG_LEVEL_SUCCESS:
      printf("\033[1;32m[SUCCESS]\033[0m: ");
      break;
    case LOG_LEVEL_INFO:
      printf("\033[1;37m[INFO]\033[0m: ");
      break;
    case LOG_LEVEL_WARN:
      printf("\033[1;33m[WARN]\033[0m: ");
      break;
    case LOG_LEVEL_ERROR:
      printf("\033[1;31m[ERROR]\033[0m: ");
      break;
  }
  vprintf(message, args);
  printf("\n");
  va_end(args);
}

/* Get the user to select an option from a list */
size_t get_option(
    const char **options,
    size_t count,
    const char *prompt,
    size_t default_option
) {
  size_t option;
  printf("%s\n", prompt);
  for (size_t i = 0; i < count; i++) {
    printf("%zu. %s\n", i, options[i]);
  }
  printf("> ");
  char buff[32];
  if (fgets(buff, sizeof(buff), stdin) == NULL) {
    return default_option;
  }
  if (sscanf(buff, "%zu", &option) != 1) {
    return default_option;
  }
  if (option >= count) {
    return default_option;
  }
  return option;
}
