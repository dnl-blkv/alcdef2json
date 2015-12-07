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
#include "alcdef2json.h"

// Output a single ALCDEF field
int output_field (FILE *output, alcdef_field field, FieldType fieldType) {
	switch(fieldType) {
		case kBooleanField: {
			return fprintf(output, "\"%s\":%s", field.name, stolower(field.value));
			break;
		}
		case kStringField: {
			return fprintf(output, "\"%s\":\"%s\"", field.name, field.value);
			break;
		}
		case kNumberField: {
			if (strlen(field.value) == 0) {
				strcpy(field.value, "null");
			}
			return fprintf(output, "\"%s\":%s", field.name, field.value);
			break;
		}
		default: {
			return -1;
			break;
		}
	}
}

void output_data (FILE *output, alcdef_field field, char *delimiter) {
	char tokens[LINE_LENGTH];
	strcpy(tokens, field.value);
	char *token = strtok(tokens, delimiter);
	
	// Output the Julian Date
	fprintf(output, "{\"jd\":%s", token);
	token = strtok(NULL, delimiter);

	// Output the magnitude
	fprintf(output, ",\"mag\":%s", token);
	token = strtok(NULL, delimiter);
	
	// Output the magnitude error, if present
	if (token) {
		fprintf(output, ",\"magerr\":%s", token);
		token = strtok(NULL, delimiter);
	}
	
	// Output the air mass, if present
	if (token) {
		fprintf(output, ",\"airmass\":%s", token);
	}
	
	fprintf(output, "}");
}

void output_flat_data (FILE *output, alcdef_field field, char *delimiter, int entry_number) {
	char tokens[LINE_LENGTH];
	strcpy(tokens, field.value);
	char *token = strtok(tokens, delimiter);
	
	// Output the Julian Date
	fprintf(output, "\"jd%d\":%s", entry_number, token);
	token = strtok(NULL, delimiter);

	// Output the magnitude
	fprintf(output, ",\"mag%d\":%s", entry_number, token);
	token = strtok(NULL, delimiter);
	
	// Output the magnitude error, if present
	if (token) {
		fprintf(output, ",\"magerr%d\":%s", entry_number, token);
		token = strtok(NULL, delimiter);
	}
	
	// Output the air mass, if present
	if (token) {
		fprintf(output, ",\"airmass%d\":%s", entry_number, token);
	}
}

