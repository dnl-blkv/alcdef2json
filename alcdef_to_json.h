// The MIT License (MIT)
// 
// Copyright (c) 2015 Daniil Belyakov
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef ALCDEF2JSON_H_
#define ALCDEF2JSON_H_

#include <stdbool.h>

#include "alcdef.h"

// The line length here is three times longer than ALCDEF line
// because escaping required for JSON could potentially increase
// its length three-fold
#define MAX_LINE_LENGTH  3 * MAX_ALCDEF_LINE_LENGTH
// Path length is maximal for ext4 file system which is the largest
// among all the often-used file systems
#define MAX_PATH_LENGTH  4096

// Enum describing field types for proper output
typedef enum {
  kBooleanField,
  kStringField,
  kNumberField
} FieldType;

// Converts a string to its equivalent in lower case
char *stolower (char*);

// Escapes a string to make it compatible with JSON and MongoDB(!)
int JsonEscapeString (char*);

// Output a single ALCDEF field
int PrintMetadataField (FILE*, const AlcdefField*, const FieldType);

// Output a single ALCDEF lightcurve data field
int PrintDataField (FILE*, const AlcdefField*, const char*, const int);

// Prints a single ALCDEF file to a given JSON file
bool AlcdefToJson (FILE*, FILE*, const bool);

#endif