CC := g++  

CFLAGS := -Wall -g -std=c++11  

TARGET := main  

SRCS := $(wildcard *.cpp)  

OBJS := $(patsubst %cpp,%o,$(SRCS))  

# 宏定义  
DEFS = -DMY_SQL
CFLAGS += $(DEFS) 
#路径
INC := -I/usr/include\
	-I/usr/local/include\
	-I/usr/local/mysql/include
	  
#链接库
#LIBS := -pthread
LIBS += -L./lib -lmysqlclient -lmysqld


%.o:%.cpp  
	$(CC) $(CFLAGS) $(INC) -c $<  
	
$(TARGET):$(OBJS)  
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^ 

clean:
	rm -rf $(TARGET) *.o  
