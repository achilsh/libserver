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
typedef void (*acceptcallback_t)(int fd, char* ip, int port);

/*  @progress_num the number of sub progress */
int co_service(int port, int progress_num);
int co_connect(const char *ip, int port);
void co_loop();
int co_connect_noloop(const char *ip, int port);

int co_send(int fd, const char * buf, int len);
void co_setreadcb(readcallback_t f);
void co_setclosecb(closecallback_t f);
void co_setaccpectcb(acceptcallback_t f);
#endif
