#include <stdlib.h>
#include <string.h>
#include "../src/csv_parser.h"
#include "unity/unity_internals.h"
#include "unity/unity.h"

// Set up and tear down functions
void setUp(void) {
    // This function will be called before each test
}

void tearDown(void) {
    // This function will be called after each test
}

// Test for parse_csv_row function
void test_parse_csv_row(void) {
    const char *csv_line = "John,25,New York";
    CSVRow row = parse_csv_row(csv_line);

    // Assert statements to check the correctness of the parsed CSVRow
    TEST_ASSERT_EQUAL_INT(3, row.num_fields);
    TEST_ASSERT_EQUAL_STRING("John", row.fields[0]);
    TEST_ASSERT_EQUAL_STRING("25", row.fields[1]);
    TEST_ASSERT_EQUAL_STRING("New York", row.fields[2]);

    // Free the allocated memory for the CSVRow
    free_csv_row(&row);
}

// Test for parse_csv_file function
void test_parse_csv_file(void) {
    const char *filename = "examples/example.csv";
    CSVTable table = parse_csv_file(filename);

    // Assert statements to check the correctness of the parsed CSVTable
    TEST_ASSERT_EQUAL_INT(5, table.num_rows);
    TEST_ASSERT_EQUAL_STRING("Name", table.rows[0].fields[0]);
    TEST_ASSERT_EQUAL_STRING("25", table.rows[1].fields[1]);
    TEST_ASSERT_EQUAL_STRING("San Francisco", table.rows[2].fields[2]);

    // Free the allocated memory for the CSVTable
    free_csv_table(&table);
}

// Test for free_csv_row function
void test_free_csv_row(void) {
    // Create a CSVRow to be freed
    CSVRow row;
    row.fields = malloc(3 * sizeof(char *));
    for (size_t i = 0; i < 3; i++) {
        row.fields[i] = malloc(10); // Assuming a reasonable maximum field size
        strcpy(row.fields[i], "Test");
    }
    row.num_fields = 3;

    // Free the CSVRow
    free_csv_row(&row);

    // Assert statements to check if memory was successfully freed
    TEST_ASSERT_NULL(row.fields);
    TEST_ASSERT_EQUAL_INT(0, row.num_fields);
}

// Test for free_csv_table function
void test_free_csv_table(void) {
    // Create a CSVTable to be freed
    CSVTable table;
    table.rows = malloc(2 * sizeof(CSVRow));
    for (size_t i = 0; i < 2; i++) {
        table.rows[i].fields = malloc(3 * sizeof(char *));
        for (size_t j = 0; j < 3; j++) {
            table.rows[i].fields[j] = malloc(10); // Assuming a reasonable maximum field size
            strcpy(table.rows[i].fields[j], "Test");
        }
        table.rows[i].num_fields = 3;
    }
    table.num_rows = 2;

    // Free the CSVTable
    free_csv_table(&table);

    // Assert statements to check if memory was successfully freed
    TEST_ASSERT_NULL(table.rows);
    TEST_ASSERT_EQUAL_INT(0, table.num_rows);
}

// Add more test functions for other functions in your library if needed

int main(void) {
    UNITY_BEGIN();

    // Run the tests
    RUN_TEST(test_parse_csv_row);
    RUN_TEST(test_parse_csv_file);
    RUN_TEST(test_free_csv_row);
    RUN_TEST(test_free_csv_table);

    // Add calls to other test functions if you have more

    return UNITY_END();
}
