# AD Benchmark

This is a repository dedicated to benchmarking automatic differentiation libraries for research purposes.
This benchmark has only been tested on MacOS Catalina.

## Installation

First clone the repository:
```
git clone https://github.com/JamesYang007/ADBenchmark.git
```

Then run the setup script to install all dependencies (and their dependencies):
- [Adept 2.0.8](http://www.met.reading.ac.uk/clouds/adept/)
- [ADOL-C 2.7.2](https://github.com/coin-or/ADOL-C)
- [CppAD 20200000](https://coin-or.github.io/CppAD/doc/cppad.htm)
- [Sacado 13.0.0](https://github.com/trilinos/Trilinos/tree/master/packages/sacado)
- [FastAD 3](https://github.com/JamesYang007/FastAD)
- [STAN math 3.3.0](https://github.com/stan-dev/math)
```
./setup.sh
```
Note that the setup will use GCC10.
If this is not available, you have to manually change the compiler version to the one that is available
by editing `setup.sh`.
If you must run the setup again for a particular library, 
delete the folder associated with that library in `lib` and run the script again.

We have not provided scripts for the following changes and we ask the users to manually make these changes:
- Add `math::` in front of `apply` in `lib/stan-dev-math/stan/math/rev/functor/adj_jac_apply.hpp` line `513`.
  Compiling with C++17 standard raises error due to ambiguity with `std::apply`.
- Open `benchmark/main.cpp` and comment out all tests except one that you want to test.
  The `std::unordered_map` object stores the configuration for the run.
  The "value" is a struct that contains the name, boolean indicating whether to run this library or not,
  and the number of iterations it should run to average out the time.
  Feel free to change any of these settings.

To build and run benchmarks:
```
./clean-build.sh -DCMAKE_C_COMPILER=gcc-10 -DCMAKE_CXX_COMPILER=g++-10
cd build/benchmark
./main
```
If GCC10 is not available, just replace `gcc-10` and `g++-10` with the correct alias for the available version
(e.g. `gcc-8` and `g++-8` if GCC8 is available).

Running `./main` should generate `csv` files in the current directory for various tests.
The format is the following:
```
N,libname1,libname2,...,libnameK
n,t1,t2,...,tK
...
```
where `N` represents the size of the input vector (ranging from `2^[0,...,14]`),
`libnameI` represents the `I`th library,
`n` is a particular size of the input vector,
and `tI` is the average elapsed time for testing with `n` and `libnameI`.
The header (first line of output) will be something like the following:
```
N,adept,adolc,cppad,sacado,stan,fastad,double
```

We wrote a Python script in `analyze` called `analyze.py` that 
scrapes `build/benchmark` directory for all `_eval.csv` files,
plots the results, and saves the plots in `docs/figs`.
The user can directly follow these commands:
```
cd analyze
python3 analyze.py
```

To both build, run the benchmark program, and create the plots,
use the prepared `run.sh` shell script:
```
./run.sh
```
