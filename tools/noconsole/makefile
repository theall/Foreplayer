CC = gcc
XX = g++
CFLAGS = -Wall -O ¨Cg
TARGET = ./noconsole

SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))

$(TARGET):$(OBJS)
	$(XX) $(OBJS) -o $(TARGET)

clean:
	rm -rf *.o noconsole
