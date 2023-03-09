/* csv_to_json.h*/

#ifndef CSV_TO_JSON_H
#define CSV_TO_JSON_H

#define MAX_SIZE 4096
typedef struct {
    char **csv_data;
}CsvRow ;

void strip(char *s);
char* csv_row_to_json(CsvRow *row);
#endif