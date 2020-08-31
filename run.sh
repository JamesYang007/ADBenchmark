#!/bin/bash

PROJECTDIR=$(dirname "$BASH_SOURCE")

cd "$PROJECTDIR"

N="$1"

./setup.sh
cd build
make
cd benchmark
./main $1
cd ../../analyze
python3 analyze.py
cd ..
