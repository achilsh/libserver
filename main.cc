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
    invorkfun(fd, coder.getMsgName(), coder.getBody());
}
static void heart(int fd, const message_t & v)
{
    mstudy::Heart * msg = dynamic_cast<mstudy::Heart *>(v.get());
    LOG(INFO) << "Heart: " << msg->heart();
}
int main(int argc, char * argv[])
{
    int port = atoi(argv[1]);
    registerfunc<mstudy::Heart>(std::bind(heart,_1, _2));
	co_setreadcb(readcb);
    co_service(port, 2);
    return 0;
}
