# its-strip
This Linux filter is created simply to get some experience creating a filter. This is in
no way a best-practise filter example. It just turned out this way.

```
its-strip v.1.0 (amd64)
Usage: its-strip [OPTION]...
   or: its-strip [OPTION]... [FILE IN] [FILE OUT]

With no FILE, read standard input

    options                explanations                         
 --------------       ------------------------------------------
  -?, --help            Shows this help screen

  -h, --hex             Strip all these hex values away.
                        ::Example: its-strip -h "0D 48"
  -d, --des             Strip all these numeric values away.
                        ::Example: its-strip -d "13 48"
  -c, --chars           Strip all these chars away.
                        ::Example: its-strip -c "abcdef"
  -l, --columns (*)     Strip columns separated by space.
                        ::Example: its-strip -l "1 4 5"
  -i, --inverse         Inverses strip. NOT COLUMNS!

  *Does not work with other options!
```
