#!/bin/sh

# user can override defaults here
include config.mk

# find source files
src_dir := src
src_dirs := $(shell find $(src_dir) -type d)

# make available to implicit rules
vpath %.c $(src_dirs)
vpath %.cpp $(src_dirs)
vpath %.h $(src_dirs)
vpath %.hpp $(src_dirs)

c_srcs := $(foreach d, $(src_dirs), $(wildcard $(d)/*.c))
cpp_srcs := $(foreach d, $(src_dirs), $(wildcard $(d)/*.cpp))

objs := $(c_srcs:.c=.o) $(cpp_srcs:.cpp=.o)

target := vtr

# default toolchain
CC      ?= gcc
CXX     ?= g++
AR      ?= ar
CFLAGS  += -Wall -Wextra -Wformat -MMD -MP -O2 
CXXFLAGS += -Wall -Wextra -Wformat -MMD -MP -O2 -std=c++17
LDFLAGS += 

inc_dirs := $(src_dirs)
CPPFLAGS += $(addprefix -I, $(inc_dirs))
			 
# posix 
ifeq ($(shell uname -s), Linux)
	LDFLAGS += -lc -lvulkan -lSDL3 -lpthread
endif

# windows
ifeq ($(OS), Windows_NT)
	LDFLAGS += -L$(VULKAN_DIR)/Lib -lc -lm -lgdi32 -limm32 -lvulkan-1
endif

# generate pre-requisites
%.d: %.c
	# remove existing pre-reqs files
	@set -e; rm -f $@; 
	# get list of includes from %.c put in tmp
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; 
	# extract just paths
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; 
	# remove tmp file
	rm -f $@.$$$$ 

%.d: %.cpp
	# remove existing pre-reqs files
	@set -e; rm -f $@; 
	# get list of includes from %.c put in tmp
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; 
	# extract just paths
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; 
	# remove tmp file
	rm -f $@.$$$$ 

# implicit recipes
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(target): $(objs)
	$(CXX) $(objs) $(LDFLAGS) -o $(target)

.PHONY: all clean run tree

all: $(target)

run: $(target)
	./$(target)

clean:
	@echo "cleaning..."
	rm -f $(objs) $(target)

tree:
	@echo "Source dirs:";  printf "  %s\n" $(src_dirs)
	#@echo "Module dirs:";  printf "  %s\n" $(mod_dirs)
	@echo "Sources:";      printf "  %s\n" $(c_srcs) $(cpp_srcs)
	@echo "Objects:";      printf "  %s\n" $(objs)

