#ifndef UTILS_INCLUDE_PERSON_H_
#define UTILS_INCLUDE_PERSON_H_

#define MAX_NAME_LEN 30
#define MAX_AGE 200

struct Person {
  // Without the specified buffer size, we would've need dynamic allocation
  // https://stackoverflow.com/a/9593851/2786884
  char name[MAX_NAME_LEN];
  int age;
};

int setName(struct Person* person, const char* name);
int nameSet(struct Person* person);
int getName(struct Person* person, char* name);

int setAge(struct Person* person, int age);
int ageSet(struct Person* person);
int getAge(struct Person* person, int* age);

int createPerson(const char* name, int age, struct Person* person);
int printPerson(struct Person* person);

#endif  // UTILS_INCLUDE_PERSON_H_
