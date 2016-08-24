#设置编译器
CC := g++

CFLAGS := -Wall -g -std=c++11

ROOT_DIR := $(shell pwd)

# 宏定义 
DEFS := -DRELEASE
#路径
INC :=
#链接库
LIBS :=
#控制
#LINUX = ak

ifdef LINUX
DEFS += -D_MY_SQL
INC += -I/usr/include -I/usr/local/include
LIBS += -pthread
$(info 'this is linux makefile')
else
DEFS += -DMY_SQL
INC += -I/usr/include -I/usr/local/include -I/usr/local/mysql/include
LIBS += -L./libs -lmysqlclient -lmysqld
$(info 'this is mac makefile')
endif

#最后添加
CFLAGS += $(DEFS)
CFLAGS += $(INC) 
CFLAGS += $(LIBS) 

SUBDIRS := $(shell ls -l | grep ^d | awk '{if($$9 != "debug") print $$9}')

CUR_SOURCE := ${wildcard *.cpp}

CUR_OBJS := ${patsubst %.cpp, %.o, $(CUR_SOURCE)}

all:$(SUBDIRS) $(CUR_OBJS)


$(SUBDIRS):ECHO
	make -C $@


ECHO:
	@echo $(SUBDIRS)


$(CUR_OBJS):%.o:%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@


clean:
	$(shell find . -name '*.o' |xargs rm -rf) 
