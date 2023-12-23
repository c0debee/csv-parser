# CSV Parser
 A minimalist C library for parsing CSV files with ease and efficiency.

*Overview:*

The CSV Parser Library provides functions to parse CSV files and store the data in a structured format. It consists of two main structures: `CSVRow` representing a single row in the CSV file, and `CSVTable` representing the entire CSV table.

*CSVRow Structure:*

```c
typedef struct {
    char **fields;     // Array of strings representing individual fields in the row
    size_t num_fields;  // Number of fields in the row
} CSVRow;
```

*CSVTable Structure:*

```c
typedef struct {
    CSVRow *rows;       // Array of CSVRows representing individual rows in the table
    size_t num_rows;    // Number of rows in the table
} CSVTable;
```

*Functions:*

1. **`CSVTable parse_csv_file(const char *filename)`**

   *Description:*
   Parses an entire CSV file and returns a `CSVTable` structure containing the parsed data.

   *Parameters:*
   - `filename`: The name of the CSV file to be parsed.

   *Returns:*
   A `CSVTable` structure containing the parsed CSV data.

2. **`void free_csv_table(CSVTable *table)`**

   *Description:*
   Frees the memory allocated for a `CSVTable` structure, including individual rows and fields.

   *Parameters:*
   - `table`: A pointer to the `CSVTable` structure to be freed.

3. **`CSVRow parse_csv_row(const char *line)`**

   *Description:*
   Parses a single CSV row from a string.

   *Parameters:*
   - `line`: A string representing a single row in the CSV file.

   *Returns:*
   A `CSVRow` structure containing the parsed row data.

4. **`void free_csv_row(CSVRow *row)`**

   *Description:*
   Frees the memory allocated for a single CSV row.

   *Parameters:*
   - `row`: A pointer to the `CSVRow` structure to be freed.


*Usage Example:*

```c
#include "csv_parser.h"
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
```

*Note:*
- Ensure that the CSV file follows the standard CSV format with fields separated by commas.
- The library handles memory allocation for rows and fields, but it's important to free the memory using `free_csv_table` or `free_csv_row` to avoid memory leaks.
