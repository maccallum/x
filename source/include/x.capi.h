/*
Copyright (c) 2018 John MacCallum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __X_CAPI_H__
#define __X_CAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <inttypes.h>

struct xobj_uint32;

typedef void (*xobj_uint32_callback)(struct xobj_uint32*, size_t n);

typedef struct xobj_uint32
{
	void *myobj;
	xobj_uint32_callback callback;
	size_t *n;
	uint32_t **buf;
} xobj_uint32;

typedef xobj_uint32 x_random_device;
typedef xobj_uint32 x_seed_seq_from;
typedef xobj_uint32 x_seed_seq_from_delegate;
typedef struct x_rng
{
	xobj_uint32 obj;
	uint64_t min;
	uint64_t max;
} x_rng;

//////////////////////////////////////////////////
// random_device
//////////////////////////////////////////////////
typedef struct random_device random_device;
x_random_device *random_device_new(void);
void random_device_delete(x_random_device *rd);
unsigned int random_device_generate(x_random_device *rd);
unsigned int random_device_min(x_random_device *rd);
unsigned int random_device_max(x_random_device *rd);

//////////////////////////////////////////////////
// seed_seq_from
//////////////////////////////////////////////////
typedef struct seed_seq_from seed_seq_from;
x_seed_seq_from *seed_seq_from_new_with_callback(x_random_device *random_device, xobj_uint32_callback random_device_delegate_callback);
x_seed_seq_from *seed_seq_from_new(x_random_device *random_device);
void seed_seq_from_delete(x_seed_seq_from *ssf);
void seed_seq_from_setcontext(x_seed_seq_from *ssf, void *context);
void seed_seq_from_generate(x_seed_seq_from *_ssf, uint32_t *start, uint32_t *end);

//////////////////////////////////////////////////
// seed_seq_from_delegate
//////////////////////////////////////////////////
typedef struct seed_seq_from_delegate seed_seq_from_delegate;
x_seed_seq_from_delegate *seed_seq_from_delegate_new_with_callback(x_seed_seq_from *seed_seq_from, xobj_uint32_callback seed_seq_from_delegate_callback);
x_seed_seq_from_delegate *seed_seq_from_delegate_new(x_seed_seq_from *seed_seq_from);
void seed_seq_from_delegate_delete(x_seed_seq_from_delegate *ssfd);
void seed_seq_from_delegate_setcontext(x_seed_seq_from_delegate *ssfd, void *context);

//////////////////////////////////////////////////
// rngs
//////////////////////////////////////////////////
typedef struct rng_pcg32 rng_pcg32;
x_rng *rng_pcg32_new(x_seed_seq_from_delegate *ssfd);
void rng_pcg32_delete(x_rng *r);
uint32_t rng_pcg32_min(void);
uint32_t rng_pcg32_max(void);
uint32_t rng_pcg32_generate(x_rng *r);
uint64_t rng_min(x_rng *r);
uint64_t rng_max(x_rng *r);

//////////////////////////////////////////////////
// distributions
//////////////////////////////////////////////////
//typedef struct dist_gamma dist_gamma;
//dist_gamma *dist_gamma_new(void);
//void dist_gamma_delete(dist_gamma *d);
//double dist_gamma_generate(void *context, xobj_uint32_callback callback, double alpha, double beta, uint64_t rng_min, uint64_t rng_max);

//////////////////////////////////////////////////
// these macros produce declarations of the form
// <dist_ret_type> dist_<dist>_generate_with_callback(x_rng *rng,
//                                                    xobj_uint32_callback rng_delegate_callback,
//                                                    uint64_t rng_min,
//                                                    uint64_t rng_max,
//						      <arg1_type> <arg1_name>,
//						      ...);
//
// and
//
// <dist_ret_type> dist_<dist>_generate(x_rng *rng,
//                                      uint64_t rng_min,
//                                      uint64_t rng_max,
//					<arg1_type> <arg1_name>,
//					...);
//
// for example
//
// DIST_GENERATE_WITH_CALLBACK_DECL(gamma, double, double alpha, double beta);
// DIST_GENERATE_DECL(gamma, double, double alpha, double beta);
//
// produce
//
// double dist_gamma_generate_with_callback(x_rng *rng,
//					    xobj_uint32_callback rng_delegate_callback,
//					    uint64_t rng_min,
//					    uint64_t rng_max,
//					    double alpha,
//					    double beta);
//
// double dist_gamma_generate(x_rng *rng,
//			      uint64_t rng_min,
//			      uint64_t rng_max,
//			      double alpha,
//			      double beta);

#define DIST_GENERATE_WITH_CALLBACK_DECL(dist, dist_ret_type, ...) \
	dist_ret_type dist_##dist##_generate_with_callback(x_rng *rng,	\
							   xobj_uint32_callback rng_delegate_callback, \
							   __VA_ARGS__)

#define DIST_GENERATE_DECL(dist, dist_ret_type, ...)\
	dist_ret_type dist_##dist##_generate(x_rng *rng,		\
					     __VA_ARGS__)

DIST_GENERATE_WITH_CALLBACK_DECL(uniform_int, long, long a, long b);
DIST_GENERATE_DECL(uniform_int, long, long a, long b);

DIST_GENERATE_WITH_CALLBACK_DECL(uniform_real, double, double a, double b);
DIST_GENERATE_DECL(uniform_real, double, double a, double b);


DIST_GENERATE_WITH_CALLBACK_DECL(bernoulli, long, double p);
DIST_GENERATE_DECL(bernoulli, long, double p);

DIST_GENERATE_WITH_CALLBACK_DECL(binomial, long, long t, double p);
DIST_GENERATE_DECL(binomial, long, long t, double p);

DIST_GENERATE_WITH_CALLBACK_DECL(geometric, long, double p);
DIST_GENERATE_DECL(geometric, long, double p);

DIST_GENERATE_WITH_CALLBACK_DECL(negative_binomial, long, long t, double p);
DIST_GENERATE_DECL(negative_binomial, long, long t, double p);

// multinomial

DIST_GENERATE_WITH_CALLBACK_DECL(hypergeometric, long, long n, long M, long N);
DIST_GENERATE_DECL(hypergeometric, long, long n, long M, long N);

// multivariate hypergeometric


DIST_GENERATE_WITH_CALLBACK_DECL(poisson, long, double mean);
DIST_GENERATE_DECL(poisson, long, double mean);

DIST_GENERATE_WITH_CALLBACK_DECL(exponential, double, double lambda);
DIST_GENERATE_DECL(exponential, double, double lambda);

DIST_GENERATE_WITH_CALLBACK_DECL(gamma, double, double alpha, double beta);
DIST_GENERATE_DECL(gamma, double, double alpha, double beta);

DIST_GENERATE_WITH_CALLBACK_DECL(weibull, double, double a, double b);
DIST_GENERATE_DECL(weibull, double, double a, double b);

DIST_GENERATE_WITH_CALLBACK_DECL(extreme_value, double, double a, double b);
DIST_GENERATE_DECL(extreme_value, double, double a, double b);

// dirichlet

DIST_GENERATE_WITH_CALLBACK_DECL(beta, double, double alpha, double beta);
DIST_GENERATE_DECL(beta, double, double alpha, double beta);

DIST_GENERATE_WITH_CALLBACK_DECL(kumaraswamy, double, double alpha, double beta);
DIST_GENERATE_DECL(kumaraswamy, double, double alpha, double beta);

DIST_GENERATE_WITH_CALLBACK_DECL(laplace, double, double mu, double sigma);
DIST_GENERATE_DECL(laplace, double, double mu, double sigma);

DIST_GENERATE_WITH_CALLBACK_DECL(erlang, double, long k, double lambda);
DIST_GENERATE_DECL(erlang, double, double k, double lambda);


DIST_GENERATE_WITH_CALLBACK_DECL(normal, double, double mean, double stddev);
DIST_GENERATE_DECL(normal, double, double mean, double stddev);

DIST_GENERATE_WITH_CALLBACK_DECL(lognormal, double, double m, double s);
DIST_GENERATE_DECL(lognormal, double, double m, double s);

DIST_GENERATE_WITH_CALLBACK_DECL(chi_squared, double, double n);
DIST_GENERATE_DECL(chi_squared, double, double n);

DIST_GENERATE_WITH_CALLBACK_DECL(cauchy, double, double a, double b);
DIST_GENERATE_DECL(cauchy, double, double a, double b);

DIST_GENERATE_WITH_CALLBACK_DECL(fisher_f, double, double m, double n);
DIST_GENERATE_DECL(fisher_f, double, double m, double n);

DIST_GENERATE_WITH_CALLBACK_DECL(student_t, double, double n);
DIST_GENERATE_DECL(student_t, double, double n);

DIST_GENERATE_WITH_CALLBACK_DECL(rayleigh, double, double sigma);
DIST_GENERATE_DECL(rayleigh, double, double sigma);

#ifdef __cplusplus
}
#endif

#endif
