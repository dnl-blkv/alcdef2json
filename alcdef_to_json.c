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
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "alcdef.h"
#include "alcdef_to_json.h"

// Converts all string characters to lower case
char *stolower (char *line) 
{
  char *p = line;
  for (; *p; ++p) *p = tolower(*p);
  
  return line;
}

// Escapes a string for usage with JSON and MongoDB(!). Basically, escapes
// double quotes and replaces commas with dots. The latter was done solely
// because of the original purpose of the program, which was to ease import 
// of ALCDEF data to MongoDB.
int JsonEscapeString (char *str) 
{
  char buffer[MAX_LINE_LENGTH];
  int i = 0, n = 0;

  for (i = 0; i < strlen(str); ++i) {
    if (str[i] == '"') {
      sprintf(buffer + i + n, "\\%c", str[i]);
      ++n;
    } else if (str[i] == ',') {
      // TODO(dnl-blkv@gmail.com): get rid of comma replacement; possibly,
      // change it to some kind of comma escaping.
      str[i] = '.';
    } else {
      sprintf(buffer + i + n, "%c", str[i]);
    }
  }
  
  if (n > 0) {
    memset(str, 0, strlen(str));
    strcpy(str, buffer);
  }
  
  return 0;
}

// Output a single ALCDEF field
int PrintMetadataField (FILE *output, const AlcdefField *field, const FieldType field_type) {
  // Create a json key out of the alcdef field name
  char json_key[MAX_LINE_LENGTH];
  strcpy(json_key, field->name);
  stolower(json_key);
  
  // Create a json value out of the alcdef field value
  char json_value[MAX_LINE_LENGTH];
  strcpy(json_value, field->value);
  JsonEscapeString(json_value);
  
  switch(field_type) {
    case kBooleanField: {
      return fprintf(output, "\"%s\":%s", json_key, stolower(json_value));
      break;
    }
    case kStringField: {
      return fprintf(output, "\"%s\":\"%s\"", json_key, json_value);
      break;
    }
    case kNumberField: {
      if (strlen(json_value) == 0) {
        strcpy(json_value, "null");
      }
      return fprintf(output, "\"%s\":%s", json_key, json_value);
      break;
    }
    default: {
      return -1;
      break;
    }
  }
}

// Outputs a single ALCDEF lightcurve data block
// Negative data number indicates the nested mode
// The first argument is output due to the nature of fprintf function on which this function relies
int PrintDataField (FILE *output, const AlcdefField *field, const char *delimiter, const int data_number) {
  // Output the opening curly brace for the flat mode
  if (data_number < 0) {
    fprintf(output, "{");
  }
  
  // Save the subfields names and tools for iteration over them
  int subfield_number = 0;
  char *subfield_names[4] = {"jd", "mag", "magerr", "airmass"};
  char *subfield_name = subfield_names[subfield_number];
  
  // Get the pointer to the first subfield value
  char subfield_values[MAX_LINE_LENGTH];
  strcpy(subfield_values, field->value);
  char *subfield_value = strtok(subfield_values, delimiter);
  
  // Output the subfields
  while (subfield_value) {
    // Add comma to separate the subfields
    if (subfield_number > 0) fprintf(output, ",");
    
    // Data number below zero indicates the flat mode
    if (data_number < 0) {
      fprintf(output, "\"%s\":%s", subfield_name, subfield_value);
    } else {
      fprintf(output, "\"%s%d\":%s", subfield_name, data_number, subfield_value);
    }
    
    // Go to the next subfield
    ++ subfield_number;
    subfield_name = subfield_names[subfield_number];
    subfield_value = strtok(NULL, delimiter);
  }

  if (data_number < 0) {
    fprintf(output, "}");
  } else {
    // For flat mode, increase the number of data field
    return (data_number + 1);
  }
  
  // Return the resulting data number (remains -1 for nested mode)
  return data_number;
}

