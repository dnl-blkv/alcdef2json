/* The MIT License (MIT)

Copyright (c) 2015 Daniil Belyakov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

// Include the required libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

// Include the local ALCDEF library
#include "alcdef.h"

// Define testing parameters
//#define TEST
#define LOWER_BORDER 1
#define HIGHER_BORDER 14
#define REPORT_LOW 26
#define REPORT_HIGH 1024

// TODO: Probably make user-definable?
#define NESTED_MODE true

// Define the statistical data structure
typedef struct s_statistics {
	// Total number of documents
	int total_doc_count;
	
	// Number of processed documents
	int doc_count;
	
	// Total number of observations
	int total_obs_count;
	
	// Number of processed observations
	int obs_count;
} statistics;

// Define the ALCDEF FIELD data structure
typedef struct s_alcdef_field {
	int code;
	char name[LINE_LENGTH];
	char value[LINE_LENGTH];
} alcdef_field;

// Define utils
// Define boolean data structure for convenience
typedef enum { false, true } bool;

// Define method for string conversion to lower case characters
char * stolower (char * line) {
	char * p = line;
	for ( ; *p; ++p) *p = tolower(*p);
	
	return line;
}

// Define method for string escaping
int escape_str (char * str) {
	char buffer[LINE_LENGTH];
	int i = 0, n = 0;

	for (i = 0; i < strlen(str); i ++) {
		if (str[i] == '"') {
			sprintf(buffer + i + n, "\\%c", str[i]);
			n ++;
		} else if (str[i] == ',') {
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

int get_field_code (char line[]) {
	int field_code = WRONG_FIELD;
	  
	if (strstr(line, "BIBCODE") == line) {
		field_code = BIBCODE;
	} else if (strstr(line, "CIBAND") == line) {
		field_code = CIBAND;
	} else if (strstr(line, "CICORRECTION") == line) {
		field_code = CICORRECTION;
	} else if (strstr(line, "CITARGET") == line) {
		field_code = CITARGET;
	} else if (strstr(line, "COMMENT") == line) {
		field_code = COMMENT;
	} else if (strstr(line, "COMPCI") == line) {
		field_code = COMPCI;
	} else if (strstr(line, "COMPDEC") == line) {
		field_code = COMPDEC;
	} else if (strstr(line, "COMPNAME") == line) {
		field_code = COMPNAME;
	} else if (strstr(line, "COMPMAG") == line) {
		field_code = COMPMAG;
	} else if (strstr(line, "COMPRA") == line) {
		field_code = COMPRA;
	} else if (strstr(line, "CONTACTINFO") == line) {
		field_code = CONTACTINFO;
	} else if (strstr(line, "CONTACTNAME") == line) {
		field_code = CONTACTNAME;
	} else if (strstr(line, "DATA") == line) {
		field_code = DATA;
	} else if (strstr(line, "DELIMITER") == line) {
		field_code = DELIMITER;
	} else if (strstr(line, "DIFFERMAGS") == line) {
		field_code = DIFFERMAGS;
	} else if (strstr(line, "ENDDATA") == line) {
		field_code = ENDDATA;
	} else if (strstr(line, "ENDMETADATA") == line) {
		field_code = ENDMETADATA;
	} else if (strstr(line, "FILTER") == line) {
		field_code = FILTER;
	} else if (strstr(line, "LTCAPP") == line) {
		field_code = LTCAPP;
	} else if (strstr(line, "LTCDAYS") == line) {
		field_code = LTCDAYS;
	} else if (strstr(line, "LTCTYPE") == line) {
		field_code = LTCTYPE;
	} else if (strstr(line, "MAGADJUST") == line) {
		field_code = MAGADJUST;
	} else if (strstr(line, "MAGBAND") == line) {
		field_code = MAGBAND;
	} else if (strstr(line, "MPCDESIG") == line) {
		field_code = MPCDESIG;
	} else if (strstr(line, "OBJECTDEC") == line) {
		field_code = OBJECTDEC;
	} else if (strstr(line, "OBJECTNAME") == line) {
		field_code = OBJECTNAME;
	} else if (strstr(line, "OBJECTNUMBER") == line) {
		field_code = OBJECTNUMBER;
	} else if (strstr(line, "OBJECTRA") == line) {
		field_code = OBJECTRA;
	} else if (strstr(line, "OBSERVERS") == line) {
		field_code = OBSERVERS;
	} else if (strstr(line, "OBSLATITUDE") == line) {
		field_code = OBSLATITUDE;
	} else if (strstr(line, "OBSLONGITUDE") == line) {
		field_code = OBSLONGITUDE;
	} else if (strstr(line, "PABB") == line) {
		field_code = PABB;
	} else if (strstr(line, "PABL") == line) {
		field_code = PABL;
	} else if (strstr(line, "PHASE") == line) {
		field_code = PHASE;
	} else if (strstr(line, "PUBLICATION") == line) {
		field_code = PUBLICATION;
	} else if (strstr(line, "REDUCEDMAGS") == line) {
		field_code = REDUCEDMAGS;
	} else if (strstr(line, "REVISEDDATA") == line) {
		field_code = REVISEDDATA;
	} else if (strstr(line, "SESSIONDATE") == line) {
		field_code = SESSIONDATE;
	} else if (strstr(line, "SESSIONTIME") == line) {
		field_code = SESSIONTIME;
	} else if (strstr(line, "STANDARD") == line) {
		field_code = STANDARD;
	} else if (strstr(line, "STARTMETADATA") == line) {
		field_code = STARTMETADATA;
	} else if (strstr(line, "UCORMAG") == line) {
		field_code = UCORMAG;
	}
	
	return field_code;
}

// Prepare a field for json
bool prepare_for_json (alcdef_field * field_pointer) {

	// Bring the field name to lower case
	stolower(field_pointer -> name);
	
	// Escape the field value for further processing
	escape_str(field_pointer -> value);
	
	return false;
}

// Define method for fetching a field data
alcdef_field get_field_data (char line[]) {
	
	// Define the pointer to the name-value delimiter
	char * delimiter_pointer = strchr(line, '=');
	
	// Calculate the field name length
	int name_length = (delimiter_pointer - line);
	
	// Create the new field data structure
	alcdef_field field;

	// Reset the field's values
	memset(field.name, 0, LINE_LENGTH);
	memset(field.value, 0, LINE_LENGTH);
	
	// Save the field name and value
	// If there is a delimiter, save field name and value
	if (delimiter_pointer != NULL) {
		strncpy(field.name, line, name_length);
		strcpy(field.value, delimiter_pointer + 1);
	// Otherwise, save name and set value to an empty line
	} else {
		strcpy(field.name, line);
		strcpy(field.value, "");
	}
	
	// Save the field code
	field.code = get_field_code(line);
	
	// TODO: Probably move outside the method?
	// Prepare for JSON
	prepare_for_json(&field);

	return field;
}

int field_has_value (int field_code) {
	return ((field_code != WRONG_FIELD) && 
	(field_code != STARTMETADATA) &&
	(field_code != ENDMETADATA) &&
	(field_code != ENDDATA));
}

int field_has_printable_value (int field_code) {
	return (field_has_value(field_code) && (field_code != DELIMITER));
}

int output_boolean (FILE * output, alcdef_field field) {
	return fprintf(output, "\"%s\":%s", field.name, stolower(field.value));
}

int output_string (FILE * output, alcdef_field field) {
	return fprintf(output, "\"%s\":\"%s\"", field.name, field.value);
}

int output_integer (FILE * output, alcdef_field field) {
	return fprintf(output, "\"%s\":%s", field.name, field.value);
}

int output_double (FILE * output, alcdef_field field) {
	
	if (strlen(field.value) == 0) {
		strcpy(field.value, "null");
	}
	
	return fprintf(output, "\"%s\":%s", field.name, field.value);
}

void output_data (FILE * output, alcdef_field field, char * delimiter) {
	char tokens[LINE_LENGTH];
	strcpy(tokens, field.value);
	char * token = strtok(tokens, delimiter);
	
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

void output_flat_data (FILE * output, alcdef_field field, char * delimiter, int entry_number) {
	char tokens[LINE_LENGTH];
	strcpy(tokens, field.value);
	char * token = strtok(tokens, delimiter);
	
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
statistics alcdef2json (const char * file_path, FILE * output, statistics stats) {
	
	// Open the file
	FILE * input = fopen(file_path, "r");
	
	// If the next file was failed to open, report error
	if (!input) {
		printf("INPUT FILE ERROR! Path: %s\n", file_path);
		getchar();
		return stats;
	}

	// Define the old field code and data fields counter
	int old_field_code = WRONG_FIELD, data_count = 1;
	

	
	// Create the field buffer
	alcdef_field field;
	field.code = WRONG_FIELD;

	// Define the line buffer and delimiter variable
	char line[LINE_LENGTH], * delimiter;

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
		field = get_field_data(line);
#ifdef TEST		
	if (LOWER_BORDER <= stats.total_obs_count && stats.total_obs_count < HIGHER_BORDER) {
#endif	
			if (field_has_printable_value(field.code) && field_has_value(old_field_code)) {
				fprintf(output, ",");
			}
			
			if ((old_field_code == ENDDATA) && 
				(field.code == STARTMETADATA)) {
#ifdef TEST		
	if (LOWER_BORDER < stats.total_obs_count) {
#endif	
				fprintf(output, ",\n");
#ifdef TEST
	}
#endif
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
					output_string(output, field);
					break;
					
				// Boolean case
				case CICORRECTION:
				case DIFFERMAGS:
				case REVISEDDATA:
					output_boolean(output, field);
					break;
					
				// Double case
				case CITARGET:
				case LTCDAYS:
				case MAGADJUST:
				case OBSLATITUDE:
				case OBSLONGITUDE:
				case PABB:
				case PABL:
				case PHASE:
				case UCORMAG:
					output_double(output, field);
					break;
					
				// Integer case
				case OBJECTNUMBER:
					output_integer(output, field);
					break;
					
				// START X - VALUES
				// Nested Case: Put to arrays, output on ENDMETADATA
				case COMPCI:
					output_double(output, field);
					break;
				case COMPDEC:
					output_string(output, field);
					break;
				case COMPMAG:
					output_double(output, field);
					break;
				case COMPNAME:
					output_string(output, field);
					break;
				case COMPRA:
					output_string(output, field);
					break;

				// DATA VALUES
				case DATA:
					// Check the delimiter
					if (NESTED_MODE) {
						output_data(output, field, delimiter);
					} else {
						output_flat_data(output, field, delimiter, data_count);
					}
					
					data_count ++;
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
					
					if (NESTED_MODE) {
						fprintf(output, "\"metadata\":{");
					}
					break;
				case ENDMETADATA:
					
					if (NESTED_MODE) {
						// TODO: Print the X-arrays
						fprintf(output, "},\"data\":[");
					} else {
						fprintf(output, ",");
					}
					
					break;
				case ENDDATA:
				
					if (NESTED_MODE) {
						fprintf(output, "]");
					} else if (REPORT_LOW < data_count && data_count < REPORT_HIGH) {
						stats.obs_count ++;
						printf ("data_count: %d, stats.obs_count: %d\n", data_count, stats.obs_count);
					}
					
					fprintf(output, "}");
					
					data_count = 1;
					
					break;

				default: break;
			}
			
#ifdef TEST
	}
#endif

		// If the field was ENDDATA, increase the total observations counter
		if (field.code == ENDDATA) {
			stats.total_obs_count ++;
		}
#ifdef TEST
	if (HIGHER_BORDER <= stats.total_obs_count) {
		break;
	}
#endif
	}
	
	// Close the input file
	fclose(input);
	input = NULL;
	
	// Increase the total document counter
	stats.total_doc_count ++;
	
	return stats;
}

bool write_alcdefs_to_json (const char * from, const char * to)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
	
	// Define the statistical structure
	statistics stats;
	stats.total_doc_count = 0;
	stats.doc_count = 0;
	stats.total_obs_count = 0;
	stats.obs_count = 0;

	int old_obs_count = 0;
	bool first_file = true;
	
	// Define the output file
	FILE * output = NULL;
	output = fopen(to, "w");
	
	if (!output) {
		printf("OUTPUT FILE ERROR\n");
		return true;
	}

    char sPath[FILE_NAME_LENGTH];

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
#ifdef TEST
	if (LOWER_BORDER <= stats.total_obs_count && stats.total_obs_count < HIGHER_BORDER) {
#endif
				if (first_file) {
					first_file = false;
				} else {
					fprintf(output, ",\n");
				}
#ifdef TEST
	}
#endif
				old_obs_count = stats.obs_count;

				stats = alcdef2json(sPath, output, stats);
				
				if (old_obs_count < stats.obs_count) {
					stats.doc_count ++;
					printf("stats.doc_count: %d\n", stats.doc_count);
				}
				
#ifdef TEST
	if (HIGHER_BORDER <= stats.total_obs_count) {
		break;
	}
#endif				
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

int main() {
	write_alcdefs_to_json("C://alcdef2json/alcdef", "C://alcdef2json/json/alcdefs.json");

	return false;
}
