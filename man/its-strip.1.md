% its-strip(1) its-strip 1.0.0
% Written by Kjetil Kristoffer Solberg
% December 2020

# NAME
its-strip - strips character or columns from file output

# SYNOPSIS
**its-strip** [*OPTION*] [*FILEIN*] [*FILEOUT*]

# DESCRIPTION
**its-strip** removes characters or columns from file output

# OPTIONS
**-?**, **--help**
: Shows the default help screen

**-h**, **--hex**
: Strip all these hex values away

**-d**, **--des**
: Strip all these numeric character values away

**-c**, **--chars**
: Strip all these character values away

**-l**, **--columns**
: Strips column indexes (1-based) from file output. Cannot be combined with other options

**-i**, **--inverse**
: Strips away the inverse of set option from file output. Not woring with -l or --columns

# EXAMPLES
ls -al | **its-strip** -h "0D 30"
: Strips all character with hex values 0x0D and 0x48 from ls output

**its-strip** -h "0D 48" < test.txt
: Strips all character with hex values 0x0D and 0x48 from test.txt

ls -al | **its-strip** -d "13 48"
: Strips all characters with numeric values 13 and 48 from ls output

**its-strip** -d "13 48" < test.txt
: Strips all characters with numeric values 13 and 48 from test.txt

ls -al | **its-strip** -c "abc"
: Strips all characters from "abc" character array from ls output

**its-strip** -c "abc" < test.txt
: Strips all characters from "abc" character array from test.txt

ls -al | **its-strip** -l "1 2 3"
: Strips away column index 1, 2 and 3 from ls output

**its-strip** -l "1 2 3" < test.txt
: Strips away column index 1, 2 and 3 from test.txt

ls -al | **its-strip** -i -c "abc"
: Strips away all characters not in "abc" character array from ls output

**its-strip** -i -c "abc" < test.txt
: Strips away all characters not in "abc" character arrya from test.txt

# BUGS
All software has bugs :)

# COPYRIGHT
License GPL-3.0-or-later. This is free software: you are free to change and redistribute it. There is NO WARRENTY, to the extent permitted by law.
