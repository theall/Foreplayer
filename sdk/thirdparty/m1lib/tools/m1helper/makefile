CC = gcc
XX = g++
CFLAGS = -Wall -O ¨Cg
TARGET = ./m1

SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))

$(TARGET):$(OBJS)
	$(XX) $(OBJS) -I../ -lm1 -o $(TARGET)

clean:
	rm -rf *.o noconsole
