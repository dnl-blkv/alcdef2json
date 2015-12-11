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
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "alcdef_to_json.h"
#include "win32_converter.h"

// Gets the type of a given path
PathType GetPathType (const char *path) {
  struct stat s;
  char dir_path[MAX_PATH_LENGTH + 1];
  char *filename;
  
  if (stat(path, &s) == 0) {
    if (s.st_mode & S_IFDIR) {
      return kDirectoryPath;
    } else if (s.st_mode & S_IFREG) {
      return kFilePath;
    } else {
      return kOtherPath;
    }
  } else {
    // Check if the path is valid but the file does not exist
    if (errno == ENOENT) {
      // Copy the path for safe modification
      strcpy(dir_path, path);
      // Find the beginning of the expected filename
      filename = strrchr(dir_path, '/');
      // Relace the first filename character with line termination character
      *(filename + 1) = '\0';
      // If the resulting directory path is valid and existing, we assume
      // that the original path was a file path
      if ((stat(dir_path, &s) == 0) && (s.st_mode & S_IFDIR)) {
        return kFilePath;
      } 
    }
      
    return kWrongPath;
  }
}

// Converts A single ALCDEF directory to a single JSON file
bool AlcdefFileToJsonFile (const char *alcdef_path, const char *json_path, const bool flat_mode) {
  FILE *input = NULL;
  input = fopen(alcdef_path, "r");
  
  printf("Input path: [%s]\n", alcdef_path);
  
  if (!input) {
    printf("Input path error!\n");
    return true;
  }
  
  FILE *output = NULL;
  output = fopen(json_path, "w");
  
  printf("Output path: [%s]\n", json_path);
  
  if (!output) {
    printf("Output path error!\n");
    return true;
  }
  
  fprintf(output, "[");

  AlcdefToJson(output, input, flat_mode);

  fprintf(output, "]");
  
  fclose(input);
  input = NULL;
  
  fclose(output);
  output = NULL;
  
  return false;
}

// Converts A single ALCDEF directory to a single JSON file
bool AlcdefDirToJsonFile (const char *alcdef_path, const char *json_path, const bool flat_mode) {
  // The Win32 variables have names in mixed case due to the naming standard
  // used in windows.h library
  WIN32_FIND_DATA findData;
  HANDLE findHandle = NULL;
  
  // Get the ALCDEF path with *.* mask to find all the files in the directory
  char masked_alcdef_path[MAX_PATH_LENGTH];
  sprintf(masked_alcdef_path, "%s/*.*", alcdef_path);
  
  printf("Input path: [%s]\n", alcdef_path);
  
  // Check if the masked path is valid
  if ((findHandle = FindFirstFile(masked_alcdef_path, &findData)) == INVALID_HANDLE_VALUE) {
    printf("Path not found: [%s]\n", alcdef_path);
    return true;
  }

  bool first_file_processed = false;

  FILE *output = NULL;
  output = fopen(json_path, "w");
  
  printf("Output path: [%s]\n", json_path);
  
  if (!output) {
    printf("Output path error!\n");
    return true;
  }
  
  fprintf(output, "[");

  FILE *input = NULL;

  do
  {
    // Find first file will always return "." and ".." as the first two
    // directory children
    if ((strcmp(findData.cFileName, ".") != 0) && 
        (strcmp(findData.cFileName, "..") != 0)) {

      // Generate the next input file path
      sprintf(masked_alcdef_path, "%s/%s", alcdef_path, findData.cFileName);

      // If not a directory
      if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

        if (first_file_processed) {
          fprintf(output, ",\n");
        } else {
          first_file_processed = true;
        }

        input = fopen(masked_alcdef_path, "r");
      
        if (!input) {
          printf("Input path error!\n");
          return true;
        }

        AlcdefToJson(output, input, flat_mode);
      
        fclose(input);
        input = NULL;
      }
    }
  } while (FindNextFile(findHandle, &findData)); //Find the next file.

  FindClose(findHandle);

  fprintf(output, "]");

  fclose(output);
  output = NULL;
  
  return false;
}

