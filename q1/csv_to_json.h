/*
 * csv_to_json.c - a program to convert a CSV file to a JSON file.
 *
 * Usage: csv_to_json.exe input_file.csv output_file.json
 *
 * Author: group 12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_to_json.h"

/* Constants */
#define MAX_SIZE 1024

/* Global variables */
int num_columns = 0;
int num_lines = 0;

/* Function prototypes */
void strip(char *s);
char* csv_row_to_json(CsvRow *row);

/* Main function */
int main(int argc, char *argv[]) {
    ...
}

/* Other functions */
void strip(char *s) {
    ...
}

char* csv_row_to_json(CsvRow *row) {
    ...
}