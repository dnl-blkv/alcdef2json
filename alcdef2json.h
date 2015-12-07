#ifndef ALCDEF2JSON_H_
#define ALCDEF2JSON_H_

// Include global headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

// Include local headers
#include "alcdef.h"

// Define alcdef line length and file name length constraints
#define LINE_LENGTH	ALCDEF_LINE_LENGTH
#define PATH_LENGTH	4096

// Declare the endpoint structure
// For source and destinations points
typedef struct endpoint {
	char path[PATH_LENGTH];
	bool isFile;
} endpoint;

// Enum describing field types for proper output
typedef enum {
	kBooleanField,
	kStringField,
	kNumberField
} FieldType;

// Output a single ALCDEF field
int output_field (FILE*, alcdef_field, FieldType);

void output_data (FILE*, alcdef_field, char*);

void output_flat_data (FILE*, alcdef_field, char*, int);

bool alcdef2json (const char*, FILE*, bool);

bool convert_all (const char*, const char*, bool);

#endif