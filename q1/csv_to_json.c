#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_to_json.h"

int num_columns = 0;
int num_lines = 0;
void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\r' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}
char* csv_row_to_json(CsvRow *row) {
    char *json = (char*) malloc(MAX_SIZE * sizeof(char));
    int i=0;
    sprintf(json, "\t{\n");
    for ( i = 0; i < num_columns; i++) {
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
    FILE *csv_file = fopen(argv[1], "r");
    FILE *json_file = fopen(argv[2], "w");
    /*CSV rows*/
    int i=0;
    int j =0;
    /* Used for check \n to determin number of lines*/
    char c;
    int row_index = 0;
    int col_index = 0;
    char **col_names = (char**) malloc(MAX_SIZE * sizeof(char*));
    char *line = (char*) malloc(MAX_SIZE * sizeof(char));
    CsvRow** rows;
    /* Check if file name was passed as argument*/
    if (argc != 3) {
        printf("Input Format: csv_to_json.exe input_file.csv output_file.json\n");
        return 0;
    }

    /* Open CSV file*/
    if (csv_file == NULL) {
        printf("Wrong input file name\n");
        return 0;
    }

    /* Count number of lines and columns*/
    while (!feof(csv_file)) {
        c = fgetc(csv_file);
        if (c == '\n') {
            num_lines++;
            if (num_lines == 1) {
                char *line = (char*) malloc(MAX_SIZE * sizeof(char));
                char *result_line;
                fgets(line, MAX_SIZE, csv_file);
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
    rows = (CsvRow**) malloc(num_lines * sizeof(CsvRow*));
    /* Create CSV rows*/
    for (i = 0; i < num_lines; i++) {
        rows[i] = (CsvRow*) malloc(sizeof(CsvRow));
        rows[i]->csv_data = (char**) malloc(num_columns * 2 * sizeof(char*));
        for (j = 0; j < num_columns; j++) {
            rows[i]->csv_data[j*2] = (char*) malloc(MAX_SIZE * sizeof(char));
            rows[i]->csv_data[j*2+1] = (char*) malloc(MAX_SIZE * sizeof(char));
        }
    }

    /* Store data in Rows */
   
    while (fgets(line, MAX_SIZE, csv_file) != NULL) {
        /* if the first line, record the names of columns*/
        if(row_index==0){
            char *result_line;
            col_index = 0;
            while (col_index<num_columns) {
                if(col_index == 0){
                    result_line = strtok(line, ",");
                }
                else{
                    result_line = strtok(NULL, ",");
                }
                /* Get rid of \n, use LF instead of CRLF*/
                strip(result_line);
                /*if(col_index==num_columns-1){
                    result_line[strlen(result_line) - 1] ='\0';
                    printf("%s\n",result_line);
                }*/
                col_names[col_index] = malloc(MAX_SIZE * sizeof(char));
                strcpy(col_names[col_index],result_line);
                col_index++;
            }
        }
        else{
            char *result_line;
            col_index = 0;
            while (col_index<num_columns) {
                if(col_index == 0){
                    result_line = strtok(line, ",");
                }
                else{
                    result_line = strtok(NULL, ",");
                }
                /* Get rid of \n use LF instead of CRLF*/
                strip(result_line);
                /*if(col_index==num_columns-1){
                    result_line[strlen(result_line) - 1] = '\0';
                    printf("%s\n",result_line);
                }*/
                strcpy(rows[row_index-1]->csv_data[col_index*2],col_names[col_index]);
                strcpy(rows[row_index-1]->csv_data[col_index*2+1], result_line);
                col_index++;
            }
        }  
        row_index++;
    }
    fclose(csv_file);

    /*Write into Json File*/
    fprintf(json_file, "[\n");
    /*Total nbum_lines -1*/
    for (i = 0; i < num_lines; i++) {
        fprintf(json_file, "%s", csv_row_to_json(rows[i]));
        if(i<num_lines-1){
            fprintf(json_file, ",\n");
        }        
    }
    fprintf(json_file, "\n]");
    fclose(json_file);

    /*Free memory*/
    free(col_names);
    free(line);
    for (i = 0; i < num_lines; i++) {
        for (j = 0; j < num_columns; j++) {
            free(rows[i]->csv_data[j*2]);
            free(rows[i]->csv_data[j*2+1]);
        }
        free(rows[i]->csv_data);
        free(rows[i]);
    }
    free(rows);
    return 0;
}
