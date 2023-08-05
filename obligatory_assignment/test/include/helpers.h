#ifndef OBLIGATORY_ASSIGNMENT_TEST_INCLUDE_HELPERS_H_
#define OBLIGATORY_ASSIGNMENT_TEST_INCLUDE_HELPERS_H_

struct FTW;
struct stat;

int strToIntArray(const char *const arrayStr, int *const intArray);
int removeFunction(const char *path, const struct stat *sb, int typeFlag,
                   struct FTW *ftwBuf);
int removeRecursively(const char *path);

#endif  // OBLIGATORY_ASSIGNMENT_TEST_INCLUDE_HELPERS_H_
