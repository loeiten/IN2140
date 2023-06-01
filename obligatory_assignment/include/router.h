#ifndef OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
#define OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_

struct Router {
  unsigned char routerId;
  unsigned char flag;
  const char* producerModel;
  struct Router* neighbors[10];  // Assignment specifies that a router can have
                                 // maximum 10 connections
};

#endif  // OBLIGATORY_ASSIGNMENT_INCLUDE_ROUTER_H_
