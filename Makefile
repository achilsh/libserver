# complie libserver.so
SRCS = co_service.cc coder.cc message.cc 
OBJS  = $(patsubst %.cc,%.o,$(SRCS))

CFLAGS += -g -fno-strict-aliasing -O2 -Wall -export-dynamic -Wall -pipe  -D_GNU_SOURCE -D_REENTRANT -fPIC -Wno-deprecated -m64 -std=c++11 -pthread
LINKS +=  -lcolib -lpthread -ldl  
FLAGS = -std=c++11 -pthread -O2 -Wall
TARGET = libserver.so

all: $(libserver) $(OBJS) 
	@echo $(OBJS)
	g++ -o app ./proto/machine_study.pb.cc main.cc $(FLAGS) -I. -I/usr/work/libco/ -L/usr/work/libco/lib/ -L./ -lserver -lcolib -lprotobuf -ldl  
	$(RM) -f $(OBJS)

libserver:
	$(CXX) -fPIC -shared -o libserver.so $(SRCS) $(FLAGS)
	$(RM) -f $(OBJS)

CPPCOMPILE = g++ -shared -fPIC  -c $< $(FLAGS)  -o $@

.cpp.o:
	$(CPPCOMPILE)
.cc.o:
	$(CPPCOMPILE)
.c.o:
	$(CCCOMPILE)

clean:
	$(RM) -f $(OBJS)
