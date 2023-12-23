#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_parser.h"

CSVRow parse_csv_row(const char *line) {
    CSVRow row;
    row.fields = NULL;
    row.num_fields = 0;

    char *token = strtok((char *)line, ",");
    while (token != NULL) {
        // Allocate memory for a new field
        row.fields = realloc(row.fields, (row.num_fields + 1) * sizeof(char *));
        row.fields[row.num_fields] = malloc(MAX_FIELD_SIZE);

        // Copy the token to the field, ensuring it's null-terminated
        strncpy(row.fields[row.num_fields], token, MAX_FIELD_SIZE - 1);
        row.fields[row.num_fields][MAX_FIELD_SIZE - 1] = '\0';

        row.num_fields++;
        token = strtok(NULL, ",");
    }

    return row;
}

void free_csv_row(CSVRow *row) {
    for (size_t i = 0; i < row->num_fields; i++) {
        // Free memory for individual fields in the row
        free(row->fields[i]);
    }
    // Free memory for the array of fields in the row
    free(row->fields);
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
}
