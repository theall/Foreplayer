all: noconsole.exe

#CC = x86_64-w64-mingw32-gcc
CC = i686-w64-mingw32-gcc
#CXX = x86_64-w64-mingw32-g++
CXX = i686-w64-mingw32-g++

# For GUI application
FLAGS_SUBSYSTEM = -mwindows
# For Console application
#FLAGS_SUBSYSTEM =

# For Unicode application
FLAGS_UNICODE = -municode -DUNICODE -D_UNICODE
# For ANSI application
#FLAGS_UNICODE =

noconsole.exe: noconsole.c
	$(CC) -o $@ ${FLAGS_SUBSYSTEM} ${FLAGS_UNICODE} ${CFLAGS} $<

clean:
	rm -f *~ *.o
