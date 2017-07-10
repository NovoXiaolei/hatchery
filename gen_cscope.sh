#/usr/bin/bash

find . -name "*.h" -o -name "*.c" > cscope.files
cscope -Rbq -i cscope.files 
