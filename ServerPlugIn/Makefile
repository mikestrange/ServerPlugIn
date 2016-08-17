CC := g++  

ROOT := $(shell pwd)

CFLAGS := -Wall -g -std=c++11  

TARGET := main  

DIRS := $(shell find . -maxdepth 2 -type d)

SRCS := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
#$(wildcard *.cpp)

OBJS := $(patsubst %cpp,%o,$(SRCS))  

#递归执行make
#EACH_MAKE := $(foreach dir,$(ROOT),make -C $(dir);)

# 宏定义 
DEFS := -DDEBUG
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
LIBS += -L./lib -lmysqlclient -lmysqld
$(info 'this is mac makefile')
endif


#最后添加
CFLAGS += $(DEFS) 
    
%.o:%.cpp  
	$(CC) $(CFLAGS) $(INC) -c $<  
	
$(TARGET):$(OBJS)  
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^ 

clean:
	$(shell find . -name '*.o' |xargs rm -rf) 
	
	
 
