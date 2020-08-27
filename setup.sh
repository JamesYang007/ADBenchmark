#!/bin/bash

mkdir -p lib
cd lib

adept="adept-2.0.8"
adolc="adolc"
cppad="cppad"
sacado="Trilinos"
stan="stan-dev-math"
fastad="FastAD"

# setup Adept2.0.8
if [ ! -d "$adept" ]; then
    wget http://www.met.reading.ac.uk/clouds/adept/$adept.tar.gz
    tar xvfz $adept.tar.gz
    rm -rf $adept.tar.gz
    cd $adept
    mkdir -p build
    ./configure CXXFLAGS="-O3 -march=native" --prefix=$PWD/build --exec_prefix=$PWD/build
    make
    make install
    cd ../ # back in lib
fi

# setup ADOL-C
if [ ! -d "$adolc" ]; then
    git clone --depth 1 --branch releases/2.7.2 https://github.com/coin-or/ADOL-C.git $adolc
    cd $adolc
    mkdir -p build
    autoreconf -fi
    CC=gcc-10 CXX=g++-10 ./configure \
        --prefix=$PWD/build \
        --exec_prefix=$PWD/build \
        --with-cflags="-O3 -march=native" \
        --with-cxxflags="-O3 -march=native -std=c++17"
    make -j6
    make install
    cd ../ # back in lib
fi

# setup CppAD
if [ ! -d "$cppad" ]; then
    git clone --depth 1 --branch 20200000.3 https://github.com/coin-or/CppAD.git $cppad
    cd $cppad
    mkdir -p build
    cd build
    cmake .. \
        -DCMAKE_C_COMPILER="gcc-10" \
        -DCMAKE_CXX_COMPILER="g++-10" \
        -Dcppad_prefix="." \
        -DCMAKE_MACOSX_RPATH=1
    make -j6
    make install
    cd ../../ # back in lib
fi

# setup Sacado
if [ ! -d "$sacado" ]; then
    git clone --depth 1 --branch trilinos-release-13-0-0 https://github.com/trilinos/Trilinos.git
    cd Trilinos
    mkdir -p build
    cd build
    cmake \
        -DCMAKE_C_COMPILER="gcc-10" \
        -DCMAKE_CXX_COMPILER="g++-10" \
        -DTrilinos_GENERATE_REPO_VERSION_FILE=OFF \
        -DTrilinos_ENABLE_EXPLICIT_INSTANTIATION=ON \
        -DBUILD_SHARED_LIBS=ON \
        -DTrilinos_ENABLE_Fortran=OFF \
        -DTrilinos_ENABLE_Sacado=ON \
        -DTrilinos_ENABLE_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX="." \
        ..
    make -j6 install
    cd ../../ # back in lib
fi

# setup STAN
if [ ! -d "$stan" ]; then
    git clone --depth 1 --branch v3.3.0 https://github.com/stan-dev/math.git $stan
    cd $stan
    echo "CXX=g++-10" > make/local
    make -j4 -f make/standalone math-libs
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

