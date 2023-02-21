#include "../include/person.h"

#include <stdio.h>   // for printf, fclose, fopen, NULL, ferror, fputs, fread
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS, atoi, malloc
#include <string.h>  // for strlen, strncpy, strcmp

#include "../include/file_operations.h"

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

int nameSet(const struct Person* person) {
  if (person == NULL) {
    return 0;
  }
  if (strcmp(person->name, "null") == 0) {
    return 0;
  }
  return 1;
}

int getName(const struct Person* person, char* name) {
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

int ageSet(const struct Person* person) {
  if (person == NULL) {
    return 0;
  }
  if (person->age == -1) {
    return 0;
  }
  return 1;
}

int getAge(const struct Person* person, int* age) {
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

int printPerson(const struct Person* person) {
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

int personToStr(const struct Person* person, char str[MAX_NAME_LEN + 4]) {
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
  const char* format = "%s,%d\n";
  int length = snprintf(NULL, 0, format, name, age);
  snprintf(str, length + 1, format, name, age);
  return EXIT_SUCCESS;
}

int readRegisterEntries(const char* path, struct Person* personArray,
                        int personArrayLen) {
  FILE* fp = fopen(path, "r");
  if (fp == NULL) {
    printf("Failed to open %s\n", path);
    return EXIT_FAILURE;
  }

  char* line = NULL;
  size_t len = 0;
  ssize_t nBytes;  // ssize_t is when the output can be negative

  // NOTE: getline is from POSIX.1-2008, not the C-standard, see
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getline.html
  // for specification, and
  // https://github.com/NetBSD/src/blob/trunk/tools/compat/getline.c
  // for possible implementation
  // Read the header
  for (int i = 0; i < 2; ++i) {
    nBytes = getline(&line, &len, fp);
    if (nBytes == -1) {
      printf("Failed to read line %d of %s\n", i, path);
      return EXIT_FAILURE;
    }
  }

  // Read the entries
  for (int i = 0; i < personArrayLen; ++i) {
    nBytes = getline(&line, &len, fp);
    if (nBytes == -1) {
      printf("Failed to entry %d of %s\n", i + 2, path);
      return EXIT_FAILURE;
    }
    char* curName;
    int curAge;
    // NOTE: We are changing curName, so we call using the address of curName
    int exitFailure = strToNameAge(line, &curName, &curAge);
    if (exitFailure) {
      printf("Failed to get entry %d of %s\n", i, path);
      return EXIT_FAILURE;
    }
    exitFailure = createPerson(curName, curAge, &(personArray[i]));
    free(curName);
    if (exitFailure) {
      return EXIT_FAILURE;
    }
  }

  free(line);

  fclose(fp);
  return EXIT_SUCCESS;
}

// NOTE: We are changing the pointer so we take a pointer to the pointer as an
//       input
int strToNameAge(const char* str, char** name, int* age) {
  // Find the length of the string and the length until comma
  int strLen = strlen(str);
  int i = 0;
  for (; i < strLen; ++i) {
    if (str[i] == ',') {
      break;
    }
  }

  if ((i == 0) || (i == strLen)) {
    printf("Failed to split %s into 'name' and 'age'\n", str);
    return EXIT_FAILURE;
  }

  // Increment i once to go from index to counter
  ++i;

  // Capture the name
  // +1 for the \0 char
  *name = (char*)malloc((i + 1) * sizeof(char));
  snprintf(*name, i, "%s", str);

  // Capture the age
  const char* ageStr = &(str[i]);
  *age = atoi(ageStr);

  return EXIT_SUCCESS;
}

// NOTE: We change the personArray, hence we must pass a pointer to the array
int readRegister(const char* path, struct Person** personArray,
                 int* personArrayLen) {
  // Get number of lines
  int lines;
  int exitFailure = getNumberOfLines(path, &lines);
  if (exitFailure) {
    printf("Failed to obtain the number of lines of %s\n", path);
    return EXIT_FAILURE;
  }

  // Get number of entries (subtract 2 due to header)
  (*personArrayLen) = lines - 2;

  // Allocate array to the personArray
  *personArray =
      (struct Person*)malloc((*personArrayLen) * sizeof(struct Person));

  // Read entries and store it to the personArray
  exitFailure = readRegisterEntries(path, *personArray, *personArrayLen);
  if (exitFailure) {
    free(*personArray);
    printf("Failed to obtain the entries from %s\n", path);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int storeRegister(const char* path, const struct Person* personArray,
                  const int personArrayLen) {
  FILE* fp = fopen(path, "w");
  if (fp == NULL) {
    printf("Failed to open %s\n", path);
    return EXIT_FAILURE;
  }

  // Print the header
  fputs("Name,Age\n\n", fp);

  for (int i = 0; i < personArrayLen; ++i) {
    char str[MAX_NAME_LEN + 4];
    if (!(nameSet(&(personArray[i]))) && !(ageSet(&(personArray[i])))) {
      continue;
    }
    personToStr(&(personArray[i]), str);
    int success = fputs(str, fp);
    if (success == EOF) {
      printf("Failed to write '%s' to %s\n", str, path);
      return EXIT_FAILURE;
    }
  }

  fclose(fp);
  return EXIT_SUCCESS;
}
