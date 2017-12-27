CC = gcc
OUTPUT = test
OBJECTS = ./src/gc_test.o ./src/gc_log.o ./src/gc_malloc.o
OBJECTS_DIR = ./src/
SOURCE = ./src/gc_test.c ./src/gc_log.c ./src/gc_malloc.c
INCLUDES = ./includes/gc_malloc.h
INCLUDE_DIR = ./includes/

$(OUTPUT) : $(OBJECTS) $(INCLUDES)
	$(CC) -o $(OUTPUT) $(OBJECTS) -I $(INCLUDE_DIR)

$(OBJECTS) : $(SOURCE) $(INCLUDES)
	$(CC) -c $< -o $@

clean :
	rm -f ./src/*.o $(OUTPUT)
