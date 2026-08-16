#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "csv_parser.h"

#define RECORD_READ_BUFFER_SIZE 1024

// Read until the physical line is complete.
// This keeps a long record from being parsed as multiple rows.
static int read_csv_line(FILE *file, char **line, size_t *capacity) {
    char chunk[RECORD_READ_BUFFER_SIZE];
    size_t line_length = 0;

    while (fgets(chunk, sizeof(chunk), file) != NULL) {
        size_t chunk_length = strlen(chunk);

        // Make sure the required buffer size cannot wrap around.
        if (chunk_length > SIZE_MAX - line_length - 1) {
            return -1;
        }

        size_t required_capacity = line_length + chunk_length + 1;
        if (required_capacity > *capacity) {
            size_t new_capacity = *capacity == 0 ? sizeof(chunk) : *capacity;

            while (new_capacity < required_capacity) {
                if (new_capacity > SIZE_MAX / 2) {
                    new_capacity = required_capacity;
                    break;
                }
                new_capacity *= 2;
            }

            // Keep the old buffer valid if realloc fails.
            char *resized_line = realloc(*line, new_capacity);
            if (resized_line == NULL) {
                return -1;
            }

            *line = resized_line;
            *capacity = new_capacity;
        }

        // Append over the previous terminator and copy the new terminator as well.
        memcpy(*line + line_length, chunk, chunk_length + 1);
        line_length += chunk_length;

        if (line_length > 0 && (*line)[line_length - 1] == '\n') {
            return 1;
        }
    }

    return line_length > 0 ? 1 : 0;
}

CSVRow parse_csv_row(const char* line) {
    CSVRow row;
    row.fields = NULL;
    row.num_fields = 0;

    size_t line_length = strcspn(line, "\r\n");
    size_t field_start = 0;

    for (size_t i = 0; i <= line_length; i++) {
        if (i != line_length && line[i] != ',') {
            continue;
        }

        size_t field_length = i - field_start;
        row.fields = (char**)realloc(row.fields, (row.num_fields + 1) * sizeof(char*));
        row.fields[row.num_fields] = (char*)malloc(field_length + 1);

        memcpy(row.fields[row.num_fields], line + field_start, field_length);
        row.fields[row.num_fields][field_length] = '\0';

        row.num_fields++;
        field_start = i + 1;
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

    char *line = NULL;
    size_t line_capacity = 0;
    int read_status;

    while ((read_status = read_csv_line(file, &line, &line_capacity)) > 0) {
        // Allocate memory for a new row
        table.rows = realloc(table.rows, (table.num_rows + 1) * sizeof(CSVRow));
        table.rows[table.num_rows] = parse_csv_row(line);
        table.num_rows++;
    }

    free(line);

    if (read_status < 0) {
        fclose(file);
        free_csv_table(&table);
        fprintf(stderr, "Error: CSV record is too large or memory allocation failed\n");
        exit(EXIT_FAILURE);
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
