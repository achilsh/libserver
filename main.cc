#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <glog/logging.h>

#include "server.h" 
#include "proto/machine_study.pb.h"

using namespace std::placeholders;

static void readcb(int fd, char* buf, int len)
{
    net::Coder coder;
    coder.decoding(buf, len);
    printf("msg %s\n", coder.getMsgName().c_str());
    invorkfun(coder.getMsgName(), coder.getBody());
}
static void onimage(const message_t & v)
{
    mstudy::ImageData * msg = dynamic_cast<mstudy::ImageData *>(v.get());
    printf("t_id %ld width %d height %d\n",msg->st_id(), msg->resize_width(), msg->resize_height()); 
}
int main(int argc, char * argv[])
{
    int port = atoi(argv[1]);
    registerfunc<mstudy::ImageData>(std::bind(onimage,_1));
	co_setreadcb(readcb);
    co_service(port, 2);
    return 0;
}
