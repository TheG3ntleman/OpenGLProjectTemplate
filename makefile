# Project structure
SOURCE_FILES = src/engine/glad/src/glad.c \
							 src/main.c \
							 src/engine/shaders.c

HEADER_FILES = src/engine/window.h \
							 src/engine/shaders.h \
							 src/engine/engine_config.h

OBJECT_FILES = src/engine/glad/src/glad.o \
							 src/main.o \
							 src/engine/shaders.o

EXE_NAME = template 

# Compiler settings/config
CC = gcc
CFLAGS = -Wall -Wextra
LFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OPTI_LVL = -O3 # GCC Specific

# Recipe

all: build
	./$(EXE_NAME)

build: $(EXE_NAME)

$(EXE_NAME): $(OBJECT_FILES)
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c $(HEADER_FILES)
	$(CC) -c $< $(OPTI_LVL) $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm $(EXE_NAME) $(OBJECT_FILES)
