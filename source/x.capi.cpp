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

random_device *random_device_new(void)
{
	return reinterpret_cast<random_device*>(new std::random_device());
}

void random_device_delete(random_device *rd)
{
	delete reinterpret_cast<std::random_device*>(rd);
}

unsigned int random_device_generate(random_device *rd)
{
	return reinterpret_cast<std::random_device*>(rd)->operator()();
}

unsigned int random_device_min(random_device *rd)
{
	return reinterpret_cast<std::random_device*>(rd)->min();
}

unsigned int random_device_max(random_device *rd)
{
	return reinterpret_cast<std::random_device*>(rd)->max();
}

//////////////////////////////////////////////////
// seed_seq_from
//////////////////////////////////////////////////

extern void random_device_delegate_callback(void *context, size_t buflen, uint32_t *buf);
template<> void x::proxy::random_device_delegate<x::proxy::delegate<uint32_t, uint32_t, &(get_u32), &(set_u32)> >::callback(x::proxy::delegate<uint32_t, uint32_t, &(get_u32), &(set_u32)>* x, unsigned long i)
{
	random_device_delegate_callback(x->context(), x->buffer_len(), x->buffer());
}

seed_seq_from *seed_seq_from_new(void)
{
	return reinterpret_cast<seed_seq_from*>(new x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>());
}

void seed_seq_from_delete(seed_seq_from *ssf)
{
	delete reinterpret_cast<x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>*>(ssf);
}

void seed_seq_from_setcontext(seed_seq_from *ssf, void *context)
{
	reinterpret_cast<x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>*>(ssf)->context(context);
}

void seed_seq_from_generate(seed_seq_from *_ssf, uint32_t *start, uint32_t *end)
{
	x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>> *ssf = reinterpret_cast<x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>*>(_ssf);
	ssf->buffer(start);
	ssf->buffer_len(end - start);
	reinterpret_cast<x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>>>*>(ssf)->generate(start, end);
}

//////////////////////////////////////////////////
// seed_seq_from_delegate
//////////////////////////////////////////////////

using seed_seq_from_delegate_base = x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>;

extern void seed_seq_from_delegate_callback(void *context, size_t n, size_t *buflen, uint32_t **buf);
template <> void x::proxy::seed_seq_from_delegate<seed_seq_from_delegate_base, std::random_device>::callback(seed_seq_from_delegate_base *ssfd, size_t n)
{
	seed_seq_from_delegate_callback(ssfd->context(), n, ssfd->buffer_len_address(), (uint32_t **)ssfd->buffer_address());
}

seed_seq_from_delegate *seed_seq_from_delegate_new(void)
{
	return reinterpret_cast<seed_seq_from_delegate*>(new x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device>());
}

void seed_seq_from_delegate_delete(seed_seq_from_delegate *ssfd)
{
	delete reinterpret_cast<x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device>*>(ssfd);
}

void seed_seq_from_delegate_setcontext(seed_seq_from_delegate *ssfd, void *context)
{
	reinterpret_cast<x::proxy::seed_seq_from_delegate<x::proxy::delegate<uint_least32_t, uint32_t, get_u32, set_u32>, std::random_device>*>(ssfd)->context(context);
}

//////////////////////////////////////////////////
// rngs
//////////////////////////////////////////////////

rng_pcg32 *rng_pcg32_new_seed_seq_from_delegate(seed_seq_from_delegate *ssfd)
{
	return reinterpret_cast<rng_pcg32*>(new pcg32(*reinterpret_cast<x::proxy::seed_seq_from_delegate<seed_seq_from_delegate_base, std::random_device>*>(ssfd)));
}

void rng_pcg32_delete(rng_pcg32 *r)
{
	delete reinterpret_cast<pcg32*>(r);
}

uint32_t rng_pcg32_min(void)
{
	return pcg32::min();
}

uint32_t rng_pcg32_max(void)
{
	return pcg32::max();
}

uint32_t rng_pcg32_generate(rng_pcg32 *r)
{
	return reinterpret_cast<pcg32*>(r)->operator()();
}

//////////////////////////////////////////////////
// rng_delegate
//////////////////////////////////////////////////

using rng_delegate_uint32 = x::proxy::delegate<uint32_t, uint32_t, get_u32, set_u32>;
using rng_delegate_uint64 = x::proxy::delegate<uint64_t, uint64_t, get_u64, set_u64>;

extern void rng_delegate_uint32_callback(void *context, size_t n, size_t *buflen, uint32_t **buf);

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0xFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	rng_delegate_uint32_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0xFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	rng_delegate_uint32_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0x7FFFFFFE>::callback(rng_delegate_uint32 *o, size_t n)
{
	rng_delegate_uint32_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0x7FFFFFFE>::callback(rng_delegate_uint32 *o, size_t n)
{
	rng_delegate_uint32_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0xFFFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	rng_delegate_uint32_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0xFFFFFFFF>::callback(rng_delegate_uint32 *o, size_t n)
{
	rng_delegate_uint32_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFF>::callback(rng_delegate_uint64 *o, size_t n)
{
	//rng_delegate_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFF>::callback(rng_delegate_uint64 *o, size_t n)
{
	//rng_delegate_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFFFFFF>::callback(rng_delegate_uint64 *o, size_t n)
{
	//rng_delegate_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

template<> void x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFFFFFF>::callback(rng_delegate_uint64 *o, size_t n)
{
	//rng_delegate_callback(o->context(), n, o->buffer_len_address(), o->buffer_address());
}

//////////////////////////////////////////////////
// distributions
//////////////////////////////////////////////////

dist_gamma *dist_gamma_new(void)
{
	return reinterpret_cast<dist_gamma*>(new std::gamma_distribution<double>());
}

void dist_gamma_delete(dist_gamma *d)
{
	delete reinterpret_cast<std::gamma_distribution<double>*>(d);
}

double dist_gamma_generate(void *context, double alpha, double beta, uint64_t rng_min, uint64_t rng_max)
{
	std::gamma_distribution d(alpha, beta);
	if(rng_max == 0xFFFFFF){
		switch(rng_min){
		case 0:
			{
				x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0xFFFFFF> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		case 1:
			{
				x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0xFFFFFF> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		}
	}else if(rng_max == 0x7FFFFFFE){
		switch(rng_min){
		case 0:
			{
				x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0x7FFFFFFE> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		case 1:
			{
				x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0x7FFFFFFE> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		}
	}else if(rng_max == 0xFFFFFFFF){
		switch(rng_min){
		case 0:
			{
				x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 0, 0xFFFFFFFF> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		case 1:
			{
				x::proxy::rng_delegate<rng_delegate_uint32, uint32_t, 1, 0xFFFFFFFF> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		}
	}else if(rng_max == 0xFFFFFFFFFFFF){
		switch(rng_min){
		case 0:
			{
				x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFF> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		case 1:
			{
				x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFF> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		}
	}else if(rng_max == 0xFFFFFFFFFFFFFFFF){
		switch(rng_min){
		case 0:
			{
				x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFFFFFF> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		case 1:
			{
				x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFFFFFF> rng;
				rng.context(context);
				return d(rng);
			}
			break;
		}
	}else{
	}
	return 0;
}

}
