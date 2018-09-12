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


#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_sysmem.h"
#include "z_dsp.h"

typedef struct _xdelegate{
	t_pxobject ob;
	void *outlet_delegation;
	double value;
	long nsamples_to_wait;
	long nsamples_waited;
	t_symbol *mode;
} t_xdelegate;

void *xdelegate_class;

double xdelegate_getvalue(t_xdelegate *x)
{
	outlet_anything(x->outlet_delegation, gensym("generate"), 0, NULL);
	double v = x->value;
	x->value = 0;
	return v;
}

void xdelegate_dist_perform64(t_xdelegate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long vectorsize, long flags, void *userparam)
{
	for(int i = 0; i < vectorsize; i++){
		if(ins[0][i] != 0){
			outs[0][i] = xdelegate_getvalue(x);
		}
	}
}

void xdelegate_process_perform64(t_xdelegate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long vectorsize, long flags, void *userparam)
{
	long rem = x->nsamples_to_wait - (x->nsamples_waited + vectorsize);
	if(rem <= 0){	
		for(long i = 0; i < vectorsize; i++){
			if(x->nsamples_to_wait - x->nsamples_waited <= 0){
				while((x->nsamples_to_wait = xdelegate_getvalue(x)) == 0.){}
				outs[0][i] = 0;
				x->nsamples_waited = 1;
			}else{
				outs[0][i] = (double)x->nsamples_waited / (double)x->nsamples_to_wait;
				x->nsamples_waited++;
			}
		}
	}else{
		for(long i = 0; i < vectorsize; i++){
			outs[0][i] = (double)x->nsamples_waited / (double)x->nsamples_to_wait;
			x->nsamples_waited++;
		}
	}
}

void xdelegate_dsp64(t_xdelegate *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	if(x->mode == gensym("dist")){
		object_method(dsp64, gensym("dsp_add64"), x, xdelegate_dist_perform64, 0, NULL);
	}else if(x->mode == gensym("process")){
		object_method(dsp64, gensym("dsp_add64"), x, xdelegate_process_perform64, 0, NULL);
	}
}

void xdelegate_float(t_xdelegate *x, double f)
{
	x->value = f;
}

void xdelegate_int(t_xdelegate *x, long l)
{
	xdelegate_float(x, l);
}

void xdelegate_assist(t_xdelegate *x, void *b, long io, long num, char *buf)
{
}

void xdelegate_free(t_xdelegate *x)
{
	dsp_free((t_pxobject *)x);
}

void *xdelegate_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_xdelegate *x = NULL;
	if((x = (t_xdelegate *)object_alloc(xdelegate_class))){
  		dsp_setup((t_pxobject *)x, 1);
		x->outlet_delegation = outlet_new((t_object *)x, "generate");
		x->mode = gensym("dist");
		if(msg == gensym("x.delegate~")){
			if(argc && atom_gettype(argv) == A_SYM){
				x->mode = atom_getsym(argv);
			}
		}else if(msg == gensym("x.dist.delegate~")){
		}else if(msg == gensym("x.process.delegate~")){
			x->mode = gensym("process");
		}
		outlet_new((t_object *)x, "signal");
		if(x->mode == gensym("process")){
			//outlet_new((t_object *)x, "signal");
		}
		x->value = 0;
		x->nsamples_to_wait = x->nsamples_waited = 0;
	}
	return x;
}

int main(void)
{
	t_class *c = class_new("x.delegate~", (method)xdelegate_new, (method)xdelegate_free, sizeof(t_xdelegate), 0L, A_GIMME, 0);
	class_addmethod(c, (method)xdelegate_assist, "assist", A_CANT, 0);
    	class_addmethod(c, (method)xdelegate_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)xdelegate_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)xdelegate_int, "int", A_LONG, 0);
	
    	class_dspinit(c);

	class_register(CLASS_BOX, c);
	common_symbols_init();

	xdelegate_class = c;

	return 0;
}
