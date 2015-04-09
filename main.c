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
#define LOWER_BORDER 22883
#define HIGHER_BORDER 22885

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

// Define method for fetching a field data
alcdef_field get_field_data (char line[]) {
	
	// Define the pointer to the name-value delimiter
	char * delimiter_pointer = strchr(line, '=');
	
	// Calculate the field name length
	int name_length = (delimiter_pointer - line);
	
	// Create the new field data structure
	alcdef_field field;
	
	// Reset the field's values
	memset(field.value, 0, strlen(field.value));
	memset(field.name, 0, strlen(field.name));
	
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
	// Process te field name and value for JSON
	// Bring the field name to lower case
	stolower(field.name);
	
	// Escape the field value for further processing
	escape_str(field.value);

	return field;
}

int field_has_value (int field_code) {
	return (field_code != WRONG_FIELD) && 
	(field_code != STARTMETADATA) &&
	(field_code != ENDMETADATA) &&
	(field_code != ENDDATA);
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
	fprintf(output, "{\"jd\":\"%s\"", token);
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
	fprintf(output, "\"data_jd%d\":\"%s\"", entry_number, token);
	token = strtok(NULL, delimiter);

	// Output the magnitude
	fprintf(output, ",\"data_mag%d\":%s", entry_number, token);
	token = strtok(NULL, delimiter);
	
	// Output the magnitude error, if present
	if (token) {
		fprintf(output, ",\"data_magerr%d\":%s", entry_number, token);
		token = strtok(NULL, delimiter);
	}
	
	// Output the air mass, if present
	if (token) {
		fprintf(output, ",\"data_airmass%d\":%s", entry_number, token);
	}
}

int alcdef2json (const char * next_file_path, FILE * output, int old_doc_count, int * old_count) {
	FILE * input = NULL;
	char buf[LINE_LENGTH], * delimiter;
	int old_field_code = WRONG_FIELD, doc_count = old_doc_count, data_count = 1;
	alcdef_field field;
	field.code = WRONG_FIELD;
	size_t ln;
	bool nested_mode = false;
	
	// count is an amount of observations with amount of points within the low-high corridor
	int low = 26, high = 1024, count = (* old_count);
	
	input = fopen(next_file_path, "r");
	
	if (!input) {
		printf("INPUT FILE ERROR! Path: %s\n", next_file_path);
		getchar();
		return true;
	}

	while (fgets(buf, LINE_LENGTH, input) != NULL) {
		
		// Save the old field code
		old_field_code = field.code;

		ln = strlen(buf) - 1;
		
		if (buf[ln] == '\n') {
			buf[ln] = '\0';
		}
		
		// Get the field code
		field = get_field_data(buf);

#ifdef TEST		
	if (LOWER_BORDER <= doc_count && doc_count < HIGHER_BORDER) {
#else
//	if (last_data_count >= data_count_threshold) {
#endif	
			if (((field_has_value(field.code) && 
				field_has_value(old_field_code)) ||
				((old_field_code == ENDDATA) && 
				(field.code == STARTMETADATA))) &&
				(field.code != DELIMITER)) {
				fprintf(output, ",");
			}
			
			if ((old_field_code == ENDDATA) && 
				(field.code == STARTMETADATA)) {
				fprintf(output, "\n");
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
				case COMPCI:
					// Put to COMPCI object, output on ENDMETADATA
					output_double(output, field);
					break;
				case COMPDEC:
					// Put to COMPDEC object, output on ENDMETADATA
					output_string(output, field);
					break;
				case COMPMAG:
					// Put to COMPMAG object, output on ENDMETADATA
					output_double(output, field);
					break;
				case COMPNAME:
					// Put to COMPNAME object, output on ENDMETADATA
					output_string(output, field);
					break;
				case COMPRA:
					// Put to COMPRA object, output on ENDMETADATA
					output_string(output, field);
					break;

				// DATA VALUES
				case DATA:
					// Check the delimiter
					if (nested_mode) {
						output_data(output, field, delimiter);
					} else {
						output_flat_data(output, field, delimiter, data_count);
						data_count ++;
					}
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
					// Print the X-objects (comp-objects)
					if (nested_mode) {
						fprintf(output, "},\"data\":[");
					} else {
						fprintf(output, ",");
						if (low < data_count && data_count < high) {
							count ++;
							printf ("data_count: %d, count: %d\n", data_count, count);
						}
						data_count = 1;
					}
					break;
				case ENDDATA:
				
					if (nested_mode) {
						fprintf(output, "]");
					}
					
					fprintf(output, "}");
					
					break;

				default: break;
			}
			
#ifdef TEST
	}
#endif

		if (field.code == ENDDATA) {
			doc_count ++;
//			printf("doc_count: %d\n", doc_count);
		}
	}
	
	fclose(input);
	input = NULL;
	
	// Return the required size elements count
	(* old_count) = count;
	return doc_count;
}

bool write_alcdefs_to_json (const char * from, const char * to)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
	
	// elder_count describes the corridorable observations amount after previous asteroid
	int doc_count = 0, * corridor_size_count, elder_count = 0, corridor_asteroids_count = 0;
	
	corridor_size_count = (int *)malloc(sizeof(int));
	*corridor_size_count = 0;
	
	FILE * output = NULL;
	int first_file = 1, counter = 0;

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
            // Build up our file path using the passed in
            // [sDir] and the file/foldername we just found:
//			printf("#%d\n", counter);
			counter ++;
            sprintf(sPath, "%s/%s", from, fdFile.cFileName);

            // Is the entity a File or Folder?
            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				
                printf("Directory, ignoring! :)");
				
            } else {
				
				// Write the next file to the target
//                printf("Writing next file: %s\n", sPath);
#ifdef TEST
	if (LOWER_BORDER <= doc_count && doc_count < HIGHER_BORDER) {
#endif
				if (!first_file) {
					fprintf(output, ",\n");
				} else {
					first_file = 0;
				}
#ifdef TEST
	}
#endif
				elder_count = * corridor_size_count;
				doc_count = alcdef2json(sPath, output, doc_count, corridor_size_count);
				
				if (elder_count < (* corridor_size_count)) {
					corridor_asteroids_count ++;
					printf("corridor_asteroids_count: %d\n", corridor_asteroids_count);
				}
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
