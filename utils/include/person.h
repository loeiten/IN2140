#ifndef UTILS_INCLUDE_PERSON_H_
#define UTILS_INCLUDE_PERSON_H_

#define MAX_NAME_LEN (30)
#define MAX_AGE (200)

struct Person {
  // Without the specified buffer size, we would've need dynamic allocation
  // https://stackoverflow.com/a/9593851/2786884
  char name[MAX_NAME_LEN];
  int age;
};

int setName(struct Person* person, const char* name);
int nameSet(const struct Person* person);
int getName(const struct Person* person, char* name);

int setAge(struct Person* person, const int age);
int ageSet(const struct Person* person);
int getAge(const struct Person* person, int* age);

int createPerson(const char* name, int age, struct Person* person);
int printPerson(const struct Person* person);
// +2 for age, +1 for , and +1 for \0
int personToStr(const struct Person* person, char str[MAX_NAME_LEN + 4]);

int strToNameAge(const char* str, char** name, int* age);
int readRegister(const char* path, struct Person** personArray,
                 int* personArrayLen);
int storeRegister(const char* path, const struct Person* personArray,
                  int personArrayLen);

#endif  // UTILS_INCLUDE_PERSON_H_
