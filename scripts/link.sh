#!/bin/bash
if [ -e file_list.txt ]; then
    rm file_list.txt
fi
### set number of channels
numchannel=32
pathtodata='/home/adam/data/molly'
### set the soft links to the raw data files
ls -rth $pathtodata/*.dat | tail -$numchannel > file_list.txt
awk -F \- '{system("ln -sf " $0 " data_in"$7)}' file_list.txt



