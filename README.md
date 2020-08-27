# AD Benchmark

This is a repository dedicated to benchmarking automatic differentiation libraries for research purposes.

## Installation

First clone the repository:
```
git clone https://github.com/JamesYang007/ADBenchmark.git
```

Then run the setup script to install all dependencies (and their dependencies):
- [Adept2.0.8](http://www.met.reading.ac.uk/clouds/adept/)
- [FastAD3](https://github.com/JamesYang007/FastAD)
- [STAN math](https://github.com/stan-dev/math)

```
./setup.sh
```

We have not provided scripts for the following changes and we ask the users to manually make these changes:
- Add `math::` in front of `apply` in `lib/stan-dev-math/stan/math/rev/functor/adj_jac_apply.hpp` line `513`.
  Compiling with C++17 standard raises error due to ambiguity with `std::apply`.

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
Currently, the header (first line of output) will be the following:
```
N,adept,stan,fastad,double
```

## References
- [STAN math](https://arxiv.org/pdf/1509.07164.pdf)
