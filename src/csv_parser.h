#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <stddef.h>

#define MAX_FIELD_SIZE 100

// Structure representing a single row in the CSV file
typedef struct {
    char **fields;     // Array of strings representing individual fields in the row
    size_t num_fields;  // Number of fields in the row
} CSVRow;

// Structure representing the entire CSV table
typedef struct {
    CSVRow *rows;       // Array of CSVRows representing individual rows in the table
    size_t num_rows;    // Number of rows in the table
} CSVTable;

// Parse a single CSV row from a string
CSVRow parse_csv_row(const char *line);

// Free the memory allocated for a single CSV row
void free_csv_row(CSVRow *row);

// Parse an entire CSV file and return a CSVTable
CSVTable parse_csv_file(const char *filename);

// Free the memory allocated for a CSVTable
void free_csv_table(CSVTable *table);

#endif /* CSV_PARSER_H */
