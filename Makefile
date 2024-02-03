CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC_DIR = src
LIBRARY_SOURCES = $(SRC_DIR)/csv_parser.c
LIBRARY_HEADERS = $(SRC_DIR)/csv_parser.h
LIBRARY_OBJECTS = csv_parser.o

EXAMPLES_DIR = examples
MAIN_PROGRAM = $(EXAMPLES_DIR)/parse_csv_file.c
EXECUTABLE = $(EXAMPLES_DIR)/parse_csv_file

TEST_DIR = test
TEST_SOURCES = $(TEST_DIR)/test_csv_parser.c
UNITY_DIR = $(TEST_DIR)/unity
UNITY_SOURCES = $(UNITY_DIR)/unity.c
UNITY_INTERNALS_HEADER = $(UNITY_DIR)/unity_internals.h

# Dummy file to force recompilation of tests
TEST_MARKER = $(TEST_DIR)/test_marker

all: $(EXECUTABLE)

$(EXECUTABLE): $(LIBRARY_OBJECTS) $(MAIN_PROGRAM)
	$(CC) $(CFLAGS) -o $@ $(MAIN_PROGRAM) $(LIBRARY_OBJECTS)

$(LIBRARY_OBJECTS): $(LIBRARY_SOURCES) $(LIBRARY_HEADERS)
	$(CC) $(CFLAGS) -c $(LIBRARY_SOURCES)

# Test target
test: $(TEST_MARKER)

$(TEST_MARKER): $(TEST_SOURCES) $(LIBRARY_SOURCES) $(LIBRARY_HEADERS) $(UNITY_SOURCES) $(UNITY_INTERNALS_HEADER)
	$(CC) $(CFLAGS) -o test_runner $(TEST_SOURCES) $(LIBRARY_SOURCES) $(UNITY_SOURCES)
	./test_runner
	touch $(TEST_MARKER)
	rm -f test_runner

clean:
	rm -f $(EXECUTABLE) $(LIBRARY_OBJECTS) $(TEST_MARKER)
	
