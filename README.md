The ALCDEF to JSON Converter is a tool created for conversion of ALCDEF format to JSON.

- ALCDEF format: http://minorplanetcenter.net/light_curve
- JSON format: http://json.org/

Flags
=====
The program currently accepts the following flags:
* --fromDir - **required**, stands for the address of the directory with input ALCDEF files
* --toFile - **required**, stands for the address of an output file (created automatically if does not exist)
* --flatMode - **optional**, if not used, the nested mode is applied

Use case
========
alcdef2json --fromDir C://alcdef2json/alcdef --toFile C://alcdef2json/json/alcdefs.json --flatMode