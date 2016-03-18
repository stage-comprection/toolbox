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

TARGET = compare_reads_files errors_analyses format_reads_file get_reads_size sort_reads

# Objects names
OBJECTS =    $(BUILD)/utils.o $(BUILD)/read.o

# Rules

all: init $(TARGET)

compare_reads_files: $(BUILD)/compare_reads_files.o $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN)/$@ $^ 


errors_analyses: $(BUILD)/errors_analyses.o $(OBJECTS)
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