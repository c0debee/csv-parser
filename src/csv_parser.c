#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_parser.h"

CSVRow parse_csv_row(const char* line) {
    CSVRow row;
    row.fields = NULL;
    row.num_fields = 0;

    // Duplicate the line to avoid modifying the original string
    char* line_copy = malloc(strlen(line)+1);
    strcpy(line_copy,line);

    char* token = strtok((char*)line_copy, ",");
    while (token != NULL) {
        
        // Remove \n and \r characters from the token
        char *newline_char = strchr(token, '\n');
        if (newline_char != NULL) {
            *newline_char = '\0';
        }

        char *carriage_return_char = strchr(token, '\r');
        if (carriage_return_char != NULL) {
            *carriage_return_char = '\0';
        }

        // Allocate memory for a new field
        row.fields = (char**)realloc(row.fields, (row.num_fields + 1) * sizeof(char*));
        row.fields[row.num_fields] = (char*)malloc(MAX_FIELD_SIZE);

        // Copy the token to the field, ensuring it's null-terminated
        strncpy(row.fields[row.num_fields], token, MAX_FIELD_SIZE - 1);
        row.fields[row.num_fields][MAX_FIELD_SIZE - 1] = '\0';

        row.num_fields++;
        token = strtok(NULL, ",");
    }

    // Free the duplicated line
    free(line_copy);

    return row;
}

void free_csv_row(CSVRow *row) {
    for (size_t i = 0; i < row->num_fields; i++) {
        // Free memory for individual fields in the row
        free(row->fields[i]);
    }
    // Free memory for the array of fields in the row
    free(row->fields);
    row->fields = NULL;
    row->num_fields = 0;
}

CSVTable parse_csv_file(const char *filename) {
    CSVTable table;
    table.rows = NULL;
    table.num_rows = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_FIELD_SIZE * 10]; // Assuming a reasonable maximum line size

    while (fgets(line, sizeof(line), file) != NULL) {
        // Allocate memory for a new row
        table.rows = realloc(table.rows, (table.num_rows + 1) * sizeof(CSVRow));
        table.rows[table.num_rows] = parse_csv_row(line);
        table.num_rows++;
    }

    fclose(file);

    return table;
}

void free_csv_table(CSVTable *table) {
    for (size_t i = 0; i < table->num_rows; i++) {
        // Free memory for individual rows in the table
        free_csv_row(&table->rows[i]);
    }
    // Free memory for the array of rows in the table
    free(table->rows);
    table->rows = NULL;
    table->num_rows = 0;
}
