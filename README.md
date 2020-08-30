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
or use the prepared `run.sh` shell script:
```
./run.sh
```
which will setup, build, run benchmark, and analyze.

## Notes

### ADOL-C

They define what's called a "tape" which records data needed to perform AD.
There is a way of having multiple tapes and recording on any one of them.
User can choose which variables are independent, 
i.e. the variables we should differentiate w.r.t., by using `operator<<=` and `operator>>=`.
It is CRUCIAL to use `operator<<=` to indicate which variables are independent.
They don't seem to have an equivalent `VarView` structure to simply view values,
so this part should part of the benchmark.

### STAN

Memory management is based on overloading `operator new`.
There are three "stacks": 
one for storing the actual variable objects `vari`,
one for storing "any object needed during autodiff and needs destructing",
and one for storing the operations.
FastAD is much more memory efficient since it only needs 
the memory for value and adjoints (no need to save any other objects or operations).
Based on looking at STAN's implementation and reading their paper,
they allocate a pointer to a (univariate) `vari` for each such object.
Hence, for matrix objects of size M x N, they allocate M x N chunks of bytes for value and adjoint 
and also M x N pointers.
FastAD rather only allocates M x N chunks of bytes for value and adjoint for intermediate expression graph, 
which is unavoidable, and only allocates 1 pointer implicitly through `Eigen::Map` to view this entire region.
In general, we allocate about less than 2/3 the amount allocated by STAN, which is significant,
since pointers are 8 bytes and if value and adjoint are both 8 bytes each, STAN allocates 8 * 3 * number of elements
and we only require allocations for value and adjoint, hence 8 * 2 * number of elements.
In fact, some expression nodes are optimized to not allocate adjoints at all.
All expressions that are scalar valued do not allocate adj and some special nodes such as `if_else` 
and in certain cases for `pow<n>` (when n == 0, 1) do not allocate adj.

STAN's design choice also explains why matrix multiplication has the problem of creating `N^2`-fold expression
when using the built-in `Eigen` matrix multiplication with objects where `Scalar` type is `vari`.
They have to allocate even more memory for `vari*` on the arena to first assign the operands for both operands,
and also the same amount of bytes for the actual `double` values for the placeholders.
This is extremely costly can be entirely avoided.
Our matrix multiplication does not require any extra allocations except two `Eigen::Map`s,
which are extremely cheap.

## References
- [ADOL-C Manual](https://github.com/coin-or/ADOL-C/blob/master/ADOL-C/doc/adolc-manual.pdf)
- [STAN math](https://arxiv.org/pdf/1509.07164.pdf)
