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
#include "x.capi-callbacks.h"

int main(int argc, char **argv)
{
	x_random_device *rd = random_device_new();
	x_seed_seq_from *ssf = seed_seq_from_new(rd);
	x_seed_seq_from_delegate *ssfd = seed_seq_from_delegate_new(ssf);
	x_rng *r = rng_pcg32_new(ssfd);

	for(int i = 0; i < 10; i++){
		double f = dist_gamma_generate(r, 2., 2.);
		printf("f = %f\n", f);
	}

	seed_seq_from_delegate_delete(ssfd);        
	seed_seq_from_delete(ssf);

	random_device_delete(rd);
	rng_pcg32_delete(r);
	while(1){sleep(1);}
}
