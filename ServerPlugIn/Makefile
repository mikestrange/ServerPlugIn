CC := g++  

CFLAGS := -Wall -g -std=c++11  

TARGET := main  

SRCS := $(wildcard *.cpp)  

OBJS := $(patsubst %cpp,%o,$(SRCS))  

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
	rm -rf $(TARGET) *.o  
