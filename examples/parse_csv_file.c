#include "../src/csv_parser.h"
#include <stdio.h>

int main() {
    const char *filename = "example.csv";
    CSVTable table = parse_csv_file(filename);

    // Example: Print the parsed CSV content
    for (size_t i = 0; i < table.num_rows; i++) {
        for (size_t j = 0; j < table.rows[i].num_fields; j++) {
            printf("%s\t", table.rows[i].fields[j]);
        }
        printf("\n");
    }

    // Clean up allocated memory
    free_csv_table(&table);

    return 0;
}
