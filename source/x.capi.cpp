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

#include <random>
#include "pcg_random.hpp"
#include "x.random.hpp"
#include "x.proxy.hpp"
#include "x.capi.h"
#include "x.capi-callbacks.h"

extern "C" {

uint32_t get_u32(uint32_t *i){return *i;}
void set_u32(uint32_t *i, uint32_t ii){*i = ii;}
uint64_t get_u64(uint64_t *i){return *i;}
void set_u64(uint64_t *i, uint64_t ii){*i = ii;}

//////////////////////////////////////////////////
// random_device
//////////////////////////////////////////////////

x_random_device *random_device_new(void)
{
	x_random_device *x = (x_random_device *)calloc(1, sizeof(x_random_device));
	x->myobj = (void *)(new std::random_device());
	return x;
}

void random_device_delete(x_random_device *x)
{
	delete reinterpret_cast<std::random_device*>(x->myobj);
	free(x);
}

unsigned int random_device_generate(x_random_device *x)
{
	return reinterpret_cast<std::random_device*>(x->myobj)->operator()();
}

unsigned int random_device_min(x_random_device *x)
{
	return reinterpret_cast<std::random_device*>(x->myobj)->min();
}

unsigned int random_device_max(x_random_device *x)
{
	return reinterpret_cast<std::random_device*>(x->myobj)->max();
}

//////////////////////////////////////////////////
// seed_seq_from
//////////////////////////////////////////////////

extern void random_device_delegate_callback(x_random_device *context, size_t n);

template<> void x::proxy::random_device_delegate<x::proxy::delegate<uint32_t, uint32_t, &(get_u32), &(set_u32)> >::callback(x::proxy::delegate<uint32_t, uint32_t, &(get_u32), &(set_u32)>* x, unsigned long i)
{
	x_random_device *o = (x_random_device *)x->context();
	o->n = x->buffer_len_address();
	o->buf = x->buffer_address();
	o->callback(o, i);
	o->n = 0;
	o->buf = NULL;
}

x_seed_seq_from *seed_seq_from_new_with_callback(x_random_device *random_device, xobj_uint32_callback random_device_delegate_callback)
{
	x_seed_seq_from *x = (x_seed_seq_from *)calloc(1, sizeof(x_seed_seq_from));
	x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>> *o = new x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>();
	random_device->callback = random_device_delegate_callback;
	o->context(random_device);
	x->myobj = o;
	return x;
}

x_seed_seq_from *seed_seq_from_new(x_random_device *random_device)
{
	return seed_seq_from_new_with_callback(random_device, def_random_device_delegate_callback);
}

void seed_seq_from_delete(x_seed_seq_from *x)
{
	delete ((x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>*)(x->myobj));
	free(x);
}

void seed_seq_from_setcontext(x_seed_seq_from *ssf, void *context)
{
	reinterpret_cast<x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>*>(ssf->myobj)->context(context);
}

void seed_seq_from_generate(x_seed_seq_from *_ssf, uint32_t *start, uint32_t *end)
{
	x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>> *ssf = reinterpret_cast<x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>*>(_ssf->myobj);
	ssf->buffer(start);
	ssf->buffer_len(end - start);
	ssf->generate(start, end);
}

//////////////////////////////////////////////////
// seed_seq_from_delegate
//////////////////////////////////////////////////

using seed_seq_from_delegate_base = x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>;

extern void seed_seq_from_delegate_callback(x_seed_seq_from *context, size_t n);

template <> void x::proxy::seed_seq_from_delegate<seed_seq_from_delegate_base, std::random_device>::callback(seed_seq_from_delegate_base *x, size_t n)
{
	x_seed_seq_from *o = (x_seed_seq_from *)x->context();
	o->n = x->buffer_len_address();
	o->buf = x->buffer_address();
	o->callback(o, n);
	o->n = 0;
	o->buf = NULL;
}

x_seed_seq_from_delegate *seed_seq_from_delegate_new_with_callback(x_seed_seq_from *seed_seq_from, xobj_uint32_callback seed_seq_from_delegate_callback)
{
	x_seed_seq_from_delegate *x = (x_seed_seq_from_delegate *)calloc(1, sizeof(x_seed_seq_from_delegate));
	x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device> *o = new x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device>();
	seed_seq_from->callback = seed_seq_from_delegate_callback;
	o->context(seed_seq_from);
	x->myobj = o;
	return x;
}

x_seed_seq_from_delegate *seed_seq_from_delegate_new(x_seed_seq_from *seed_seq_from)
{
	return seed_seq_from_delegate_new_with_callback(seed_seq_from, def_seed_seq_from_delegate_callback);
}

void seed_seq_from_delegate_delete(x_seed_seq_from_delegate *x)
{
	
	if(x->myobj){
		if(((x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device> *)(x->myobj))->buffer()){
			free(((x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device> *)(x->myobj))->buffer());
		}
		delete reinterpret_cast<x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device>*>(x->myobj);
	}
	free(x);
}

void seed_seq_from_delegate_setcontext(x_seed_seq_from_delegate *ssfd, void *context)
{
	reinterpret_cast<x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device>*>(ssfd->myobj)->context(context);
}

//////////////////////////////////////////////////
// rngs
//////////////////////////////////////////////////

x_rng *rng_pcg32_new(x_seed_seq_from_delegate *ssfd)
{
	x_rng *x = (x_rng *)calloc(1, sizeof(x_rng));
	x->obj.myobj = new pcg32(*reinterpret_cast<x::proxy::seed_seq_from_delegate<seed_seq_from_delegate_base, std::random_device>*>(ssfd->myobj));
	x->min = pcg32::min();
	x->max = pcg32::max();
	return x;
}

void rng_pcg32_delete(x_rng *x)
{
	delete reinterpret_cast<pcg32*>(x->obj.myobj);
	free(x);
}

uint32_t rng_pcg32_min(void)
{
	return pcg32::min();
}

uint32_t rng_pcg32_max(void)
{
	return pcg32::max();
}

uint32_t rng_pcg32_generate(x_rng *r)
{
	return reinterpret_cast<pcg32*>(r->obj.myobj)->operator()();
}

uint64_t rng_min(x_rng *r)
{
	if(r){
		return r->min;
	}
	return 0;
}

uint64_t rng_max(x_rng *r)
{
	if(r){
		return r->max;
	}
	return 0;
}

//////////////////////////////////////////////////
// Rng_delegate
//////////////////////////////////////////////////

using rng_delegate_uint32 = x::proxy::delegate<uint32_t, uint32_t, get_u32, set_u32>;
using rng_delegate_uint64 = x::proxy::delegate<uint64_t, uint64_t, get_u64, set_u64>;

extern void rng_delegate_uint32_callback(void *context, size_t n);

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0xFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	xobj_uint32 *x = (xobj_uint32 *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0xFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	xobj_uint32 *x = (xobj_uint32 *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0x7FFFFFFE>::callback(rng_delegate_uint32 *o, size_t n)
{
	xobj_uint32 *x = (xobj_uint32 *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0x7FFFFFFE>::callback(rng_delegate_uint32 *o, size_t n)
{
	xobj_uint32 *x = (xobj_uint32 *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0xFFFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	xobj_uint32 *x = (xobj_uint32 *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0xFFFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	xobj_uint32 *x = (xobj_uint32 *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFF>::callback(rng_delegate_uint64 *o, size_t n)
{

}

template<> void x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFF>::callback(rng_delegate_uint64 *o, size_t n)
{

}

template<> void x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFFFFFF>::callback(rng_delegate_uint64 *o, size_t n)
{

}

template<> void x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFFFFFF>::callback(rng_delegate_uint64 *o, size_t n)
{

}

//////////////////////////////////////////////////
// distributions
//////////////////////////////////////////////////
#define DIST_CALL(dist_ret_name, rng_type, min, max)	\
	{\
		int i = 0;\
		x::proxy::rng_delegate<rng_delegate_##rng_type, rng_type##_t, min, max> rngd;\
		rngd.context(rng);\
		rng_type##_t buf = 0;\
		rngd.buffer(&buf);\
		rngd.buffer_len(1);\
		rng->obj.buf = rngd.buffer_address();\
		rng->obj.n = rngd.buffer_len_address();\
		rng->obj.callback = rng_delegate_callback;\
		dist_ret_name = d(rngd);\
		rngd.buffer(NULL);\
		rngd.buffer_len(0);\
		rng->obj.buf = NULL;\
		rng->obj.n = 0;\
	}

#define DIST_CALL_SWITCH(dist_ret_name)					\
	{								\
		if(rng_max == 0xFFFFFF){				\
			switch(rng_min){				\
			case 0:						\
				DIST_CALL(dist_ret_name, uint32, 0, 0xFFFFFF); \
			case 1:						\
				DIST_CALL(dist_ret_name, uint32, 1, 0xFFFFFF); \
			}						\
		}else if(rng_max == 0x7FFFFFFE){			\
			switch(rng_min){				\
			case 0:						\
				DIST_CALL(dist_ret_name, uint32, 0, 0x7FFFFFFE); \
				break;					\
			case 1:						\
				DIST_CALL(dist_ret_name, uint32, 1, 0x7FFFFFFE); \
				break;					\
			}						\
		}else if(rng_max == 0xFFFFFFFF){			\
			switch(rng_min){				\
			case 0:						\
				DIST_CALL(dist_ret_name, uint32, 0, 0xFFFFFFFF); \
				break;					\
			case 1:						\
				DIST_CALL(dist_ret_name, uint32, 1, 0xFFFFFFFF); \
				break;					\
			}						\
		}else if(rng_max == 0xFFFFFFFFFFFF){			\
			switch(rng_min){				\
			case 0:						\
				{					\
				}					\
				break;					\
			case 1:						\
				{					\
				}					\
				break;					\
			}						\
		}else if(rng_max == 0xFFFFFFFFFFFFFFFF){		\
			switch(rng_min){				\
			case 0:						\
				{					\
				}					\
				break;					\
			case 1:						\
				{					\
				}					\
				break;					\
			}						\
		}else{							\
		}							\
	}

#define DIST_GENERATE_WITH_CALLBACK_DEFN(dist, dist_ret_type, ...)\
	{								\
		using namespace x::random;				\
		uint64_t rng_min = rng->min;				\
		uint64_t rng_max = rng->max;				\
		uint64_t buf = 0;					\
		dist##_distribution<dist_ret_type> d(__VA_ARGS__);	\
		dist_ret_type ret;					\
		DIST_CALL_SWITCH(ret);			\
		return ret;						\
	}

#define DIST_GENERATE_DEFN(dist, dist_ret_type, ...)\
	{								\
		return dist_##dist##_generate_with_callback(rng, def_rng_delegate_uint32_callback, /*rng_min, rng_max, */__VA_ARGS__); \
	}

DIST_GENERATE_WITH_CALLBACK_DECL(uniform_int, long, long a, long b)
DIST_GENERATE_WITH_CALLBACK_DEFN(uniform_int, long, a, b)
DIST_GENERATE_DECL(uniform_int, long, long a, long b)
DIST_GENERATE_DEFN(uniform_int, long, a, b)

DIST_GENERATE_WITH_CALLBACK_DECL(uniform_real, double, double a, double b)
DIST_GENERATE_WITH_CALLBACK_DEFN(uniform_real, double, a, b)
DIST_GENERATE_DECL(uniform_real, double, double a, double b)
DIST_GENERATE_DEFN(uniform_real, double, a, b)


DIST_GENERATE_WITH_CALLBACK_DECL(bernoulli, long, double p)
DIST_GENERATE_WITH_CALLBACK_DEFN(bernoulli, long, p)
DIST_GENERATE_DECL(bernoulli, long, double p)
DIST_GENERATE_DEFN(bernoulli, long, p)

DIST_GENERATE_WITH_CALLBACK_DECL(binomial, long, long t, double p)
DIST_GENERATE_WITH_CALLBACK_DEFN(binomial, long, t, p)
DIST_GENERATE_DECL(binomial, long, long t, double p)
DIST_GENERATE_DEFN(binomial, long, t, p)

DIST_GENERATE_WITH_CALLBACK_DECL(geometric, long, double p)
DIST_GENERATE_WITH_CALLBACK_DEFN(geometric, long, p)
DIST_GENERATE_DECL(geometric, long, double p)
DIST_GENERATE_DEFN(geometric, long, p)

DIST_GENERATE_WITH_CALLBACK_DECL(negative_binomial, long, long t, double p)
DIST_GENERATE_WITH_CALLBACK_DEFN(negative_binomial, long, t, p)
DIST_GENERATE_DECL(negative_binomial, long, long t, double p)
DIST_GENERATE_DEFN(negative_binomial, long, t, p)

long *dist_multinomial_generate_with_callback(x_rng *rng, xobj_uint32_callback rng_delegate_callback, long n, long buflen, double *p, long *res)
{
	uint64_t rng_min = rng->min;
	uint64_t rng_max = rng->max;
	uint64_t buf = 0;
	x::random::multinomial_distribution<long> d(n, p, p + buflen);
	std::vector<long> ret;
	DIST_CALL_SWITCH(ret);
	long _n = buflen;
	if(ret.size() < _n){
		_n = ret.size();
	}
	for(long i = 0; i < _n; i++){
		res[i] = ret[i];
	}
	return res;
}

long *dist_multinomial_generate(x_rng *rng, long n, long buflen, double *p, long *res)
{
	return dist_multinomial_generate_with_callback(rng, def_rng_delegate_uint32_callback, n, buflen, p, res);
}

DIST_GENERATE_WITH_CALLBACK_DECL(hypergeometric, long, long n, long M, long N)
DIST_GENERATE_WITH_CALLBACK_DEFN(hypergeometric, long, n, M, N)
DIST_GENERATE_DECL(hypergeometric, long, long n, long M, long N)
DIST_GENERATE_DEFN(hypergeometric, long, n, M, N)

long *dist_multivariate_hypergeometric_generate_with_callback(x_rng *rng, xobj_uint32_callback rng_delegate_callback, long n, long buflen, long *M, long *res)
{
	uint64_t rng_min = rng->min;
	uint64_t rng_max = rng->max;
	uint64_t buf = 0;
	x::random::multivariate_hypergeometric_distribution<long> d(n, M, M + buflen);
	std::vector<long> ret;
	DIST_CALL_SWITCH(ret);
	long _n = buflen;
	if(ret.size() < _n){
		_n = ret.size();
	}
	for(long i = 0; i < _n; i++){
		res[i] = ret[i];
	}
	return res;
}

long *dist_multivariate_hypergeometric_generate(x_rng *rng, long n, long buflen, long *M, long *res)
{
	return dist_multivariate_hypergeometric_generate_with_callback(rng, def_rng_delegate_uint32_callback, n, buflen, M, res);
}

DIST_GENERATE_WITH_CALLBACK_DECL(poisson, long, double mean)
DIST_GENERATE_WITH_CALLBACK_DEFN(poisson, long, mean)
DIST_GENERATE_DECL(poisson, long, double mean)
DIST_GENERATE_DEFN(poisson, long, mean)

DIST_GENERATE_WITH_CALLBACK_DECL(exponential, double, double lambda)
DIST_GENERATE_WITH_CALLBACK_DEFN(exponential, double, lambda)
DIST_GENERATE_DECL(exponential, double, double lambda)
DIST_GENERATE_DEFN(exponential, double, lambda)

DIST_GENERATE_WITH_CALLBACK_DECL(gamma, double, double alpha, double beta)
DIST_GENERATE_WITH_CALLBACK_DEFN(gamma, double, alpha, beta)
DIST_GENERATE_DECL(gamma, double, double alpha, double beta)
DIST_GENERATE_DEFN(gamma, double, alpha, beta)

DIST_GENERATE_WITH_CALLBACK_DECL(weibull, double, double a, double b)
DIST_GENERATE_WITH_CALLBACK_DEFN(weibull, double, a, b)
DIST_GENERATE_DECL(weibull, double, double a, double b)
DIST_GENERATE_DEFN(weibull, double, a, b)

DIST_GENERATE_WITH_CALLBACK_DECL(extreme_value, double, double a, double b)
DIST_GENERATE_WITH_CALLBACK_DEFN(extreme_value, double, a, b)
DIST_GENERATE_DECL(extreme_value, double, double a, double b)
DIST_GENERATE_DEFN(extreme_value, double, a, b)

double *dist_dirichlet_generate_with_callback(x_rng *rng, xobj_uint32_callback rng_delegate_callback, long buflen, double *alpha, double *res)
{
	uint64_t rng_min = rng->min;
	uint64_t rng_max = rng->max;
	uint64_t buf = 0;
	x::random::dirichlet_distribution<double> d(alpha, alpha + buflen);
	std::vector<double> ret;
	DIST_CALL_SWITCH(ret);
	long _n = buflen;
	if(ret.size() < _n){
		_n = ret.size();
	}
	for(long i = 0; i < _n; i++){
		res[i] = ret[i];
	}
	return res;
}

double *dist_dirichlet_generate(x_rng *rng, long buflen, double *alpha, double *res)
{
	return dist_dirichlet_generate_with_callback(rng, def_rng_delegate_uint32_callback, buflen, alpha, res);
}

DIST_GENERATE_WITH_CALLBACK_DECL(beta, double, double alpha, double beta)
DIST_GENERATE_WITH_CALLBACK_DEFN(beta, double, alpha, beta)
DIST_GENERATE_DECL(beta, double, double alpha, double beta)
DIST_GENERATE_DEFN(beta, double, alpha, beta)

DIST_GENERATE_WITH_CALLBACK_DECL(kumaraswamy, double, double alpha, double beta)
DIST_GENERATE_WITH_CALLBACK_DEFN(kumaraswamy, double, alpha, beta)
DIST_GENERATE_DECL(kumaraswamy, double, double alpha, double beta)
DIST_GENERATE_DEFN(kumaraswamy, double, alpha, beta)

DIST_GENERATE_WITH_CALLBACK_DECL(laplace, double, double mu, double sigma)
DIST_GENERATE_WITH_CALLBACK_DEFN(laplace, double, mu, sigma)
DIST_GENERATE_DECL(laplace, double, double mu, double sigma)
DIST_GENERATE_DEFN(laplace, double, mu, sigma)

DIST_GENERATE_WITH_CALLBACK_DECL(erlang, double, long k, double lambda)
DIST_GENERATE_WITH_CALLBACK_DEFN(erlang, double, k, lambda)
DIST_GENERATE_DECL(erlang, double, double k, double lambda)
DIST_GENERATE_DEFN(erlang, double, k, lambda)


DIST_GENERATE_WITH_CALLBACK_DECL(normal, double, double mean, double stddev)
DIST_GENERATE_WITH_CALLBACK_DEFN(normal, double, mean, stddev)
DIST_GENERATE_DECL(normal, double, double mean, double stddev)
DIST_GENERATE_DEFN(normal, double, mean, stddev)

DIST_GENERATE_WITH_CALLBACK_DECL(lognormal, double, double m, double s)
DIST_GENERATE_WITH_CALLBACK_DEFN(lognormal, double, m, s)
DIST_GENERATE_DECL(lognormal, double, double m, double s)
DIST_GENERATE_DEFN(lognormal, double, m, s)

DIST_GENERATE_WITH_CALLBACK_DECL(chi_squared, double, double n)
DIST_GENERATE_WITH_CALLBACK_DEFN(chi_squared, double, n)
DIST_GENERATE_DECL(chi_squared, double, double n)
DIST_GENERATE_DEFN(chi_squared, double, n)

DIST_GENERATE_WITH_CALLBACK_DECL(cauchy, double, double a, double b)
DIST_GENERATE_WITH_CALLBACK_DEFN(cauchy, double, a, b)
DIST_GENERATE_DECL(cauchy, double, double a, double b)
DIST_GENERATE_DEFN(cauchy, double, a, b)

DIST_GENERATE_WITH_CALLBACK_DECL(fisher_f, double, double m, double n)
DIST_GENERATE_WITH_CALLBACK_DEFN(fisher_f, double, m, n)
DIST_GENERATE_DECL(fisher_f, double, double m, double n)
DIST_GENERATE_DEFN(fisher_f, double, m, n)

DIST_GENERATE_WITH_CALLBACK_DECL(student_t, double, double n)
DIST_GENERATE_WITH_CALLBACK_DEFN(student_t, double, n)
DIST_GENERATE_DECL(student_t, double, double n)
DIST_GENERATE_DEFN(student_t, double, n)

DIST_GENERATE_WITH_CALLBACK_DECL(rayleigh, double, double sigma)
DIST_GENERATE_WITH_CALLBACK_DEFN(rayleigh, double, sigma)
DIST_GENERATE_DECL(rayleigh, double, double sigma)
DIST_GENERATE_DEFN(rayleigh, double, sigma)


long dist_discrete_generate_with_callback(x_rng *rng, xobj_uint32_callback rng_delegate_callback, long buflen, double *probabilities)
{
	uint64_t rng_min = rng->min;
	uint64_t rng_max = rng->max;
	uint64_t buf = 0;
	x::random::discrete_distribution<long> d(probabilities, probabilities + buflen);
	long ret;
	DIST_CALL_SWITCH(ret);
	return ret;
}

long dist_discrete_generate(x_rng *rng, long buflen, double *probabilities)
{
	return dist_discrete_generate_with_callback(rng, def_rng_delegate_uint32_callback, buflen, probabilities);
}

// piecewise constant
double dist_piecewise_constant_generate_with_callback(x_rng *rng, xobj_uint32_callback rng_delegate_callback, long intervals_len, double *intervals, double *densities)
{
	uint64_t rng_min = rng->min;
	uint64_t rng_max = rng->max;
	uint64_t buf = 0;
	x::random::piecewise_constant_distribution<double> d(intervals, intervals + intervals_len, densities);
	double ret;
	DIST_CALL_SWITCH(ret);
	return ret;
}

double dist_piecewise_constant_generate(x_rng *rng, long intervals_len, double *intervals, double *densities)
{
	return dist_piecewise_constant_generate_with_callback(rng, def_rng_delegate_uint32_callback, intervals_len, intervals, densities);
}

double dist_piecewise_linear_generate_with_callback(x_rng *rng, xobj_uint32_callback rng_delegate_callback, long intervals_len, double *intervals, double *densities)
{
	uint64_t rng_min = rng->min;
	uint64_t rng_max = rng->max;
	uint64_t buf = 0;
	x::random::piecewise_linear_distribution<double> d(intervals, intervals + intervals_len, densities);
	double ret;
	DIST_CALL_SWITCH(ret);
	return ret;
}

double dist_piecewise_linear_generate(x_rng *rng, long intervals_len, double *intervals, double *densities)
{
	return dist_piecewise_linear_generate_with_callback(rng, def_rng_delegate_uint32_callback, intervals_len, intervals, densities);
}

// piecewise linear

} // extern "C"
