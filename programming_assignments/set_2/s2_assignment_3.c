#include <ctype.h>
#include <libgen.h>  // for basename
#include <stdio.h>   // for printf
#include <stdlib.h>  // for atoi, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strlen

void count(FILE* fp, const char* word, int* hits, int* words) {
  // Initialize
  int wordLen = strlen(word);
  int matchIndex = 0;

  // Reset counters
  hits = 0;
  words = 0;

  // Definition: A word boundary is whatever is not a alphanumerical character
  // https://en.cppreference.com/w/c/string/byte/isalnum

  // Read character by character
  int c = getc(fp);

  // Skip initial whitespaces
  while (!isalnum(c)) {
    c = getc(fp);
  }

  while (c != EOF) {
    // Deal with non-alphanumerical characters
    if (!isalnum(c)) {
      // Count the current as a word
      ++words;
      if (matchIndex == (wordLen - 1)) {
        // Count the match
        ++hits;
      }
      // Skip consecutive non-alnum characters
      while (!isalnum(c)) {
        c = getc(fp);
      }
      // Reset the word length and the match index
      matchIndex = 0;
    }

    // Deal with alphanumerical characters
    if (isalnum(c)) {
      if (c == word[matchIndex]) {
        ++matchIndex;
      } else {
        matchIndex = 0;
      }
    }

    // Update the character
    c = getc(fp);
  }

  return;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    // NOTE: Base does only work for UNIX systems
    printf("Usage: ./%s STR PATH\n", basename(argv[0]));
    return EXIT_FAILURE;
  }

  // Capture the word
  const char* word = argv[1];
  int hits;

  // Capture the path
  const char* path = argv[2];
  int words;

  // Open the file
  FILE* fp;
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("%s not found", path);
    return EXIT_FAILURE;
  }

  // Count hits and words
  count(fp, word, &hits, &words);

  // Close the file
  fclose(fp);

  // Print the result
  printf("%d/%d\n", hits, words);

  return EXIT_SUCCESS;
}
