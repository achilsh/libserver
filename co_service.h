#ifndef _CO_SERVICE_H_
#define _CO_SERVICE_H_
/* Copyright (c) 2016~2018 chenwj; see COPYRIGHT 
 * link:-lproto -llibco -std=c++11 -pthread -lpthread -ldl -lserver  at linux
 * note: max package size 16*1024
*/
#include <unistd.h>
#include <stdio.h>
#include <co_routine.h>

typedef void (*readcallback_t)(int fd, char* buf, int len);
typedef void (*closecallback_t)(int fd, int err);

/*  @progress_num the number of sub progress */
int co_service(int port, int progress_num);
int co_connect(const char *ip, int port);
void co_loop();

void co_setreadcb(readcallback_t f);
void co_setclosecb(closecallback_t f);
#endif
