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

// Reset a field
bool reset_field (alcdef_field *field_pointer) {
	// Reset the field code
	field_pointer->code = 0;
	
	// Reset the field name and value
	memset(field_pointer->name, 0, ALCDEF_LINE_LENGTH);
	memset(field_pointer->value, 0, ALCDEF_LINE_LENGTH);
	
	return false;
}

// Get the code for a given field
int get_field_code (alcdef_field *field) {
	
	char *name = field->name;
	
	int field_code = WRONG_FIELD;
	  
	if (strcmp(name, "BIBCODE") == 0) {
		field_code = BIBCODE;
	} else if (strcmp(name, "CIBAND") == 0) {
		field_code = CIBAND;
	} else if (strcmp(name, "CICORRECTION") == 0) {
		field_code = CICORRECTION;
	} else if (strcmp(name, "CITARGET") == 0) {
		field_code = CITARGET;
	} else if (strcmp(name, "COMMENT") == 0) {
		field_code = COMMENT;
	} else if (strcmp(name, "CONTACTINFO") == 0) {
		field_code = CONTACTINFO;
	} else if (strcmp(name, "CONTACTNAME") == 0) {
		field_code = CONTACTNAME;
	} else if (strcmp(name, "DATA") == 0) {
		field_code = DATA;
	} else if (strcmp(name, "DELIMITER") == 0) {
		field_code = DELIMITER;
	} else if (strcmp(name, "DIFFERMAGS") == 0) {
		field_code = DIFFERMAGS;
	} else if (strcmp(name, "ENDDATA") == 0) {
		field_code = ENDDATA;
	} else if (strcmp(name, "ENDMETADATA") == 0) {
		field_code = ENDMETADATA;
	} else if (strcmp(name, "FILTER") == 0) {
		field_code = FILTER;
	} else if (strcmp(name, "LTCAPP") == 0) {
		field_code = LTCAPP;
	} else if (strcmp(name, "LTCDAYS") == 0) {
		field_code = LTCDAYS;
	} else if (strcmp(name, "LTCTYPE") == 0) {
		field_code = LTCTYPE;
	} else if (strcmp(name, "MAGADJUST") == 0) {
		field_code = MAGADJUST;
	} else if (strcmp(name, "MAGBAND") == 0) {
		field_code = MAGBAND;
	} else if (strcmp(name, "MPCDESIG") == 0) {
		field_code = MPCDESIG;
	} else if (strcmp(name, "OBJECTDEC") == 0) {
		field_code = OBJECTDEC;
	} else if (strcmp(name, "OBJECTNAME") == 0) {
		field_code = OBJECTNAME;
	} else if (strcmp(name, "OBJECTNUMBER") == 0) {
		field_code = OBJECTNUMBER;
	} else if (strcmp(name, "OBJECTRA") == 0) {
		field_code = OBJECTRA;
	} else if (strcmp(name, "OBSERVERS") == 0) {
		field_code = OBSERVERS;
	} else if (strcmp(name, "OBSLATITUDE") == 0) {
		field_code = OBSLATITUDE;
	} else if (strcmp(name, "OBSLONGITUDE") == 0) {
		field_code = OBSLONGITUDE;
	} else if (strcmp(name, "PABB") == 0) {
		field_code = PABB;
	} else if (strcmp(name, "PABL") == 0) {
		field_code = PABL;
	} else if (strcmp(name, "PHASE") == 0) {
		field_code = PHASE;
	} else if (strcmp(name, "PUBLICATION") == 0) {
		field_code = PUBLICATION;
	} else if (strcmp(name, "REDUCEDMAGS") == 0) {
		field_code = REDUCEDMAGS;
	} else if (strcmp(name, "REVISEDDATA") == 0) {
		field_code = REVISEDDATA;
	} else if (strcmp(name, "SESSIONDATE") == 0) {
		field_code = SESSIONDATE;
	} else if (strcmp(name, "SESSIONTIME") == 0) {
		field_code = SESSIONTIME;
	} else if (strcmp(name, "STANDARD") == 0) {
		field_code = STANDARD;
	} else if (strcmp(name, "STARTMETADATA") == 0) {
		field_code = STARTMETADATA;
	} else if (strcmp(name, "UCORMAG") == 0) {
		field_code = UCORMAG;
	
	// The following fields are numbered, thus
	// strstr() is used instead of strcmp
	} else if (strstr(name, "COMPCI") == name) {
		field_code = COMPCI;
	} else if (strstr(name, "COMPDEC") == name) {
		field_code = COMPDEC;
	} else if (strstr(name, "COMPNAME") == name) {
		field_code = COMPNAME;
	} else if (strstr(name, "COMPMAG") == name) {
		field_code = COMPMAG;
	} else if (strstr(name, "COMPRA") == name) {
		field_code = COMPRA;
	}
	
	return field_code;
}

// Converts all string characters to lower case
char *stolower (char *line) 
{
	char *p = line;
	for (; *p; ++p) *p = tolower(*p);
	
	return line;
}

// Escapes a string
int escape_str (char *str) 
{
	char buffer[ALCDEF_LINE_LENGTH];
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

// Creates a new field, populates it from an ALCDEF document line and returns pointer to it
alcdef_field *create_field (char alcdef_line[]) {
	alcdef_field *field = malloc(sizeof(field));
	
	return populate_field(field, alcdef_line);
}

// Populates an empty alcdef field from an ALCDEF document line
alcdef_field *populate_field (alcdef_field *field_pointer, char alcdef_line[]) {
	
	// Get the token(s)
	char * token = strtok(alcdef_line, "=");

	// Copy the name
	strcpy(field_pointer->name, token);
	
	// Get the next token
	token = strtok(NULL, "=");
	
	// If token is present, copy it
	if (token) {
		strcpy(field_pointer->value, token);
	}
	
	// Save the field code
	field_pointer->code = get_field_code(field_pointer);
	
	// ERROR: wrong field name discovered
	if (field_pointer->code == WRONG_FIELD) {
		printf("WRONG FIELD NAME: %s\n", field_pointer->name);
	}
	
	// Posprocess the field to prepare it for JSON
	
	// Bring the field name to lower case
	stolower(field_pointer->name);
	
	// Escape the field value for further processing
	escape_str(field_pointer->value);

	// Return the field
	return field_pointer;
}

// Repopulates a non-empty alcdef field from an ALCDEF document line
alcdef_field *repopulate_field (alcdef_field *field_pointer, char alcdef_line[]) {
	// Reset the field's member values
	reset_field(field_pointer);
	
	populate_field(field_pointer, alcdef_line);
	
	return field_pointer;
}

// Check if a given field is a valid field with value
int field_has_value (int field_code) {
	return ((field_code != WRONG_FIELD) && 
	(field_code != STARTMETADATA) &&
	(field_code != ENDMETADATA) &&
	(field_code != ENDDATA));
}

// Check if a given field has a value printable in JSON
int field_has_printable_value (int field_code) {
	return (field_has_value(field_code) && (field_code != DELIMITER));
}
