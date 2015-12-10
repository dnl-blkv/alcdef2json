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
#ifndef ALCDEF_H_
#define ALCDEF_H_

#include <stdbool.h>

// The maximal line length for ALCDEF documents is 255
#define MAX_ALCDEF_LINE_LENGTH 256

// ALCDEF field codes
typedef enum {
  kWrongField = 0,
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
} AlcdefFieldCode;

// Declare the ALCDEF FIELD data structure
typedef struct AlcdefField {
  AlcdefFieldCode code;
  char name[MAX_ALCDEF_LINE_LENGTH];
  char value[MAX_ALCDEF_LINE_LENGTH];
} AlcdefField;

// Gets a code for a given field
AlcdefFieldCode FindFieldCode (const AlcdefField*);

// Populates a given existing empty field from a given ALCDEF document line
AlcdefField *PopulateField (const char*, AlcdefField*);

// Resets a given field
bool ResetField (AlcdefField*);

// Repopulates a given existing non-empty alcdef field from an alcdef document line
AlcdefField *RepopulateField (const char*, AlcdefField*);

// Checks if a given field is a field with value
bool field_has_value (const AlcdefFieldCode);

// Checks if a given field has a value printable in JSON
bool field_has_significant_value (const AlcdefFieldCode);

#endif