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
	if(x->buf && *(x->buf)){
		free(*(x->buf));
		*(x->buf) = NULL;
		x->buf = NULL;
	}
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
	//random_device_delegate_callback(x->context(), x->buffer_len(), x->buffer());
	o->callback(o, i);
	o->n = 0;
	o->buf = NULL;
}

x_seed_seq_from *seed_seq_from_new(x_random_device *random_device, xobj_uint32_callback random_device_delegate_callback)
{
	x_seed_seq_from *x = (x_seed_seq_from *)calloc(1, sizeof(x_seed_seq_from));
	x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>> *o = new x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>();
	
	random_device->callback = random_device_delegate_callback;
	o->context(random_device);
	x->myobj = o;
	return x;
}

void seed_seq_from_delete(x_seed_seq_from *x)
{
	delete reinterpret_cast<x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>*>(x->myobj);
	if(x->buf && *(x->buf)){
		free(*(x->buf));
		*(x->buf) = NULL;

		x->buf = NULL;
	}
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
}

x_seed_seq_from_delegate *seed_seq_from_delegate_new(x_seed_seq_from *seed_seq_from, xobj_uint32_callback seed_seq_from_delegate_callback)
{
	x_seed_seq_from_delegate *x = (x_seed_seq_from_delegate *)calloc(1, sizeof(x_seed_seq_from_delegate));
	x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device> *o = new x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device>();
	seed_seq_from->callback = seed_seq_from_delegate_callback;
	o->context(seed_seq_from);
	x->myobj = o;
	return x;
}

void seed_seq_from_delegate_delete(x_seed_seq_from_delegate *x)
{
	delete reinterpret_cast<x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device>*>(x->myobj);
	if(x->buf && *(x->buf)){
		free(*(x->buf));
		*(x->buf) = NULL;
		x->buf = NULL;
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

x_rng *rng_pcg32_new(x_rng *ssfd)
{
	x_rng *x = (x_rng *)calloc(1, sizeof(x_rng));
	x->myobj = new pcg32(*reinterpret_cast<x::proxy::seed_seq_from_delegate<seed_seq_from_delegate_base, std::random_device>*>(ssfd->myobj));
	return x;
}

void rng_pcg32_delete(x_rng *x)
{
	delete reinterpret_cast<pcg32*>(x->myobj);
	if(x->buf && *(x->buf)){
		free(*(x->buf));
		*(x->buf) = NULL;
		x->buf = NULL;
	}
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
	return reinterpret_cast<pcg32*>(r->myobj)->operator()();
}

//////////////////////////////////////////////////
// Rng_delegate
//////////////////////////////////////////////////

using rng_delegate_uint32 = x::proxy::delegate<uint32_t, uint32_t, get_u32, set_u32>;
using rng_delegate_uint64 = x::proxy::delegate<uint64_t, uint64_t, get_u64, set_u64>;

extern void rng_delegate_uint32_callback(void *context, size_t n);

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0xFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	x_rng *x = (x_rng *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0xFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	x_rng *x = (x_rng *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0x7FFFFFFE>::callback(rng_delegate_uint32 *o, size_t n)
{
	x_rng *x = (x_rng *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0x7FFFFFFE>::callback(rng_delegate_uint32 *o, size_t n)
{
	x_rng *x = (x_rng *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0xFFFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	x_rng *x = (x_rng *)(o->context());
	x->callback(x, n);
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0xFFFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	x_rng *x = (x_rng *)(o->context());
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

// dist_gamma *dist_gamma_new(void)
// {
// 	return reinterpret_cast<dist_gamma*>(new std::gamma_distribution<double>());
// }

// void dist_gamma_delete(dist_gamma *d)
// {
// 	delete reinterpret_cast<std::gamma_distribution<double>*>(d);
// }

#define DIST_CALL(dist_ret_type, rng_type, min, max) \
	{\
		int i = 0;\
		x::proxy::rng_delegate<rng_delegate_##rng_type, rng_type##_t, min, max> rngd;\
		rngd.context(rng);\
		rng_type##_t buf = 0;\
		rngd.buffer(&buf);\
		rngd.buffer_len(1);\
		rng->buf = rngd.buffer_address();\
		rng->n = rngd.buffer_len_address();\
		rng->callback = rng_delegate_callback;\
		dist_ret_type f = d(rngd);\
		rngd.buffer(NULL);\
		rngd.buffer_len(0);\
		rng->buf = NULL;\
		rng->n = 0;\
		return f;\
	}

#define DIST_CALL_SWITCH(dist_ret_type)					\
	{								\
		if(rng_max == 0xFFFFFF){				\
			switch(rng_min){				\
			case 0:						\
				DIST_CALL(dist_ret_type, uint32, 0, 0xFFFFFF);	\
			case 1:						\
				DIST_CALL(dist_ret_type, uint32, 1, 0xFFFFFF);	\
			}						\
		}else if(rng_max == 0x7FFFFFFE){			\
			switch(rng_min){				\
			case 0:						\
				DIST_CALL(dist_ret_type, uint32, 0, 0x7FFFFFFE); \
			case 1:						\
				DIST_CALL(dist_ret_type, uint32, 1, 0x7FFFFFFE); \
			}						\
		}else if(rng_max == 0xFFFFFFFF){			\
			switch(rng_min){				\
			case 0:						\
				DIST_CALL(dist_ret_type, uint32, 0, 0xFFFFFFFF); \
			case 1:						\
				DIST_CALL(dist_ret_type, uint32, 1, 0xFFFFFFFF); \
			}						\
		}else if(rng_max == 0xFFFFFFFFFFFF){			\
			switch(rng_min){				\
			case 0:						\
				{					\
					return 0;			\
				}					\
				break;					\
			case 1:						\
				{					\
					return 0;			\
				}					\
				break;					\
			}						\
		}else if(rng_max == 0xFFFFFFFFFFFFFFFF){		\
			switch(rng_min){				\
			case 0:						\
				{					\
					return 0;			\
				}					\
				break;					\
			case 1:						\
				{					\
					return 0;			\
				}					\
				break;					\
			}						\
		}else{							\
		}							\
		return 0;						\
	}

double dist_gamma_generate(x_rng *rng,
			   xobj_uint32_callback rng_delegate_callback,
			   uint64_t rng_min,
			   uint64_t rng_max,
			   double alpha,
			   double beta)
{
	uint64_t buf = 0;
	x::random::gamma_distribution<double> d(alpha, beta);
	DIST_CALL_SWITCH(double);
}

} // extern "C"