// Writes a portion of an ALCDEF file from the input pointer till the file end
// into a JSON file pointed by the output pointer
bool AlcdefToJson (FILE *output, FILE *input, const bool flat_mode) {
  // The data fields are counted starting with 1, similarly
  // to the {X}-values of ALCDEF
  int previous_field_code = kWrongField;
  // The number of data line in the current lightcurve block
  int data_number = flat_mode ? 1 : -1;
  
  // Buffer to store the ALCDEF lines
  char alcdef_line[MAX_LINE_LENGTH];
  
  // Buffer to store the fields data
  AlcdefField field;
  field.code = kWrongField;
  
  // Pointer to the ALCDEF data delimiter (either \t or | in ALCDEF 2.0)
  char *delimiter = "\t";

  size_t line_length;

  // Scan the input file line by line
  while (fgets(alcdef_line, MAX_LINE_LENGTH, input) != NULL) {
    line_length = strlen(alcdef_line);
    // If a current ALCDEF line ends with a newline character, replace with \0
    // to mark the end of the line
    if (alcdef_line[line_length - 1] == '\n') {
      alcdef_line[line_length - 1] = '\0';
    }
    
    // Save the old field code for further reference
    previous_field_code = field.code;
    
    // Fetch the field from the new line into the buffer
    RepopulateField(alcdef_line, &field);

    // If the field had some data, output comma to separate key-value pairs
    // in the resulting JSON file
    if (field_has_significant_value(field.code) && field_has_value(previous_field_code)) {
      fprintf(output, ",");
    }
    
    // If the field teminates a lightcurve block, add comma and a newline to
    // separate objects representing the blocks in the resulting JSON file 
    if ((previous_field_code == kEndData) && (field.code == kStartMetadata)) {
      fprintf(output, ",\n");
    }
    
    // Process the field according to its code
    switch (field.code) {
      // String fields
      case kBibCode:
      case kCiBand:
      case kComment:
      case kContactInfo:
      case kContactName:
      case kFilter:
      case kLtcApp:
      case kLtcType:
      case kMagBand:
      case kMpcDesig:
      case kObjectDec:
      case kObjectName:
      case kObjectRa:
      case kObservers:
      case kPublication:
      case kReducedMags:
      case kSessionDate:
      case kSessionTime:
      case kStandard:
      // String {X} - fields
      case kCompDec:
      case kCompName:
      case kCompRa: {
        PrintMetadataField(output, &field, kStringField);
        break;
      }
      // Boolean fields
      case kCiCorrection:
      case kDifferMags:
      case kRevisedData: {
        PrintMetadataField(output, &field, kBooleanField);
        break;
      }
      // Number fields
      //  Double fields
      case kCiTarget:
      case kLtcDays:
      case kMagAdjust:
      case kObsLatitude:
      case kObsLongitude:
      case kPabB:
      case kPabL:
      case kPhase:
      case kUCorMag:
      //  Double {X} - fields
      case kCompCi:
      case kCompMag:
      //  Integer fields
      case kObjectNumber: {
        PrintMetadataField(output, &field, kNumberField);
        break;
      }
      // Data fields
      case kData: {
        data_number = PrintDataField(output, &field, delimiter, data_number);
        break;
      }
      case kDelimiter: {
        // Save the delimiter
        if (strstr(field.value, "TAB")) {
          delimiter = "\t";
        } else {
          delimiter = "|";
        }
        break;
      }
      // Extreme fields
      case kStartMetadata: {
        // At this point a new lightcurve block begins
        fprintf(output, "{");
        
        if (!flat_mode) {
          fprintf(output, "\"metadata\":{");
        }
        break;
      }
      case kEndMetadata: {
        if (!flat_mode) {
          fprintf(output, "},\"data\":[");
        } else {
          fprintf(output, ",");
        }
        
        break;
      }
      case kEndData: {
        if (!flat_mode) {
          fprintf(output, "]");
        } else {
          // Reset the data number
          data_number = 1;
        }
        
        fprintf(output, "}");
        // At this point a lightcurve block ends
        break;
      }
      default: {
        break;
      }
    }
  }
  
  // Close the input file
  fclose(input);
  input = NULL;
  
  return false;
}
