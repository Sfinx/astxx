
#DEBUG		= -g3 -pg
OPTIMIZE	= -O3
DEFINES 	= -D__USE_REENTRANT -D_REENTRANT -D_THREAD_SAFE \
			-D_LIBC_REENTRANT -D_GNU_SOURCE
INCLUDES	= -I./include
CXXFLAGS	= -Wall -std=gnu++11 $(DEFINES) $(INCLUDES) $(DEBUG) $(OPTIMIZE)
LIBS		= -L./libs -lastxx_manager -lboost_signals -lpthread -lboost_system
LDFLAGS		= $(STATIC) $(DEBUG) -Wl,-O6 -Wl,--start-group $(LIBS) -Wl,--end-group
TARGETS		= libastxx_manager libastxx_agi test

MANAGER_OBJS = src/manager/basic_action.o src/manager/connection.o src/manager/error.o
AGI_OBJS = src/agi.o
TEST_OBJS = examples/test.o

all:	do-it-all

ifeq (.depend,$(wildcard .depend))
include .depend
do-it-all:	$(TARGETS)
else
do-it-all:	depend $(TARGETS)
endif

libastxx_manager:	$(MANAGER_OBJS)
	@mkdir -p libs
	@ar cr libs/libastxx_manager.a $(MANAGER_OBJS)
	@ranlib libs/libastxx_manager.a

libastxx_agi:	$(AGI_OBJS)
	@mkdir -p libs
	@ar cr libs/libastxx_agi.a $(AGI_OBJS)
	@ranlib libs/libastxx_agi.a

test:	$(TEST_OBJS) libastxx_manager
	g++ $(TEST_OBJS) -o examples/test $(LDFLAGS)

.cpp.o:
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f DEADJOE libs/* examples/test .depend $(TEST_OBJS) $(AGI_OBJS) $(MANAGER_OBJS)  *~ *core nohup.out && echo Clean Ok.
                
dep depend:
	$(CXX) $(CXXFLAGS) -M src/manager/*.cc src/*.cc > .depend
