#ifndef CSV_TO_JSON_HEADER
#define CSV_TO_JSON_HEADER

#define MAX_SIZE 4096
#define MAX_COL_SIZE 11
typedef struct {
    char **csv_data;
} CsvRow;

char* csv_to_json(CsvRow *row);

#endif
