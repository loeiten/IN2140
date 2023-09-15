#ifndef HOME_EXAM_ASSIGNMENT_FILES_PRINT_LIB_INCLUDE_PRINT_LIB_H_
#define HOME_EXAM_ASSIGNMENT_FILES_PRINT_LIB_INCLUDE_PRINT_LIB_H_

#include <stdio.h>  // for FILE

void print_pkt(unsigned char* packet);
void print_received_pkt(short ownAddress, unsigned char* packet);
void print_forwarded_pkt(short ownAddress, unsigned char* packet);
void print_weighted_edge(short from_node, short to_node, int weight);
void print_clear_logfile(void);
// These two functions are added for good book-keeping, and are not part of the
// original library
FILE* get_logfile(const char* mode);
int make_directories(const char* const directories);
int concatenate_path(char** const path, const char* const log_dir,
                     const char* const format);

#endif  // HOME_EXAM_ASSIGNMENT_FILES_PRINT_LIB_INCLUDE_PRINT_LIB_H_
