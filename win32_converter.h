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
#ifndef WIN32_ALCDEF_TO_JSON_CONVERTER_H_
#define WIN32_ALCDEF_TO_JSON_CONVERTER_H_

#include <windows.h>

#define MAX_PATH_LENGTH MAX_PATH

typedef enum PathType {
  kDirectoryPath,
  kFilePath,
  kOtherPath,
  kWrongPath
} PathType;


PathType GetPathType (const char*);

bool AlcdefFileToJsonFile (const char*, const char*, const bool);

bool AlcdefDirToJsonFile (const char*, const char*, const bool);

bool AlcdefDirToJsonDir (const char*, const char*, const bool);

bool ConvertAlcdefToJson (const char*, const char*, const bool);

#endif