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

STAN's own Eigen plugin to be able to reuse the API actually gets rid of vectorization:
```
L3178:
# /usr/local/include/eigen3/Eigen/src/Core/CoreEvaluators.h:543:     return m_functor(m_argImpl.coeff(row, col));
    movq    (%rax), %rdx    # _374->vi_, _374->vi_
# /usr/local/include/eigen3/Eigen/src/Core/Redux.h:195:     for(Index i = 1; i < mat.innerSize(); ++i)
    addq    $8, %rax    #, ivtmp.12333
# /usr/local/include/eigen3/Eigen/src/Core/functors/BinaryFunctors.h:42:   EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE const result_type operator() (const LhsScalar& a, const RhsScalar& b) const { return a +  b; }
    vaddsd  8(%rdx), %xmm0, %xmm0   # MEM[(const double &)SR.7596_375 + 8], res, res
# /usr/local/include/eigen3/Eigen/src/Core/Redux.h:195:     for(Index i = 1; i < mat.innerSize(); ++i)
    cmpq    %rcx, %rax  # _1330, ivtmp.12333
    jne L3178   #,
```
This is the assembly for evaluating `sum(v)` where `v` is `Eigen::Matrix<vari, Dynamic, 1>` (vector).
The plugin defines a way to view just the value part of a structure containing both value and adjoint.
This heterogenous structure of (value, adjoint) pairs forbids reading the values in a contiguous manner,
which is crucial for vectorization to work properly.
Likewise, reading the adjoints is also not done in a contiguous manner, and hence back-evaluation cannot be vectorized.
We see that the instruction used to increment `res` is `vaddsd`,
which is an AVX instruction to add scalar double-precision values
(note that the prefix `v` does not mean vectorization but rather just indicates that it is an `AVX` instruction (cite Intel?)).
Hence, while AVX instruction is used, addition is _not_ done in parallel.

Compare this with FastAD assembly (snippet):
```
 L9109:
 # /usr/local/include/eigen3/Eigen/src/Core/functors/AssignmentFunctors.h:49:   EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE void assignCoeff(DstScalar& a, const SrcScalar& b) const { a += b; }
     vaddpd  (%rax), %ymm1, %ymm2    # MEM[base: _1374, offset: 0], tmp2317, vect__1661.15503
     addq    $32, %rax   #, ivtmp.15611
     vmovupd %ymm2, -32(%rax)    # vect__1661.15503, MEM[base: _1374, offset: 0]
     cmpq    %r12, %rax  # _2712, ivtmp.15611
     jne L9109   #,
```
Vectorization was possible and we see that `vaddpd` is being used,
which is an AVX instruction to add _packed_ double-precision values;
packed indicates that multiple values are added in parallel.
As for back-evaluation, fastad is more optimized by design to simply assign the adjoints,
which generally only requires a load and store, rather than increment, 
which generally requires two loads, add, and a store.

For the matrix multiplication, STAN creates temporary contiguous memory and views it as a normal `Eigen::Matrix<double...>`
object for both matrices and computes the matrix multiplication using Eigen's built-in methods.
Hence, in terms of computation speed, it isn't as different as other examples.
Note also that the time to AD this for FastAD vs the time for a single matrix multiplication (baseline) is 3.16.
Since AD for matrix multiplication requires 3 (N x N) multiplications, there is about 16% overhead using FastAD vs. 
manually evaluating the multiplication and also the partial derivatives.
We consider sFastAD is very optimal.

## References
- [ADOL-C Manual](https://github.com/coin-or/ADOL-C/blob/master/ADOL-C/doc/adolc-manual.pdf)
- [STAN math](https://arxiv.org/pdf/1509.07164.pdf)
