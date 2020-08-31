#!/bin/bash

NAME="$1"

# directory where current shell script resides
PROJECTDIR=$(dirname "$BASH_SOURCE")

cd "$PROJECTDIR"

CXX_DEFINES="-DADEPT_STACK_THREAD_UNSAFE
             -DEIGEN_ARRAYBASE_PLUGIN=\"stan/math/prim/eigen_plugins.h\"
             -DEIGEN_MATRIXBASE_PLUGIN=\"stan/math/prim/eigen_plugins.h\"
             -DEIGEN_NO_DEBUG
             -DNDEBUG
             -D_REENTRANT"

CXX_INCLUDES="-I$PROJECTDIR/benchmark
              -I$PROJECTDIR/lib/adept-2.0.8/build/include
              -I$PROJECTDIR/lib/adolc/build/include
              -I$PROJECTDIR/lib/cppad/build/include
              -I$PROJECTDIR/lib/Trilinos/build/include
              -I$PROJECTDIR/lib/stan-dev-math
              -I$PROJECTDIR/lib/stan-dev-math/lib/tbb_2019_U8/include
              -I$PROJECTDIR/lib/stan-dev-math/lib/boost_1.72.0
              -I$PROJECTDIR/lib/stan-dev-math/lib/sundials_5.2.0/include
              -isystem $PROJECTDIR/lib/FastAD/build/include
              -isystem /usr/local/include/eigen3"

CXX_FLAGS="-O3
           -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk
           -march=native
           -Wno-macro-redefined
           -S
           -fverbose-asm
           -std=gnu++17"

COMMAND="g++-10 $CXX_DEFINES $CXX_FLAGS $CXX_INCLUDES $PROJECTDIR/benchmark/main.cpp -o build/benchmark/$NAME"
echo $COMMAND
$COMMAND
