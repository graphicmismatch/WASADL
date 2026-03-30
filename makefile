# Executable name
EXEC = WASADL

# Source files
CPP_SRC = WASADL.cpp
C_SRC   = ./lib/tinyexpr/tinyexpr.c

CPP_OBJ = $(CPP_SRC:.cpp=.o)
C_OBJ   = $(C_SRC:.c=.o)
OBJ     = $(CPP_OBJ) $(C_OBJ)

# Compilers
CXX = g++
CC  = gcc

# Paths
SNDFILE_DIR = ./lib/libsndfile
SNDFILE_BUILD = $(SNDFILE_DIR)/build
SNDFILE_LIB = $(SNDFILE_BUILD)/libsndfile.a
SNDFILE_INC = $(SNDFILE_DIR)/include

# Flags
CFLAGS  = -Wall -Wshadow -O2 -I$(SNDFILE_INC) -I./lib/tinyexpr
CXXFLAGS = $(CFLAGS)
LDFLAGS = $(SNDFILE_LIB) -lm

# Default target
all: $(EXEC)

# Link
$(EXEC): $(OBJ) $(SNDFILE_LIB)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

# Compile C++
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Install dependencies
install-deps:
	mkdir -p lib
	cd lib && git clone https://github.com/graphicmismatch/tinyexpr || true
	cd lib && git clone https://github.com/libsndfile/libsndfile.git || true
	cd $(SNDFILE_DIR) && mkdir -p build
	cd $(SNDFILE_BUILD) && \
		cmake .. \
		-DBUILD_SHARED_LIBS=OFF \
		-DENABLE_EXTERNAL_LIBS=OFF \
		-DCMAKE_BUILD_TYPE=Release
	cd $(SNDFILE_BUILD) && $(MAKE) -j$$(nproc)

# Rebuild only libsndfile
rebuild-libsndfile:
	rm -rf $(SNDFILE_BUILD)
	mkdir -p $(SNDFILE_BUILD)
	cd $(SNDFILE_BUILD) && \
		cmake .. \
		-DBUILD_SHARED_LIBS=OFF \
		-DENABLE_EXTERNAL_LIBS=OFF \
		-DCMAKE_BUILD_TYPE=Release
	cd $(SNDFILE_BUILD) && $(MAKE) -j$$(nproc)

# Clean build artifacts
clean:
	rm -f $(EXEC) $(OBJ)

# Remove dependencies
uninstall-deps:
	rm -rf lib

.PHONY: all clean install-deps uninstall-deps rebuild-libsndfile