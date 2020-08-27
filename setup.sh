#!/bin/bash

fastadpath="lib/FastAD"

# setup FastAD
if [ ! -d "$fastadpath" ]; then
    git clone https://github.com/JamesYang007/FastAD.git $fastadpath
    cd $fastadpath
    ./clean-build.sh release -DFASTAD_ENABLE_TEST=OFF \
        -DCMAKE_INSTALL_PREFIX=".." # installs into build
    cd build/release
    ninja install
    cd ../../ # in lib/FastAD
    cd ../../ # in working directory
fi
