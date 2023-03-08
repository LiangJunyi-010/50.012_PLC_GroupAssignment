#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_to_json.h"

int num_columns = 0;
int num_lines = 0;
char* csv_row_to_json(CsvRow *row) {
    char *json = (char*) malloc(MAX_SIZE * sizeof(char));
    sprintf(json, "\t{\n");
    for (int i = 0; i < num_columns; i++) {
        sprintf(json + strlen(json), "\t\t\"%s\" : \"%s\"", row->csv_data[i*2], row->csv_data[i*2+1]);
        if (i < num_columns - 1) {
            sprintf(json + strlen(json), ",");
        }
        sprintf(json + strlen(json), "\n");
    }
    sprintf(json + strlen(json), "\t}");
    return json;
}

int main(int argc, char *argv[]) {
    /* Check if file name was passed as argument*/
    if (argc != 3) {
        printf("Usage: csv_to_json input_file.csv output_file.json\n");
        return 0;
    }

    /* Open CSV file*/
    FILE *csv_file = fopen(argv[1], "r");
    if (csv_file == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 0;
    }

    /* Count number of lines and columns in the file*/
    char c;
    while (!feof(csv_file)) {
        c = fgetc(csv_file);
        if (c == '\n') {
            num_lines++;
            if (num_lines == 1) {
                char *line = (char*) malloc(MAX_COL_SIZE * sizeof(char));
                fgets(line, MAX_SIZE, csv_file);
                char *result_line;
                result_line = strtok(line, ",");
                while (result_line != NULL) {
                    num_columns++;
                    result_line = strtok(NULL, ",");
                }
                free(line);
            }
        }
    }
    rewind(csv_file);

    /* Allocate memory for CSV rows*/
    CsvRow **rows = (CsvRow**) malloc(num_lines * sizeof(CsvRow*));
    for (int i = 0; i < num_lines; i++) {
        rows[i] = (CsvRow*) malloc(sizeof(CsvRow));
        rows[i]->csv_data = (char**) malloc(num_columns * 2 * sizeof(char*));
        for (int j = 0; j < num_columns; j++) {
            rows[i]->csv_data[j*2] = (char*) malloc(MAX_SIZE * sizeof(char));
            rows[i]->csv_data[j*2+1] = (char*) malloc(MAX_SIZE * sizeof(char));
        }
    }

    /* Parse CSV file and store data in CsvRows */
    int row_index = 0;
    char **col_names = (char**) malloc(MAX_SIZE * sizeof(char));
    char *line = (char*) malloc(MAX_SIZE * sizeof(char));
    while (fgets(line, MAX_SIZE, csv_file) != NULL) {
        if(row_index==0){
            char *result_line;
            int col_index = 0;
            result_line = strtok(line, ",");
            while (col_index<num_columns) {
                strcpy(col_names[col_index],result_line);
            col_index++;
            }
        }
        else{
            char *result_line;
            int col_index = 0;
            result_line = strtok(line, ",");
            while (col_index<num_columns) {
                strcpy(rows[row_index]->csv_data[col_index*2],(char*)col_names[col_index]);
                strcpy(rows[row_index]->csv_data[col_index*2+1], result_line);
                col_index++;
            }
            row_index++;
        }  
    }
    fclose(csv_file);

    /*Write into Json File*/
    FILE *json_file = fopen(argv[2], "w");
    fprintf(json_file, "[\n");
    /*total nbum_lines -1*/
    for (int i = 0; i < num_lines-1; i++) {
        fprintf(json_file, "%s", csv_row_to_json(rows[i]));
        fprintf(json_file, ",");
    }
    fprintf(json_file, "]");
    fclose(json_file);

    /*Free memory*/
    free(col_names);
    free(line);
    for (int i = 0; i < num_lines; i++) {
        for (int j = 0; j < num_columns; j++) {
            free(rows[i]->csv_data[j*2]);
            free(rows[i]->csv_data[j*2+1]);
        }
        free(rows[i]->csv_data);
        free(rows[i]);
    }
    free(rows);

    return 0;
}
