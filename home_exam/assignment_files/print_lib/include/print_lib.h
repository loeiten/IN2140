#ifndef HOME_EXAM_ASSIGNMENT_FILES_PRINT_LIB_INCLUDE_PRINT_LIB_H_
#define HOME_EXAM_ASSIGNMENT_FILES_PRINT_LIB_INCLUDE_PRINT_LIB_H_

void print_pkt(unsigned char* packet);
void print_received_pkt(short ownAddress, unsigned char* packet);
void print_forwarded_pkt(short ownAddress, unsigned char* packet);
void print_weighted_edge(short from_node, short to_node, int weight);
void print_clear_logfile(void);

#endif  // HOME_EXAM_ASSIGNMENT_FILES_PRINT_LIB_INCLUDE_PRINT_LIB_H_
