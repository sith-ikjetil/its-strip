#!/bin/bash
echo "Building its-strip ..."
echo "> using release build < "

g++ its-strip.cpp -o its-strip
if [[ $? -eq 0 ]]
then
    echo "> its-strip build Ok"
else
    echo "> its-strip build Error"
fi

echo "> Build Process Complete <"