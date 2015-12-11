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
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "win32_converter.h"

// Main
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

  ConvertAlcdefToJson(fromPath, toPath, flatMode);

  // Output the time elapsed for the convertion
  printf("Time Elapsed: %lu seconds.\n", (unsigned long)time(NULL) - startTime);
  
  // Return false
  return false;
}