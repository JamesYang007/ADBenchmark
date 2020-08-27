#!/bin/bash

mkdir -p lib
cd lib

adept="adept-2.0.8"
fastad="FastAD"

# setup Adept2.0.8
if [ ! -d "$adept" ]; then
    wget http://www.met.reading.ac.uk/clouds/adept/adept-2.0.8.tar.gz
    tar xvfz adept-2.0.8.tar.gz
    rm -rf adept-2.0.8.tar.gz
    cd adept-2.0.8
    mkdir -p build
    ./configure CXXFLAGS="-O3 -march=native" --prefix=$PWD/build --exec_prefix=$PWD/build
    make
    make install
    cd ../ # back in lib
fi

# setup FastAD3
if [ ! -d "$fastad" ]; then
    git clone https://github.com/JamesYang007/$fastad.git
    cd $fastad
    ./clean-build.sh release -DFASTAD_ENABLE_TEST=OFF \
        -DCMAKE_INSTALL_PREFIX=".." # installs into build
    cd build/release
    ninja install
    cd ../../.. # back in lib
fi
