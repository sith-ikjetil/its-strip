#!/bin/bash
#: Title       : build-debug.sh
#: Date        : 2022-07-23
#: Author      : Kjetil Kristoffer Solberg <post@ikjetil.no>
#: Version     : 1.0
#: Description : Builds ccat.
echo "Building its-strip ..."
echo "> using debug build < "

g++ its-strip.cpp -o its-strip -ggdb
if [[ $? -eq 0 ]]
then
    echo "> its-strip build ok"
else
    echo "> its-strip build error"
fi

echo "> build process complete <"