// Convert a single ALCDEF file to JSON
bool alcdef2json (const char *input_file_path, FILE *output, bool nested_mode) {
	
	// Open the file
	FILE *input = fopen(input_file_path, "r");
	
	// If the next file was failed to open, report error
	if (!input) {
		printf("INPUT FILE ERROR! Path: %s\n", input_file_path);
		getchar();
		return true;
	}

	// Define the old field code and data fields counter
	int old_field_code	= WRONG_FIELD, 
		data_count		= 1;
	
	// Create the field buffer
	alcdef_field field;
	field.code = WRONG_FIELD;

	// Define the line buffer and delimiter variable
	char line[LINE_LENGTH], *delimiter;

	// Variable to store the last index in the line
	size_t last_line_index;

	// Read the input file line by line to the line buffer
	while (fgets(line, LINE_LENGTH, input) != NULL) {
		
		// Save the last line index
		last_line_index = strlen(line) - 1;
		
		// If the line ends with newline, replace with \0
		if (line[last_line_index] == '\n') {
			line[last_line_index] = '\0';
		}
		
		// Save the old field code
		old_field_code = field.code;
		
		// Get the field code
		repopulate_field(&field, line);

			if (field_has_printable_value(field.code) && field_has_value(old_field_code)) {
				fprintf(output, ",");
			}
			
			if ((old_field_code == ENDDATA) && 
				(field.code == STARTMETADATA)) {
					
				fprintf(output, ",\n");
			}
			
			// Process the field
			switch (field.code) {
				
				// String case
				case BIBCODE:
				case CIBAND:
				case COMMENT:
				case CONTACTINFO:
				case CONTACTNAME:
				case FILTER:
				case LTCAPP:
				case LTCTYPE:
				case MAGBAND:
				case MPCDESIG:
				case OBJECTDEC:
				case OBJECTNAME:
				case OBJECTRA:
				case OBSERVERS:
				case PUBLICATION:
				case REDUCEDMAGS:
				case SESSIONDATE:
				case SESSIONTIME:
				case STANDARD:
					output_field(output, field, kStringField);
					break;
					
				// Boolean case
				case CICORRECTION:
				case DIFFERMAGS:
				case REVISEDDATA:
					output_field(output, field, kBooleanField);
					break;
				
				// Number case
				//   > Double case
				case CITARGET:
				case LTCDAYS:
				case MAGADJUST:
				case OBSLATITUDE:
				case OBSLONGITUDE:
				case PABB:
				case PABL:
				case PHASE:
				case UCORMAG:
				//   > Integer case
				case OBJECTNUMBER:
					output_field(output, field, kNumberField);
					break;
					
				// START X - VALUES
				// Nested Case: Put to arrays, output on ENDMETADATA
				case COMPCI:
					output_field(output, field, kNumberField);
					break;
				case COMPDEC:
					output_field(output, field, kStringField);
					break;
				case COMPMAG:
					output_field(output, field, kNumberField);
					break;
				case COMPNAME:
					output_field(output, field, kStringField);
					break;
				case COMPRA:
					output_field(output, field, kStringField);
					break;

				// DATA VALUES
				case DATA:
					// Check the delimiter
					if (nested_mode) {
						output_data(output, field, delimiter);
					} else {
						output_flat_data(output, field, delimiter, data_count);
					}
					
					++ data_count;
					break;
				case DELIMITER:
					// Save the delimiter
					if (strstr(field.value, "TAB")) {
						delimiter = "\t";
					} else {
						delimiter = "|";
					}
					break;

				// EXTREME CHARACTERS
				case STARTMETADATA:
					fprintf(output, "{");
					
					if (nested_mode) {
						fprintf(output, "\"metadata\":{");
					}
					break;
				case ENDMETADATA:
					
					if (nested_mode) {
						// TODO: Print the X-arrays
						fprintf(output, "},\"data\":[");
					} else {
						fprintf(output, ",");
					}
					
					break;
				case ENDDATA:
				
					if (nested_mode) {
						fprintf(output, "]");
					}
					
					fprintf(output, "}");
					
					data_count = 1;
					
					break;

				default: break;
			}
	}
	
	// Close the input file
	fclose(input);
	input = NULL;
	
	return false;
}

// Convert input data to required type output data
bool convert_all (const char *from, const char *to, bool nested_mode)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

	bool first_file = true;
	
	// Define the output file
	FILE *output = NULL;
	output = fopen(to, "w");
	
	if (!output) {
		printf("OUTPUT FILE ERROR\n");
		return true;
	}

    char sPath[PATH_LENGTH];

    // Specify a file mask. *.* = We want everything!
    sprintf(sPath, "%s/*.*", from);

	printf("Output set to: %s\n", to);
	
	// Check if path is valid
    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path not found: [%s]\n", from);
        return true;
    }

	// OPEN JSON ARRAY
	fprintf(output, "[");

    do
    {
        // Find first file will always return "."
        //    and ".." as the first two directories.
        if (strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
        {

			// Generate the next file path
            sprintf(sPath, "%s/%s", from, fdFile.cFileName);

            // Is the entity a File or Folder?
            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				
                printf("Directory, ignoring! :)");
				
            } else {
				
				// Write the next file to the target
//                printf("Writing next file: %s\n", sPath);
// With no upfront reading, precise result is not reachable

				if (first_file) {
					first_file = false;
				} else {
					fprintf(output, ",\n");
				}

				alcdef2json(sPath, output, nested_mode);
            }
        }
    } while (FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind);

	// CLOSE JSON ARRAY
	fprintf(output, "]");

	fclose(output);
	output = NULL;
	
    return false;
}