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
#include "alcdef2json.h"

int main(int argc, char *argv[]) {
	
	// Save the execution starting time
	unsigned long int start_time = (unsigned long)time(NULL);
	
	endpoint input, output;
	
	int i = 0;
	
	bool nested_mode = true;
	
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
			} else if (strcmp("--flatMode", argv[i]) == 0) {
				// Save the output mode
				nested_mode = false;
			}
		}
	}
	
	// Use case: ALCDEF_to_JSON --fromDir C://alcdef2json/alcdef --toFile C://alcdef2json/json/alcdefs.json
	// Add support to output mode
	convert_all(input.path, output.path, nested_mode);

	// Output the time elapsed for the convertion
	printf("Time Elapsed: %lu seconds.\n", (unsigned long)time(NULL) - start_time);
	
	// Return false
	return false;
}