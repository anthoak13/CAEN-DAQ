#!/bin/bash

saveLoc="/home/adam/Documents/trees/"

filename=$(basename `readlink -f data_in0`)
filename="${filename%.*}"

echo "Saving tree: $filename.root"
cp -f macro.root $saveLoc/$filename.root
