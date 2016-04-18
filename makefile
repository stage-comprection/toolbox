# Compiler options
CC = g++
OPTCFLAGS= -Ofast -march=native 
CFLAGS= -Wall -Werror -std=c++11 -I $(LIBPATH) $(OPTCFLAGS)
LDFLAGS= -pthread -static-libstdc++

# Directory organisation
BASEDIR = .
SRC = $(BASEDIR)/src
BUILD = $(BASEDIR)/build
BIN = $(BASEDIR)/bin

TARGET = access_reads compare_reads_files compare_subset count_insertions get_bad_reads format_reads_file get_reads_size sort_reads 

# Objects names
OBJECTS =    $(BUILD)/utils.o $(BUILD)/read.o

# Rules

all: init $(TARGET)

access_reads: $(BUILD)/access_reads.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


compare_reads_files: $(BUILD)/compare_reads_files.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


compare_subset: $(BUILD)/compare_subset.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


count_insertions: $(BUILD)/count_insertions.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


get_bad_reads: $(BUILD)/get_bad_reads.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


format_reads_file: $(BUILD)/format_reads_file.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


get_reads_size: $(BUILD)/get_reads_size.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


sort_reads: $(BUILD)/sort_reads.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BUILD)/*.o
	rm -rf $(BIN)/$(TARGET)

init: 
	mkdir -p $(BUILD) $(BIN)

rebuild: clean $(TARGET)
