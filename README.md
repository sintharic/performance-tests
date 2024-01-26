Collection of small scientific projects comparing different programming languages and impelementations with respect to performance and ease of use.



FFT results
-----------

Benchmark consists of two 2D double precision arrays (resolution N x N) and their Fourier transforms.
A triangular wave in x- and y-direction is generated one wave vector after the other and transformed back and forth each time.

Computation time on MacBook Pro 2019 at N=1024:

- Julia (using Intel MKL): 7.6 s
- Julia (using FFTW): 9.0 s
- C++: 13.2 s
- Python: 16.2 s

Memory usage (according to `top`) on MacBook Pro 2019 at N=1024:

- C++: 50 MB
- Python: ~95 MB (32 MiB according to `tracealloc`)
- Julia (using FFTW): ~160 MB (352 MiB according to `maxrss`???)
- Julia (using Intel MKL): ~170 MB (352 MiB according to `maxrss`???)

Benchmarks involving different resolutions or other computational tasks may follow in the future.



Caveats
-------

Figuring out memory usage appears to be a difficult task, since it does not necessarily take all the linked libraries into account.
While Julia and Python both provide their own internal measurement tools, C++ does not and `valgrind` does not seem to work on Mac.
With zero overhead, given that a `double` is 8 byte and a `complex<double>` is 16 byte, I would calculate an ideal memory usage of `2*8*1024*1024 + 2*16*1024*513` = 33.6 MiB, i.e. just above 2^25 MiB.
Python's internal estimate was somehow below that value.
And the estimate reported by my tracker function in Julia was an entire order of magnitude above... 



Summary
-------

- Julia's speed is very impressive.
- Julia's 1-based indexing is very inconvenient.
- The amount of C++ boilerplate necessary to perform such easy tasks makes me very sad.
- I need to figure out better ways to measure memory consumption before I continue with these benchmarks.