// Converts A single ALCDEF directory to a single JSON file
bool AlcdefDirToJsonDir (const char *alcdef_path, const char *json_path, const bool flat_mode) {
  // The Win32 variables have names in mixed case due to the naming standard
  // used in windows.h library
  WIN32_FIND_DATA findData;
  HANDLE findHandle = NULL;
  
  // Get the ALCDEF path with *.* mask to find all the files in the directory
  char masked_alcdef_path[MAX_PATH_LENGTH];
  sprintf(masked_alcdef_path, "%s/*.*", alcdef_path);
  
  printf("Input path: [%s]\n", alcdef_path);
  
  // Check if the masked path is valid
  if ((findHandle = FindFirstFile(masked_alcdef_path, &findData)) == INVALID_HANDLE_VALUE) {
    printf("Path not found: [%s]\n", alcdef_path);
    return true;
  }
  
  char masked_json_path[MAX_PATH_LENGTH];
  
  printf("Output path: [%s]\n", json_path);
  
  FILE *output = NULL;
  FILE *input = NULL;

  do
  {
    // Find first file will always return "." and ".." as the first two
    // directory children
    if ((strcmp(findData.cFileName, ".") != 0) && 
        (strcmp(findData.cFileName, "..") != 0)) {

      // Generate the next input files paths
      sprintf(masked_alcdef_path, "%s/%s", alcdef_path, findData.cFileName);
      sprintf(masked_json_path, "%s/%s.json", json_path, findData.cFileName);

      // If not a directory
      if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

        input = fopen(masked_alcdef_path, "r");
      
        if (!input) {
          printf("Input path error!\n");
          return true;
        }
        
        output = fopen(masked_json_path, "w");
  
        if (!output) {
          printf("Output path error!\n");
          return true;
        }
        
        fprintf(output, "[");
        
        AlcdefToJson(output, input, flat_mode);
        
        fprintf(output, "]");
        
        fclose(input);
        input = NULL;

        fclose(output);
        output = NULL;
      }
    }
  } while (FindNextFile(findHandle, &findData)); //Find the next file.

  FindClose(findHandle);
  
  return false;
}

// Converts ALCDEF file(s) to JSON file(s)
bool ConvertAlcdefToJson (const char *alcdef_path, const char *json_path, const bool flat_mode)
{
  // Get ALCDEF path
  PathType alcdef_path_type = GetPathType(alcdef_path);
  
  if ((alcdef_path_type == kOtherPath) || (alcdef_path_type == kWrongPath)) {
    printf("ERROR: wrong input path discovered! Path type: %d\n Path: [%s]\n", alcdef_path_type, alcdef_path);
    return true;
  }
  
  // Get JSON path
  PathType json_path_type = GetPathType(json_path);

  if ((json_path_type == kOtherPath) || (json_path_type == kWrongPath)) {
    printf("ERROR: wrong output path discovered! Path type: %d\n Path: [%s]\n", json_path_type, json_path);
    return true;
  }
  
  char autonamed_json_file_path[MAX_PATH_LENGTH];
  const char *alcdef_file_name;
  
  if (alcdef_path_type == kDirectoryPath) {
    if (json_path_type == kDirectoryPath) {
      // 1. Directory to Directory. Converts all ALCDEF files in a directory into a
      // directory of automatically named JSON files.
      AlcdefDirToJsonDir(alcdef_path, json_path, flat_mode);
    } else {
      // 2. Directory to File. Converts all ALCDEF files in a directory into a single
      // JSON file
      AlcdefDirToJsonFile(alcdef_path, json_path, flat_mode);
    }
  } else {
    if (json_path_type == kDirectoryPath) {
      // 3. File to Directory. Equivalent to case 4., but the output file is named 
      // automatically
      if ((alcdef_file_name = strrchr(alcdef_path, '/')) == NULL) {
        alcdef_file_name = alcdef_path;
      }
      sprintf(autonamed_json_file_path, "%s%s.json", json_path, alcdef_file_name);
      printf("Autonamed path: [%s]\n", autonamed_json_file_path);
      AlcdefFileToJsonFile(alcdef_path, autonamed_json_file_path, flat_mode);
      
    } else {
      // 4. File to File (Single ALCDEF file into a single JSON file)
      AlcdefFileToJsonFile(alcdef_path, json_path, flat_mode);
    }
  }
  
  return false;
}