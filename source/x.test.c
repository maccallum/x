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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "x.capi.h"

void random_device_delegate_callback(xobj_uint32 *x, size_t n)
{
	if(*(x->n) != n || !(*(x->buf))){
		*(x->buf) = (uint32_t *)realloc(*(x->buf), n * sizeof(uint32_t));
		*(x->n) = n;
	}
	for(size_t i = 0; i < n; i++){
		(*(x->buf))[i] = random_device_generate(x);
	}
}

void seed_seq_from_delegate_callback(xobj_uint32 *x, size_t n)
{
	if(!(x->n) || !(x->buf) || *(x->n) != n){
		*(x->buf) = (uint32_t *)realloc(*(x->buf), n * sizeof(uint32_t));
		*(x->n) = n;
	}
	seed_seq_from_generate(x, *(x->buf), (*(x->buf)) + n);
}

void rng_delegate_uint32_callback(xobj_uint32 *x, size_t n){
	if(!(x->n) || !(x->buf) || *(x->n) != n){
		*(x->buf) = (uint32_t *)realloc(*(x->buf), n * sizeof(uint32_t));
		*(x->n) = n;	
	}
	for(size_t i = 0; i < n; i++){
		(*(x->buf))[i] = rng_pcg32_generate(x);
	}
}

// int main(int argc, char **argv)
// {
// 	xobj_uint32 *rd = random_device_new(random_device_delegate_callback);
// 	xobj_uint32 *ssf = seed_seq_from_new(seed_seq_from_delegate_callback);
// 	seed_seq_from_setcontext(ssf, (void *)rd);
// 	size_t buflen = 10;
// 	uint32_t buf[buflen];
// 	xobj_uint32 *ssfd = seed_seq_from_delegate_new();
// 	seed_seq_from_delegate_setcontext(ssfd, ssf);
// 	xobj_uint32 *r = rng_pcg32_new_seed_seq_from_delegate(ssfd);
// 	for(int i = 0; i < 10; i++){
// 		double f = dist_gamma_generate((void *)r, rng_delegate_uint32_callback, 2., 2., rng_pcg32_min(), rng_pcg32_max());
// 		printf("f = %f\n", f);
// 	}
	
// 	random_device_delete(rd);
// 	seed_seq_from_delete(ssf);
// 	seed_seq_from_delegate_delete(ssfd);
// 	rng_pcg32_delete(r);

// 	return 0;
// }

int main(int argc, char **argv)
{
	xobj_uint32 *rd = random_device_new();
	// ^^^
	// return new random_device;
	
	xobj_uint32 *ssf = seed_seq_from_new(rd, random_device_delegate_callback);
	// ^^^
	// ssf = new seed_ceq_from<random_device_delegate>
	// ssf->context(rd);
	// rd->callback = random_device_delegate_callback
	
	xobj_uint32 *ssfd = seed_seq_from_delegate_new(ssf, seed_seq_from_delegate_callback);
	// ^^^
	// ssfd = new seed_seq_from_delegate<std::random_device>
	// ssfd->context(ssf)
	// ssf->callback = seed_seq_from_delegate_callback
	
	xobj_uint32 *r = rng_pcg32_new(ssfd);

	for(int i = 0; i < 10; i++){
		double f = dist_gamma_generate((void *)r, rng_delegate_uint32_callback, 2., 2., rng_pcg32_min(), rng_pcg32_max());
		printf("f = %f\n", f);
	}

	random_device_delete(rd);
	seed_seq_from_delete(ssf);
	seed_seq_from_delegate_delete(ssfd);
	rng_pcg32_delete(r);
}
