#!/bin/bash

fileName='my_file.txt'
chars=aoeusnthSNTHAOEU1234567890lrcgLRGCFYPfpy
for i in {1..125000} ; do
    echo -n "${chars:RANDOM%${#chars}:1}" >> $fileName
done
for i in {1..10} ; do
if (($i%2))
then
    cat $fileName >> ${fileName}.t
else
    cat ${fileName}.t >> $fileName
fi
done