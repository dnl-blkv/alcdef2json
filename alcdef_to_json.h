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

// The line length here is three times longer than ALCDEF line
// because escaping required for JSON could potentially increase
// its length three-fold
#define MAX_LINE_LENGTH  3 * MAX_ALCDEF_LINE_LENGTH
// Path length is maximal for ext4 file system which is the largest
// among all the often-used file systems
#define MAX_PATH_LENGTH  4096

// Declare the endpoint structure
// For source and destinations points
typedef struct Endpoint {
  char path[MAX_PATH_LENGTH];
  bool isFile;
} Endpoint;

// Enum describing field types for proper output
typedef enum {
  kBooleanField,
  kStringField,
  kNumberField
} FieldType;

// Converts a string to its equivalent in lower case
char *stolower (char*);

// Escapes a string for JSON
int JsonEscapeString (char*);

// Output a single ALCDEF field
int PrintMetadataField (FILE*, const AlcdefField*, const FieldType);

// Output a single ALCDEF lightcurve data block in a nested manner
int PrintDataField (FILE*, const AlcdefField*, const char*, const int);

// Converts a single ALCDEF file to JSON
bool AlcdefToJson (const char*, const bool, FILE*);

#endif