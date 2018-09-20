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

//////////////////////////////////////////////////
// random_device
//////////////////////////////////////////////////

typedef struct random_device random_device;
random_device *random_device_new(void);
void random_device_delete(random_device *rd);
unsigned int random_device_generate(random_device *rd);
unsigned int random_device_min(random_device *rd);
unsigned int random_device_max(random_device *rd);

//////////////////////////////////////////////////
// seed_seq_from
//////////////////////////////////////////////////

typedef struct seed_seq_from seed_seq_from;
seed_seq_from *seed_seq_from_new(void);
void seed_seq_from_delete(seed_seq_from *ssf);
void seed_seq_from_setcontext(seed_seq_from *ssf, void *context);
void seed_seq_from_generate(seed_seq_from *_ssf, uint32_t *start, uint32_t *end);

//////////////////////////////////////////////////
// seed_seq_from_delegate
//////////////////////////////////////////////////

typedef struct seed_seq_from_delegate seed_seq_from_delegate;
seed_seq_from_delegate *seed_seq_from_delegate_new(void);
void seed_seq_from_delegate_delete(seed_seq_from_delegate *ssfd);
void seed_seq_from_delegate_setcontext(seed_seq_from_delegate *ssfd, void *context);

//////////////////////////////////////////////////
// rngs
//////////////////////////////////////////////////

typedef struct rng_pcg32 rng_pcg32;
rng_pcg32 *rng_pcg32_new_seed_seq_from_delegate(seed_seq_from_delegate *ssfd);
void rng_pcg32_delete(rng_pcg32 *r);
uint32_t rng_pcg32_min(void);
uint32_t rng_pcg32_max(void);
uint32_t rng_pcg32_generate(rng_pcg32 *r);

//////////////////////////////////////////////////
// rng_delegate
//////////////////////////////////////////////////

// typedef struct rng_delegate_0_FFFFFF rng_delegate_0_FFFFFF;
// typedef struct rng_delegate_1_FFFFFF rng_delegate_1_FFFFFF;
// typedef struct rng_delegate_0_7FFFFFFE rng_delegate_0_7FFFFFFE;
// typedef struct rng_delegate_0_7FFFFFFE rng_delegate_1_7FFFFFFE;
// typedef struct rng_delegate_0_FFFFFFFF rng_delegate_0_FFFFFFFF;
// typedef struct rng_delegate_0_FFFFFFFF rng_delegate_1_FFFFFFFF;
// typedef struct rng_delegate_0_FFFFFFFFFFFF rng_delegate_0_FFFFFFFFFFFF;
// typedef struct rng_delegate_1_FFFFFFFFFFFF rng_delegate_1_FFFFFFFFFFFF;
// typedef struct rng_delegate_0_FFFFFFFFFFFFFFFF rng_delegate_0_FFFFFFFFFFFFFFFF;
// typedef struct rng_delegate_0_FFFFFFFFFFFFFFFF rng_delegate_1_FFFFFFFFFFFFFFFF;

// rng_delegate_0_FFFFFFFF *rng_delegate_0_FFFFFFFF_new(void);
// void rng_delegate_0_FFFFFFFF_delete(rng_delegate_0_FFFFFFFF *rd);

//////////////////////////////////////////////////
// distributions
//////////////////////////////////////////////////

typedef struct dist_gamma dist_gamma;
dist_gamma *dist_gamma_new(void);
void dist_gamma_delete(dist_gamma *d);
double dist_gamma_generate(void *context, double alpha, double beta, uint64_t rng_min, uint64_t rng_max);

#ifdef __cplusplus
}
#endif

#endif
