#include "../include/person.h"

#include <stdio.h>   // for printf, fclose, fopen, NULL, ferror, fputs, fread
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS, atoi, malloc
#include <string.h>  // for strlen, strncpy, strcmp

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
