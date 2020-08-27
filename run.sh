#!/bin/bash

PROJECTDIR=$(dirname "$BASH_SOURCE")

cd "$PROJECTDIR"

./setup.sh
cd build
make
cd benchmark
./main
cd ../../analyze
python3 analyze.py
cd ..
