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

#include "x.capi.h"

void def_random_device_delegate_callback(x_random_device *x, size_t n)
{
	if(*(x->n) < n || !(*(x->buf))){
		*(x->buf) = (uint32_t *)realloc(*(x->buf), n * sizeof(uint32_t));
		*(x->n) = n;
	}
	for(size_t i = 0; i < n; i++){
		(*(x->buf))[i] = random_device_generate(x);
	}
}

void def_seed_seq_from_delegate_callback(x_seed_seq_from *x, size_t n)
{
	if(!(x->n) || !(x->buf) || *(x->n) < n){
		*(x->buf) = (uint32_t *)realloc(*(x->buf), n * sizeof(uint32_t));
		*(x->n) = n;
	}
	seed_seq_from_generate(x, *(x->buf), (*(x->buf)) + n);
}

void def_rng_delegate_uint32_callback(xobj_uint32 *x, size_t n)
{
	if(!(x->n) || !(x->buf) || *(x->n) < n){
		*(x->buf) = (uint32_t *)realloc(*(x->buf), n * sizeof(uint32_t));
		*(x->n) = n;	
	}
	for(size_t i = 0; i < n; i++){
		(*(x->buf))[i] = rng_pcg32_generate((x_rng *)x);
	}
}
