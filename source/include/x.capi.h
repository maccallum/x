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

struct xobj_uint32;

typedef void (*xobj_uint32_callback)(struct xobj_uint32*, size_t n);

typedef struct xobj_uint32
{
	void *myobj;
	xobj_uint32_callback callback;
	size_t *n;
	uint32_t **buf;
} xobj_uint32;

//////////////////////////////////////////////////
// random_device
//////////////////////////////////////////////////
typedef struct random_device random_device;
xobj_uint32 *random_device_new(void);
void random_device_delete(xobj_uint32 *rd);
unsigned int random_device_generate(xobj_uint32 *rd);
unsigned int random_device_min(xobj_uint32 *rd);
unsigned int random_device_max(xobj_uint32 *rd);

//////////////////////////////////////////////////
// seed_seq_from
//////////////////////////////////////////////////
typedef struct seed_seq_from seed_seq_from;
xobj_uint32 *seed_seq_from_new(xobj_uint32 *random_device, xobj_uint32_callback random_device_delegate_callback);
void seed_seq_from_delete(xobj_uint32 *ssf);
void seed_seq_from_setcontext(xobj_uint32 *ssf, void *context);
void seed_seq_from_generate(xobj_uint32 *_ssf, uint32_t *start, uint32_t *end);

//////////////////////////////////////////////////
// seed_seq_from_delegate
//////////////////////////////////////////////////
typedef struct seed_seq_from_delegate seed_seq_from_delegate;
xobj_uint32 *seed_seq_from_delegate_new(xobj_uint32 *seed_seq_from, xobj_uint32_callback seed_seq_from_delegate_callback);
void seed_seq_from_delegate_delete(xobj_uint32 *ssfd);
void seed_seq_from_delegate_setcontext(xobj_uint32 *ssfd, void *context);

//////////////////////////////////////////////////
// rngs
//////////////////////////////////////////////////
typedef struct rng_pcg32 rng_pcg32;
xobj_uint32 *rng_pcg32_new(xobj_uint32 *ssfd);
void rng_pcg32_delete(xobj_uint32 *r);
uint32_t rng_pcg32_min(void);
uint32_t rng_pcg32_max(void);
uint32_t rng_pcg32_generate(xobj_uint32 *r);

//////////////////////////////////////////////////
// distributions
//////////////////////////////////////////////////
//typedef struct dist_gamma dist_gamma;
//dist_gamma *dist_gamma_new(void);
//void dist_gamma_delete(dist_gamma *d);
double dist_gamma_generate(void *context, xobj_uint32_callback callback, double alpha, double beta, uint64_t rng_min, uint64_t rng_max);

#ifdef __cplusplus
}
#endif

#endif
