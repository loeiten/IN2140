#include "../include/command.h"

#include <stdlib.h>
#include <string.h>

int getCommand(const char *commandStr, const char *command, const char **args) {
  // The first word is always the command
  // Find the first space
  // Use strtok to find spaces

  // Special case for set_model
  if (strcmp(command, "setModel") == 0) {
    // The following word is the routerID
    // The rest of the string is the model name and can contain spaces
    return EXIT_SUCCESS;
  }

  // The arguments are divided by spaces

  return EXIT_SUCCESS;
}
