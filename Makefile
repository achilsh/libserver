# complie libserver.so
SRCS = co_service.cc coder.cc message.cc 
OBJS  = $(patsubst %.cc,%.o,$(SRCS))

CFLAGS += -g -fno-strict-aliasing -O2 -Wall -export-dynamic -Wall -pipe  -D_GNU_SOURCE -D_REENTRANT -fPIC -Wno-deprecated -m64 -std=c++11 -pthread
LINKS +=  -lcolib -lpthread -ldl  
FLAGS = -std=c++11 -pthread -O2 -Wall
TARGET = libserver.so

INSTANLL_ROOT = /usr/local

all: libserver $(OBJS) 
	@echo $(OBJS)
	g++ -g -o app ./proto/machine_study.pb.cc main.cc $(FLAGS) -I. -I/usr/work/libco/ -L/usr/work/libco/lib/ -L./ -lserver -lcolib -lglog -lprotobuf -ldl  
	$(RM) -f $(OBJS)

libserver:
	$(CXX) -g -fPIC -shared -o libserver.so $(SRCS) $(FLAGS)
	$(RM) -f $(OBJS)

CPPCOMPILE = g++ -g -shared -fPIC  -c $< $(FLAGS)  -o $@

.cpp.o:
	$(CPPCOMPILE)
.cc.o:
	$(CPPCOMPILE)
.c.o:
	$(CCCOMPILE)

clean:
	$(RM) -f $(OBJS)

install:
	cp -f co_service.h coder.h message.h server.h $(INSTANLL_ROOT)/include/
	cp -f $(TARGET) $(INSTANLL_ROOT)/lib/ 

uninstall:
	rm -f $(INSTANLL_ROOT)/co_service.h $(INSTANLL_ROOT)/coder.h $(INSTANLL_ROOT)/message.h $(INSTANLL_ROOT)/server.h
	rm -f $(INSTANLL_ROOT)/lib/$(TARGET)
