# project name: unzip
project(unzip)

# set source files
set(unzip unzip.c unzip.h)

# set make to a static library
add_library(unzip STATIC ${unzip})

install(FILES unzip.h ../include)
install(TARGETS unzip ../lib)