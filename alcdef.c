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

// Get the code for a given field
FieldCode GetFieldCode (const AlcdefField *field) {
  
  const char *name = field->name;
  
  FieldCode field_code = kWrongField;
    
  if (strcmp(name, "BIBCODE") == 0) {
    field_code = kBibCode;
  } else if (strcmp(name, "CIBAND") == 0) {
    field_code = kCiBand;
  } else if (strcmp(name, "CICORRECTION") == 0) {
    field_code = kCiCorrection;
  } else if (strcmp(name, "CITARGET") == 0) {
    field_code = kCiTarget;
  } else if (strcmp(name, "COMMENT") == 0) {
    field_code = kComment;
  } else if (strcmp(name, "CONTACTINFO") == 0) {
    field_code = kContactInfo;
  } else if (strcmp(name, "CONTACTNAME") == 0) {
    field_code = kContactName;
  } else if (strcmp(name, "DATA") == 0) {
    field_code = kData;
  } else if (strcmp(name, "DELIMITER") == 0) {
    field_code = kDelimiter;
  } else if (strcmp(name, "DIFFERMAGS") == 0) {
    field_code = kDifferMags;
  } else if (strcmp(name, "ENDDATA") == 0) {
    field_code = kEndData;
  } else if (strcmp(name, "ENDMETADATA") == 0) {
    field_code = kEndMetadata;
  } else if (strcmp(name, "FILTER") == 0) {
    field_code = kFilter;
  } else if (strcmp(name, "LTCAPP") == 0) {
    field_code = kLtcApp;
  } else if (strcmp(name, "LTCDAYS") == 0) {
    field_code = kLtcDays;
  } else if (strcmp(name, "LTCTYPE") == 0) {
    field_code = kLtcType;
  } else if (strcmp(name, "MAGADJUST") == 0) {
    field_code = kMagAdjust;
  } else if (strcmp(name, "MAGBAND") == 0) {
    field_code = kMagBand;
  } else if (strcmp(name, "MPCDESIG") == 0) {
    field_code = kMpcDesig;
  } else if (strcmp(name, "OBJECTDEC") == 0) {
    field_code = kObjectDec;
  } else if (strcmp(name, "OBJECTNAME") == 0) {
    field_code = kObjectName;
  } else if (strcmp(name, "OBJECTNUMBER") == 0) {
    field_code = kObjectNumber;
  } else if (strcmp(name, "OBJECTRA") == 0) {
    field_code = kObjectRa;
  } else if (strcmp(name, "OBSERVERS") == 0) {
    field_code = kObservers;
  } else if (strcmp(name, "OBSLATITUDE") == 0) {
    field_code = kObsLatitude;
  } else if (strcmp(name, "OBSLONGITUDE") == 0) {
    field_code = kObsLongitude;
  } else if (strcmp(name, "PABB") == 0) {
    field_code = kPabB;
  } else if (strcmp(name, "PABL") == 0) {
    field_code = kPabL;
  } else if (strcmp(name, "PHASE") == 0) {
    field_code = kPhase;
  } else if (strcmp(name, "PUBLICATION") == 0) {
    field_code = kPublication;
  } else if (strcmp(name, "REDUCEDMAGS") == 0) {
    field_code = kReducedMags;
  } else if (strcmp(name, "REVISEDDATA") == 0) {
    field_code = kRevisedData;
  } else if (strcmp(name, "SESSIONDATE") == 0) {
    field_code = kSessionDate;
  } else if (strcmp(name, "SESSIONTIME") == 0) {
    field_code = kSessionTime;
  } else if (strcmp(name, "STANDARD") == 0) {
    field_code = kStandard;
  } else if (strcmp(name, "STARTMETADATA") == 0) {
    field_code = kStartMetadata;
  } else if (strcmp(name, "UCORMAG") == 0) {
    field_code = kUCorMag;
  // The following fields are suffixed with numbers, thus
  // strstr() is used instead of strcmp
  } else if (strstr(name, "COMPCI") == name) {
    field_code = kCompCi;
  } else if (strstr(name, "COMPDEC") == name) {
    field_code = kCompDec;
  } else if (strstr(name, "COMPNAME") == name) {
    field_code = kCompName;
  } else if (strstr(name, "COMPMAG") == name) {
    field_code = kCompMag;
  } else if (strstr(name, "COMPRA") == name) {
    field_code = kCompRa;
  }
  
  return field_code;
}

// Creates a new field, populates it from an ALCDEF document line and returns pointer to it
AlcdefField *CreateField (const char *alcdef_line) {
  AlcdefField *field = malloc(sizeof(field));
  
  return PopulateField(alcdef_line, field);
}

// Populates an empty alcdef field from an ALCDEF document line
AlcdefField *PopulateField (const char *alcdef_line, AlcdefField *field) {
  
  // Get the token(s)
  char name_value[MAX_ALCDEF_LINE_LENGTH];
  strcpy(name_value, alcdef_line);
  char *token = strtok(name_value, "=");

  // Copy the name
  strcpy(field->name, token);
  
  // Get the next token
  token = strtok(NULL, "=");
  
  // If token is present, copy it
  if (token) {
    strcpy(field->value, token);
  }
  
  // Save the field code
  field->code = GetFieldCode(field);
  
  // ERROR: wrong field name discovered
  if (field->code == kWrongField) {
    printf("WRONG FIELD NAME: %s\n", field->name);
  }

  // Return the field
  return field;
}

// Reset a field
bool ResetField (AlcdefField *field) {
  // Reset the field code
  field->code = kWrongField;
  
  // Reset the field name and value
  memset(field->name, 0, MAX_ALCDEF_LINE_LENGTH);
  memset(field->value, 0, MAX_ALCDEF_LINE_LENGTH);
  
  return false;
}

// Repopulates a non-empty alcdef field from an ALCDEF document line
AlcdefField *RepopulateField (const char *alcdef_line, AlcdefField *field) {
  // Reset the field's member values
  ResetField(field);
  
  PopulateField(alcdef_line, field);
  
  return field;
}

// Check if a given field is a valid field with value
bool field_has_value (const FieldCode field_code) {
  return ((field_code != kWrongField) && (field_code != kStartMetadata) &&
      (field_code != kEndMetadata) && (field_code != kEndData));
}

// Check if a given field has a value printable in JSON
bool field_has_printable_value (const FieldCode field_code) {
  return (field_has_value(field_code) && (field_code != kDelimiter));
}
