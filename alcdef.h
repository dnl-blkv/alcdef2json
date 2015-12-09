#ifndef ALCDEF_H_
#define ALCDEF_H_

// Include global headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

// The maximal line length for ALCDEF documents is 255
#define MAX_ALCDEF_LINE_LENGTH 256

// Field codes
typedef enum {
	kWrongField = - 1,
	kBibCode,
	kCiBand,
	kCiCorrection,
	kCiTarget,
	kComment,
	kCompCi,
	kCompDec,
	kCompName,
	kCompMag,
	kCompRa,
	kContactInfo,
	kContactName,
	kData,
	kDelimiter,
	kDifferMags,
	kEndData,
	kEndMetadata,
	kFilter,
	kLtcApp,
	kLtcDays,
	kLtcType,
	kMagAdjust,
	kMagBand,
	kMpcDesig,
	kObjectDec,
	kObjectName,
	kObjectNumber,
	kObjectRa,
	kObservers,
	kObsLatitude,
	kObsLongitude,
	kPabB,
	kPabL,
	kPhase,
	kPublication,
	kReducedMags,
	kRevisedData,
	kSessionDate,
	kSessionTime,
	kStandard,
	kStartMetadata,
	kUCorMag 
} FieldCode;

// Declare the ALCDEF FIELD data structure
typedef struct AlcdefField {
	FieldCode code;
	char name[MAX_ALCDEF_LINE_LENGTH];
	char value[MAX_ALCDEF_LINE_LENGTH];
} AlcdefField;

// Gets a code for a given field
FieldCode GetFieldCode (const AlcdefField*);

// Creates a new field and populates field from an ALCDEF document line
AlcdefField *CreateField (const char*);

// Populates a given existing empty field from a given ALCDEF document line
AlcdefField *PopulateField (const char*, AlcdefField*);

// Resets a given field
bool ResetField (AlcdefField*);

// Repopulates a given existing non-empty alcdef field from an alcdef document line
AlcdefField *RepopulateField (const char*, AlcdefField*);

// Check if a given field is a field with value
bool field_has_value (const FieldCode);

// Check if a given field has a value printable in JSON
bool field_has_printable_value (const FieldCode);

#endif