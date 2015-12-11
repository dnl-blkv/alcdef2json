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
#include <string.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "alcdef.h"
#include "alcdef_to_json.h"

typedef enum PathType {
  kDirectoryPath,
  kFilePath,
  kOtherPath,
  kWrongPath
} PathType;

// Gets the type of a given path
PathType GetPathType (const char *path) {
  struct stat s;
  if (stat(path, &s) == 0) {
      if (s.st_mode & S_IFDIR) {
          return kDirectoryPath;
      } else if (s.st_mode & S_IFREG) {
          return kFilePath;
      } else {
          return kOtherPath;
      }
  } else {
      return kWrongPath;
  }
}

// Converts A single ALCDEF directory to a single JSON file
bool ConvertAlcdefDirToJsonFile (const char *alcdef_path, const char *json_path, const bool flat_mode) {
  // The Win32 variables have names in mixed case due to the naming standard
  // used in windows.h library
  WIN32_FIND_DATA fdFile;
  HANDLE hFind = NULL;

  bool first_file_processed = false;
  
  // Define the output file
  FILE *output = NULL;
  output = fopen(json_path, "w");
  
  if (!output) {
    printf("OUTPUT FILE ERROR\n");
    return true;
  }

  char full_source_path[MAX_PATH_LENGTH];

  // Specify a file mask. *.* = We want everything!
  sprintf(full_source_path, "%s/*.*", alcdef_path);

  printf("Output set to: %s\n", json_path);
  
  // Check if path is valid
    if ((hFind = FindFirstFile(full_source_path, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path not found: [%s]\n", alcdef_path);
        return true;
    }

  // OPEN JSON ARRAY
  fprintf(output, "[");

    do
    {
      // Find first file will always return "."
      //    and ".." as the first two directories.
      if (strcmp(fdFile.cFileName, ".") != 0 && 
          strcmp(fdFile.cFileName, "..") != 0)
      {

        // Generate the next file path
        sprintf(full_source_path, "%s/%s", alcdef_path, fdFile.cFileName);

        // Is the entity a File or Folder?
        if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    
          printf("Directory, ignoring! :)");
    
        } else {
    
          // Write the next file to the target
          //                printf("Writing next file: %s\n", sPath);
          // With no upfront reading, precise result is not reachable

          if (first_file_processed) {
            fprintf(output, ",\n");
          } else {
            first_file_processed = true;
          }
          
          // Open the input file
          FILE *input = fopen(full_source_path, "r");

          // If the next file was failed to open, report error
          if (!input) {
          printf("INPUT FILE ERROR! Path: %s\n", full_source_path);
            // Wait for any input key
            getchar();
            return true;
          }

          AlcdefToJson(output, input, flat_mode);
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

// Converts ALCDEF file(s) to JSON file(s)
bool ConvertAllAlcdefToJson (const char *alcdef_path, const char *json_path, const bool flat_mode)
{
  // TODO FOR GITHUB:
  // 4. File to Directory (split file into separate observations?)
  
  // Get ALCDEF path
  PathType alcdef_path_type = GetPathType(alcdef_path);
  
  if ((alcdef_path_type == kOtherPath) || (alcdef_path_type == kWrongPath)) {
    printf("ERROR: wrong ALCDEF path discovered! Path type: %d\n", alcdef_path_type);
    return true;
  }
  
  // Get JSON path
  PathType json_path_type = GetPathType(json_path);

  if ((json_path_type == kOtherPath) || (json_path_type == kWrongPath)) {
    printf("ERROR: wrong JSON path discovered! Path type: %d\n", json_path_type);
    return true;
  }
  
  if (alcdef_path_type == kDirectoryPath) {
    if (json_path_type == kDirectoryPath) {
      // 1. Directory to Directory (ALCDEF files directory into JSON files directory)
    } else {
      // 2. Directory to File (all ALCDEF into a single JSON)
      ConvertAlcdefDirToJsonFile(alcdef_path, json_path, flat_mode);
    }
  } else {
    // 3. File to File (Single ALCDEF file into a single JSON file)
  }
  
  return false;
}

int main(int argc, char *argv[]) {
  
  // Save the execution starting time
  unsigned long int startTime = (unsigned long)time(NULL);
  
  char fromPath[MAX_PATH_LENGTH];
  char toPath[MAX_PATH_LENGTH];
  bool flatMode = false;
  
  int i = 0;
  
  // Use case: alcdef2json --from C://alcdef2json/alcdef/ --to C://alcdef2json/json/alcdefs.json --flat
  if (argc > 1) {
    for (i = 1; i < argc; i ++) {
      if (strcmp("--from", argv[i]) == 0) {
        // Save alcdef path
        if (i < (argc - 1)) {
          strcpy(fromPath, argv[i + 1]);
        }
      } else if (strcmp("--to", argv[i]) == 0) {
        // Save json path
        if (i < (argc - 1)) {
          strcpy(toPath, argv[i + 1]);
        }
      } else if (strcmp("--flat", argv[i]) == 0) {
        // Save the output mode
        flatMode = true;
      }
    }
  }

  ConvertAllAlcdefToJson(fromPath, toPath, flatMode);

  // Output the time elapsed for the convertion
  printf("Time Elapsed: %lu seconds.\n", (unsigned long)time(NULL) - startTime);
  
  // Return false
  return false;
}