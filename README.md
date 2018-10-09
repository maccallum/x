# X: Random number generation and distribution for Max/MSP and other environments.

### Features
* Wide variety of distributions and random number generators to choose from
* Distributions, generators, and seeds are composable
* Support for audio-rate generation (MSP)
* [odot](https://github.com/CNMAT/CNMAT-odot) support
* C/C++ library designed to be easily embedded in other environments

### Currently supported environments:
* Max/MSP
* Lisp (SBCL)

### Planned support:
* OpenMusic / LispWorks
* Python
* PD

### Distributions
* benford
* bernoulli
* beta
* binomial
* cauchy
* chi_squared
* dirichlet
* discrete
* erlang
* exponential
* extreme_value
* fisher_f
* gamma
* geometric
* hypergeometric
* kumaraswamy
* laplace
* lognormal
* multinomial
* multivariate_hypergeometric
* negative_binomial
* normal
* piecewise_constant
* piecewise_linear
* poisson
* rayleigh
* student_t
* uniform_int
* uniform_real
* weibull

### RNGs
* knuth_b
* minstd_rand
* minstd_rand0
* mt19937
* mt19937_64
* pcg32
* pcg64
* ranlux24
* ranlux48
