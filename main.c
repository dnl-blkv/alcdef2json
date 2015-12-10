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
#include "alcdef_to_json.h"

// Converts a directory of ALCDEF files to JSON
bool AlcdefDirToJson (const char *source_path, const char *destination_path, const bool flat_mode)
{
	// The Win32 variables have names in mixed case due to the naming standard
	// used in windows.h library
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

	bool first_file_processed = false;
	
	// Define the output file
	FILE *output = NULL;
	output = fopen(destination_path, "w");
	
	if (!output) {
		printf("OUTPUT FILE ERROR\n");
		return true;
	}

    char full_source_path[MAX_PATH_LENGTH];

    // Specify a file mask. *.* = We want everything!
    sprintf(full_source_path, "%s/*.*", source_path);

	printf("Output set to: %s\n", destination_path);
	
	// Check if path is valid
    if ((hFind = FindFirstFile(full_source_path, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path not found: [%s]\n", source_path);
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
            sprintf(full_source_path, "%s/%s", source_path, fdFile.cFileName);

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

				AlcdefToJson(full_source_path, flat_mode, output);
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

int main(int argc, char *argv[]) {
	
	// Save the execution starting time
	unsigned long int startTime = (unsigned long)time(NULL);
	
	Endpoint input, output;
	
	int i = 0;
	
	bool flatMode = false;
	
	// TODO:
	// --fromFile
	// --toDir
	if (argc > 1) {
		for (i = 1; i < argc; i ++) {
			if (strcmp("--fromDir", argv[i]) == 0) {
				// Save input directory path
				if (i < (argc - 1)) {
					strcpy(input.path, argv[i + 1]);
					input.isFile = false;
				}
			} else if (strcmp("--fromFile", argv[i]) == 0) {
				// TODO TODO TODO
				// Save input directory path
				if (i < (argc - 1)) {
					strcpy(input.path, argv[i + 1]);
					input.isFile = true;
				}
			} else if (strcmp("--toDir", argv[i]) == 0) {
				// TODO TODO TODO
				// Save output file path
				if (i < (argc - 1)) {
					strcpy(output.path, argv[i + 1]);
					output.isFile = false;
				}
			} else if (strcmp("--toFile", argv[i]) == 0) {
				// Save output file path
				if (i < (argc - 1)) {
					strcpy(output.path, argv[i + 1]);
					output.isFile = true;
				}
			} else if (strcmp("--flat", argv[i]) == 0) {
				// Save the output mode
				flatMode = true;
			}
		}
	}
	
	// Use case: ALCDEF_to_JSON --fromDir C://alcdef2json/alcdef --toFile C://alcdef2json/json/alcdefs.json
	// Add support to output mode
	AlcdefDirToJson(input.path, output.path, flatMode);

	// Output the time elapsed for the convertion
	printf("Time Elapsed: %lu seconds.\n", (unsigned long)time(NULL) - startTime);
	
	// Return false
	return false;
}