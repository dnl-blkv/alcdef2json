The ALCDEF 2.0 to JSON Converter is a tool for conversion astronomic data in 
ALCDEF 2.0 format to more popular and generic JSON data interchange format.

- ALCDEF format: http://minorplanetcenter.net/light_curve
- JSON format: http://json.org/

The main conversion points are as follows:
- All JSON keys are in lower case
- Boolean strings from ALCDEF are represented as booleans in JSON
- String values from ALCDEF are escaped in JSON; commas are replaced with dots
- Doubles including the Julian Date are represented as doubles
- In the flat mode, data entries are suffixed with their index numbers
- In the nested mode, data entries are put into arrays, each element of such 
  arrays being an object with subfields of data represented as key-value pairs

Flags
=====
The program currently accepts the following flags:
- --from - **required**, with a single argument; the argument defines the 
  address of the input ALCDEF file, or directory with such files
- --to - **required**, with a single argument; the argument defines the address
  of the output JSON file, or directory for such files; the output file(s) 
  is/are created automatically if not exist
- --flat - **optional**, with no arguments; defines the flat operation mode 
  (the choices are *flat* and *nested*); default choice is nested

Example use case
========
```
alcdef2json --from C://alcdef --to C://json/alcdefs.json --flat
```
