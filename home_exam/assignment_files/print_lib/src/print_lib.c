#include "../include/print_lib.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_OK (0)

#define LEN_SWAP (0x0001)
#define BAD_LENGTH (0x0002)
#define MINOR_LENGTH_BUG (0x0004)
#define LENGTH_BUG (0x0008)
#define STRING_TERM_BUG (0x0010)
#define SHORT_STRING_BUG (0x0020)
#define TEST_CODE_BUG (0x0800)
#define INCOMPLETE_PACKET (0x8000)

static unsigned int validate_packet(unsigned char* packet) {
  short tmp;
  unsigned int minor = PACKET_OK;
  memcpy(&tmp, &packet[0], sizeof(tmp));
  unsigned short length = ntohs(tmp);

  if (length < 6) {
    return INCOMPLETE_PACKET;
  }
  /* Unused variables
  unsigned short source = ntohs( *(short*)&packet[2] );
  unsigned short dest   = ntohs( *(short*)&packet[4] );*/
  char* msg = (char*)&packet[6];

  if (length > 1500) {
    if (ntohs(length) > 1500) {
      length = 1500;
      minor |= BAD_LENGTH;
    } else {
      length = ntohs(length);
      minor |= LEN_SWAP;
    }
  }

  if (packet[length - 1] != '\0') {
    size_t slen = strlen(msg);
    if (slen + 7 == length) {
      minor |= TEST_CODE_BUG;
    } else if (length >= slen && length < slen + 7) {
      minor |= MINOR_LENGTH_BUG;
    } else if (length > slen) {
      minor |= LENGTH_BUG;
    } else {
      minor |= STRING_TERM_BUG;
    }
  } else if (length != strlen(msg) + 7) {
    minor |= SHORT_STRING_BUG;
  }
  return minor;
}

static void print_msg(FILE* logfile, unsigned int chk, short ownAddress,
                      const char* operation, unsigned char* packet) {
  if (chk & INCOMPLETE_PACKET) {
    fprintf(logfile, "[1] created incomplete PKT\n");
  } else {
    short tmp;
    unsigned short length;
    unsigned short source;
    unsigned short dest;
    char* msg;

    memcpy(&tmp, &packet[0], sizeof(tmp));
    length = ntohs(tmp);
    /* Switched source and dest
    source = ntohs( *(short*)&packet[2] );
    dest   = ntohs( *(short*)&packet[4] );*/
    memcpy(&tmp, &packet[2], sizeof(tmp));
    dest = ntohs(tmp);
    memcpy(&tmp, &packet[4], sizeof(tmp));
    source = ntohs(tmp);
    msg = (char*)&packet[6];

    if (chk & LEN_SWAP) {
      memcpy(&tmp, &packet[0], sizeof(tmp));
      length = ntohs(tmp);
    }

    unsigned short msgMalloc = 0;
    if (chk & STRING_TERM_BUG) {
      msgMalloc = 1;
      if (length - 6 > 0)
        msg = strndup((char*)&packet[6], length - 6);
      else
        msg = strdup("<empty>");
    }

    fprintf(logfile, "[%d] %s PKT len %d src %d dst %d FLG %x msg %s \n",
            ownAddress, operation, length, source, dest, chk, msg);

    if ((chk & STRING_TERM_BUG) && msgMalloc) {
      free(msg);
    }
  }
}

void print_pkt(unsigned char* packet) {
  unsigned int chk = validate_packet(packet);

  FILE* logfile = fopen("logfile.txt", "a");
  print_msg(logfile, chk, 1, "CREATE", packet);
  fclose(logfile);
}

void print_received_pkt(short ownAddress, unsigned char* packet) {
  unsigned int chk = validate_packet(packet);

  FILE* logfile = fopen("logfile.txt", "a");
  print_msg(logfile, chk, ownAddress, "RCV", packet);
  fclose(logfile);
}

void print_forwarded_pkt(short ownAddress, unsigned char* packet) {
  unsigned int chk = validate_packet(packet);

  FILE* logfile = fopen("logfile.txt", "a");
  print_msg(logfile, chk, ownAddress, "FWD", packet);
  fclose(logfile);
}

void print_weighted_edge(short from_node, short to_node, int weight) {
  FILE* logfile = fopen("logfile.txt", "a");
  if (weight < 0) {
    fprintf(logfile, "[C]: %d IS NOT on path from 1 to %d\n", from_node,
            to_node);
  } else {
    fprintf(logfile, "[C]: distance from 1 to %d is %d\n", from_node, weight);
    fprintf(logfile, "[C]: %d IS on path from 1 to %d\n", from_node, to_node);
  }
  fclose(logfile);
}

void print_clear_logfile(void) {
  FILE* logfile = fopen("logfile.txt", "w");
  fclose(logfile);
}
