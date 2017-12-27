CC = gcc
OUTPUT = test
SOURCE = ./src/gc_log.c ./src/gc_test.c ./src/gc_malloc.c
OBJECTS = $(patsubst %.c, %.o, $(SOURCE))
INCLUDES = ./includes/gc_malloc.h
INCLUDE_DIR = ./includes/
FLAGS = -g

$(OUTPUT) : $(OBJECTS) $(INCLUDES)
	$(CC) $(FLAGS) -o $(OUTPUT) $(OBJECTS) -I $(INCLUDE_DIR)

%.o : %.c
	$(CC) $(FLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

clean :
	rm -f ./src/*.o $(OUTPUT)
