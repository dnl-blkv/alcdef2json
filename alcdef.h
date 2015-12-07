#ifndef ALCDEF_H_
#define ALCDEF_H_

#include <stdbool.h>

// The maximal line length for ALCDEF documents is 255
#define ALCDEF_LINE_LENGTH 264

// Field codes
typedef enum {
	WRONG_FIELD = - 1,
	BIBCODE,
	CIBAND,
	CICORRECTION,
	CITARGET,
	COMMENT,
	COMPCI,
	COMPDEC,
	COMPNAME,
	COMPMAG,
	COMPRA,
	CONTACTINFO,
	CONTACTNAME,
	DATA,
	DELIMITER,
	DIFFERMAGS,
	ENDDATA,
	ENDMETADATA,
	FILTER,
	LTCAPP,
	LTCDAYS,
	LTCTYPE,
	MAGADJUST,
	MAGBAND,
	MPCDESIG,
	OBJECTDEC,
	OBJECTNAME,
	OBJECTNUMBER,
	OBJECTRA,
	OBSERVERS,
	OBSLATITUDE,
	OBSLONGITUDE,
	PABB,
	PABL,
	PHASE,
	PUBLICATION,
	REDUCEDMAGS,
	REVISEDDATA,
	SESSIONDATE,
	SESSIONTIME,
	STANDARD,
	STARTMETADATA,
	UCORMAG 
} FieldCode;

// Declare the ALCDEF FIELD data structure
typedef struct s_alcdef_field {
	FieldCode code;
	char name[ALCDEF_LINE_LENGTH];
	char value[ALCDEF_LINE_LENGTH];
} alcdef_field;

// TODO: Move to a separate file

// Converts a string to its equivalent in lower case
char *stolower (char*);

// Escapes a string
int escape_str (char*);

// TODO END

// Gets a code for a given field
int get_field_code (alcdef_field*);

// Creates a new field and populates field from an ALCDEF document line
alcdef_field *create_field (char alcdef_line[]);

// Populates a given existing empty field from a given ALCDEF document line
alcdef_field *populate_field (alcdef_field*, char[]);

// Resets a given field
bool reset_field (alcdef_field*);

// Repopulates a given existing non-empty alcdef field from an alcdef document line
alcdef_field *repopulate_field (alcdef_field*, char[]);

// Check if a given field is a field with value
int field_has_value (int);

// Check if a given field has a value printable in JSON
int field_has_printable_value (int);

#endif