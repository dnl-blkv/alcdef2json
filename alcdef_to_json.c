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
#include "alcdef_to_json.h"

// Converts all string characters to lower case
char *stolower (char *line) 
{
	char *p = line;
	for (; *p; ++p) *p = tolower(*p);
	
	return line;
}

// Escapes a string for JSON
int JsonEscapeString (char *str) 
{
	char buffer[MAX_LINE_LENGTH];
	int i = 0, n = 0;

	for (i = 0; i < strlen(str); ++i) {
		if (str[i] == '"') {
			sprintf(buffer + i + n, "\\%c", str[i]);
			n ++;
		} else if (str[i] == ',') {
			// TODO: Temporal solution for dots, an alternative should be found
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
	// Create the json key out of the alcdef field name
	char json_key[MAX_LINE_LENGTH];
	strcpy(json_key, field->name);
	stolower(json_key);
	
	// Create the json-compatible value out of the alcdef field value
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

	// Terminate according to the mode
	if (data_number < 0) {
		fprintf(output, "}");
	} else {
		return (data_number + 1);
	}
	
	// Return the resulting data number (remains -1 for nested mode)
	return data_number;
}

// Converts a single ALCDEF file to JSON
bool AlcdefToJson (const char *input_file_path, const bool flat_mode, FILE *output) {
	
	// Open the input file
	FILE *input = fopen(input_file_path, "r");
	
	// If the next file was failed to open, report error
	if (!input) {
		printf("INPUT FILE ERROR! Path: %s\n", input_file_path);
		// Wait for any input key
		getchar();
		return true;
	}

	// The data fields are counted starting with 1, similarly
	// to the {X}-values of ALCDEF
	int previous_field_code = kWrongField;
	int data_number = flat_mode ? 1 : -1;
	
	// Create the field buffer
	AlcdefField field;
	field.code = kWrongField;

	// Define the alcdef_line buffer and delimiter variable
	char  alcdef_line[MAX_LINE_LENGTH], 
		 *delimiter;

	// Variable to store the last index in the alcdef_line
	size_t line_length;

	// Read the input file alcdef_line by alcdef_line to the alcdef_line buffer
	while (fgets(alcdef_line, MAX_LINE_LENGTH, input) != NULL) {
		
		// Save the last alcdef_line index
		line_length = strlen(alcdef_line);
		
		// If the alcdef_line ends with newline, replace with \0
		if (alcdef_line[line_length - 1] == '\n') {
			alcdef_line[line_length - 1] = '\0';
		}
		
		// Save the old field code
		previous_field_code = field.code;
		
		// Get the field code
		RepopulateField(alcdef_line, &field);

		// If the field had some data, output comma to separate key-value pairs
		// in the resulting JSON file
		if (field_has_printable_value(field.code) && field_has_value(previous_field_code)) {
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
			// Double fields
			case kCiTarget:
			case kLtcDays:
			case kMagAdjust:
			case kObsLatitude:
			case kObsLongitude:
			case kPabB:
			case kPabL:
			case kPhase:
			case kUCorMag:
			// Double {X} - fields
			case kCompCi:
			case kCompMag:
			// Integer fields
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
					// Reset data number to the initial value
					data_number = 1;
				}
				
				fprintf(output, "}");
				
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