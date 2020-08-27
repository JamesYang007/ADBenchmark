# directory where current shell script resides
PROJECTDIR=$(dirname "$BASH_SOURCE")

cd "$PROJECTDIR"

mkdir -p build && cd build
rm -rf *
cmake ../ "$@"
cmake --build . -- -j12
