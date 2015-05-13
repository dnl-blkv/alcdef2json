The ALCDEF to JSON Converter is a tool created for conversion of ALCDEF format to JSON.

- ALCDEF format: http://minorplanetcenter.net/light_curve
- JSON format: http://json.org/
  - The keys are in lower case
  - Boolean strings from alcdef represented as booleans (lower case in JSON)
  - Strings are plain copied with commas replaced with dots and double quotes escaped
  - Doubles including the Julian Date represented as doubles
  - In flat mode, data entries are suffixed with their index numbers
  - In nested mode, data entries are put into an array, where each entry is an object with data's sub-fields as properties

Flags
=====
The program currently accepts the following flags:
* --fromFile (upcoming) or --fromDir - **required**, stands for the address of the directory with input ALCDEF files
* --toFile or --toDir (upcoming) - **required**, stands for the address of an output file (created automatically if does not exist)
* --flatMode - **optional**, if not used, the nested mode is applied

Example use case
========
```
alcdef2json --fromDir C://alcdef2json/alcdef --toFile C://alcdef2json/json/alcdefs.json --flatMode
```
