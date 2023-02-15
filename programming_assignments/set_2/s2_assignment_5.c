#include <libgen.h>  // for basename
#include <stdio.h>   // for printf, fclose, fopen, NULL, ferror, fputs, fread
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS, atoi, malloc
#include <string.h>

#include "../../utils/include/string_helpers.h"

#define MAX_NAME_LEN 30
#define MAX_AGE 200

struct Person {
  // Without the specified buffer size, we would've need dynamic allocation
  // https://stackoverflow.com/a/9593851/2786884
  char name[MAX_NAME_LEN];
  int age;
};

int setName(struct Person* person, const char* name) {
  if (person == NULL) {
    printf("Tried to write to null-pointer\n");
    return EXIT_FAILURE;
  }
  int length = strlen(name);

  if (length > MAX_NAME_LEN) {
    printf("%s is > max allowed chars (%d)\n", name, MAX_NAME_LEN);
    return EXIT_FAILURE;
  }

  strncpy(person->name, name, length + 1);

  return EXIT_SUCCESS;
}

int nameSet(struct Person* person) {
  if (person == NULL) {
    return 0;
  }
  if (strcmp(person->name, "null") == 0) {
    return 0;
  }
  return 1;
}

int getName(struct Person* person, char* name) {
  if (!nameSet(person)) {
    printf("Name was not set\n");
    return EXIT_FAILURE;
  }
  int length = strlen(person->name);
  strncpy(name, person->name, length + 1);
  return EXIT_SUCCESS;
}

int setAge(struct Person* person, int age) {
  if (person == NULL) {
    printf("Tried to write to null-pointer\n");
    return EXIT_FAILURE;
  }
  if (age > MAX_AGE) {
    printf("%d is larger than max allowed age (%d)\n", age, MAX_AGE);
    return EXIT_FAILURE;
  }

  person->age = age;
  return EXIT_SUCCESS;
}

int ageSet(struct Person* person) {
  if (person == NULL) {
    return 0;
  }
  if (person->age == -1) {
    return 0;
  }
  return 1;
}

int getAge(struct Person* person, int* age) {
  if (!ageSet(person)) {
    printf("Age was not set\n");
    return EXIT_FAILURE;
  }
  *age = person->age;
  return EXIT_SUCCESS;
}

int createPerson(const char* name, int age, struct Person* person) {
  int exitFailure = setName(person, name);
  if (exitFailure) {
    return EXIT_FAILURE;
  }
  exitFailure = setAge(person, age);
  if (exitFailure) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int printPerson(struct Person* person) {
  char name[MAX_NAME_LEN];
  int exitFailure = getName(person, name);
  if (exitFailure) {
    return EXIT_FAILURE;
  }
  int age;
  exitFailure = getAge(person, &age);
  if (exitFailure) {
    return EXIT_FAILURE;
  }
  const char* format = "Name: %s, age: %d\n";
  int length = snprintf(NULL, 0, format, name, age);
  char* str = malloc(length + 1);
  snprintf(str, length + 1, format, name, age);
  printf("%s", str);
  free(str);
  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s STR INT ...\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  int i = 1;
  while (i < argc) {
    // Always initialize the person to some non-valid values
    struct Person person = {"null", -1};

    // Capture name
    char* name;
    int bufferLen = strlen(argv[i]) + 1;
    name = (char*)malloc(bufferLen * sizeof(char));
    snprintf(name, bufferLen, "%s", argv[i]);

    // Capture age
    ++i;
    if (i == argc) {
      printf("Not enough arguments given\n");
      return EXIT_FAILURE;
    }

    int age;
    int exitFailure = getInt(argv[i], &age);
    if (exitFailure) {
      printf("Failed to capture the age from '%s'", argv[i]);
      return EXIT_FAILURE;
    }

    // Create person
    setName(&person, name);
    setAge(&person, age);

    // Print the person
    printPerson(&person);

    // Clean-up
    free(name);

    ++i;
  }

  return EXIT_SUCCESS;
}
