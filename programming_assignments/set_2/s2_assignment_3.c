/*
 * Assignment:
 *
 * Write a program which takes a word as its first argument, and which counts
 * the number of occurrences of that word in a file which is given as the
 * second argument.
 * Extra: Make a program which counts the occurrence of every word in a file.
 * It can be wise to think through what data structure you use.
 */

#include <ctype.h>   // for isalnum
#include <libgen.h>  // for basename
#include <stdio.h>   // for fgetc, printf, EOF, fclose, fopen, FILE, NULL
#include <stdlib.h>  // for EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>  // for strlen

void count(FILE* fp, const char* word, int* hits, int* words) {
  // Initialize
  int wordLen = strlen(word);
  int matchIndex = 0;

  // Reset counters
  *hits = 0;
  *words = 0;

  // Definition: A word boundary is whatever is not a alphanumerical character
  // https://en.cppreference.com/w/c/string/byte/isalnum

  // Read character by character
  int c = fgetc(fp);

  while (c != EOF) {
    // Deal with non-alphanumerical characters
    // Skip consecutive non-alnum characters
    while (!isalnum(c) && (c != EOF)) {
      c = fgetc(fp);
    }

    // Exit if we hit EOF
    if (c == EOF) {
      break;
    }

    // Deal with alphanumerical characters
    // Find matches
    while (isalnum(c) && (c != EOF)) {
      if ((matchIndex < wordLen) && (c == word[matchIndex])) {
        ++matchIndex;
        c = fgetc(fp);
      } else {
        // No chance to find match, skip the rest
        while (isalnum(c) && (c != EOF)) {
          c = fgetc(fp);
        }
      }
    }

    // Count the word
    ++(*words);

    if (matchIndex == wordLen) {
      // Count the match
      ++(*hits);
    }
    // Reset the match index
    matchIndex = 0;
  }

  return;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    // NOTE: Base is from POSIX.1-2008, not the C-standard, see
    // https://www.unix.com/man-page/posix/3p/basename/
    // for specification and
    // https://github.com/coreutils/coreutils/blob/master/src/basename.c
    // for possible implementation
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
