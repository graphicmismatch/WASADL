# Executable name
EXEC = WASADL

# Source files
SRC = WASADL.cpp ./lib/tinyexpr/tinyexpr.c
OBJ = $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

# Compiler and flags
CXX = g++
CC = gcc

# Paths for local libsndfile build
SNDFILE_INC = ./lib/libsndfile/include
SNDFILE_LIB = ./lib/libsndfile/.libs

# Flags
CFLAGS  = -Wall -Wshadow -O2 -I$(SNDFILE_INC) -I./lib/tinyexpr
LDFLAGS = -lm -L$(SNDFILE_LIB) -lsndfile

# Default target
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependency installation
install-deps:
	mkdir -p lib
	cd lib && git clone https://github.com/graphicmismatch/tinyexpr
	cd lib/tinyexpr && $(CC) -c $(CFLAGS) tinyexpr.c -o tinyexpr.o
	cd lib && git clone https://github.com/libsndfile/libsndfile.git
	cd lib/libsndfile && autoreconf -vif && ./configure --disable-shared --enable-static && $(MAKE) -j

.PHONY: install-deps

update-deps:
	cd lib/tinyexpr && $(CC) -c $(CFLAGS) tinyexpr.c -o tinyexpr.o
#	cd lib/libsndfile && autoreconf -vif && ./configure --disable-shared --enable-static && $(MAKE) -j

.PHONY: update-deps
# Clean up
clean:
	-rm -f $(EXEC) $(OBJ)

.PHONY: clean uninstall-deps

# Uninstall deps
uninstall-deps:
	cd lib/libsndfile && $(MAKE) uninstall || true
	rm -rf lib/libsndfile
	rm -rf lib

