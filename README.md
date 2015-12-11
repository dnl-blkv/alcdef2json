The ALCDEF 2.0 to JSON Converter is a tool for conversion astronomic data in 
ALCDEF 2.0 format to more popular and generic JSON data interchange format.

- ALCDEF format: http://minorplanetcenter.net/light_curve
- JSON format: http://json.org/

The main conversion points are as follows:
- All JSON keys are in lower case.
- Boolean strings from ALCDEF are represented as booleans in JSON.
- String values from ALCDEF are escaped in JSON; commas are replaced with dots.
- Doubles including the Julian Date are represented as doubles.
- In the flat mode, data entries are suffixed with their index numbers.
- In the nested mode, data entries are put into arrays, each element of such 
  arrays being an object with subfields of data represented as key-value pairs.
  Consider the lightcurve data block from the previous list item converted in 
  the nested mode:
  
Flat Mode vs. Nested Mode
=========================

The converter allows for conversion in the two modes: flat and nested, as 
described in the list above. Below is a brief comparison of the examples
of usage of the two.

Consider a lightcurve data block:

```
DATA=2455817.735937|+7.328|+0.004|2.224
DATA=2455817.736235|+7.317|+0.004|2.219
DATA=2455817.736529|+7.317|+0.004|2.214
ENDDATA
```

The same data block after the flat-mode conversion to JSON is as follows:

```
{
  ...,
  "jd1":2455817.735937,
  "mag1":+7.328,
  "magerr1":+0.004,
  "airmass1":2.224,
  "jd2":2455817.736235,
  "mag2":+7.317,
  "magerr2":+0.004,
  "airmass2":2.219,
  "jd3":2455817.736529,
  "mag3":+7.317,
  "magerr3":+0.004,
  "airmass3":2.214,
  ...
}
```

The same data block after the nested-mode conversion, on the other hand,
looks as follows:

```
{
  ...,
  "data":[
  {"jd":2455817.735937,"mag":+7.328,"magerr":+0.004,"airmass":2.224},
  {"jd":2455817.736235,"mag":+7.317,"magerr":+0.004,"airmass":2.219},
  {"jd":2455817.736529,"mag":+7.317,"magerr":+0.004,"airmass":2.214}
  ],
  ...
}
```  

Flags
=====
The program currently accepts the following flags:
- --from - **required**, with a single argument; the argument defines the 
  address of the source ALCDEF file, or directory with such files.
- --to - **required**, with a single argument; the argument defines the address
  of the resulting JSON file, or directory for such files; the output files
  (but not directories!) are created automatically if not exist.
- --flat - **optional**, with no arguments; defines the flat operation mode 
  (the choices are *flat* and *nested*); default choice is nested

Example use cases
=================
There are four unique use cases, each of them could optionally use the --flat 
flag. The cases are as follows:
- From directory to directory, for example:

```
alcdef2json --from C://alcdef --to C://json --flat
```

In this case, all the files from the ALCDEF directory will be converted 
one-to-one to JSON files and located to the JSON directory under the 
original names with but with ```.json``` extension suffixes. For instance, 
```alcdef001.txt``` will be turned into ```alcdef001.txt.json```. Notice
the ```--flat``` flag denoting the flat mode.

- From directory to file, for example:

```
alcdef2json --from C://alcdef --to C://json/alcdefs.json
```

In this case, all the files from the ALCDEF directory will be converted many-to-one
and written to a specified JSON files as a single JSON array of ligthcurve 
blocks. No original file borders will be shown in the resulting array. Absence of
```--flat``` flag will lead to the nested mode conversion.

- From file to directory, for example:

```
alcdef2json --from C://alcdef/ALCDEF_1_Ceres_20151117_171441 --to C://json
```

In this case, a one-to-one conversion will occur, and the resulting JSON file will be
named similarly to the source ALCDEF file, but with ```.json``` extension suffix.
Absence of ```--flat``` flag will lead to the nested mode conversion.

- From file to file, for example:

```
alcdef2json --from C://alcdef/ALCDEF_1_Ceres_20151117_171441 --to C://json/alcdefs.json
```

In this case, a plain one-to-one conversion will occur. The source ALCDEF file will
be converted to JSON and saved under the path specified after the ```--to``` flag.
Absence of ```--flat``` flag will lead to the nested mode conversion.
