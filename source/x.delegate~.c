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

#ifndef __clang__
#define MAX_NVARS 8
#define MAX_NVARS_STR "8"
#endif

typedef struct _xdelegate{
	t_pxobject ob;
	void *outlet_delegation;
	double *value;
	int delegate_did_return;
	long nvars;
	long nsamples_to_wait;
	long nsamples_waited;
	t_symbol *mode;
} t_xdelegate;

void *xdelegate_class;

void xdelegate_getvalues(t_xdelegate *x, double *buf)
{
	outlet_anything(x->outlet_delegation, gensym("generate"), 0, NULL);
	memcpy(buf, x->value, x->nvars * sizeof(double));
	memset(x->value, 0, x->nvars * sizeof(double));
}

double xdelegate_getvalue(t_xdelegate *x)
{
	outlet_anything(x->outlet_delegation, gensym("generate"), 0, NULL);
	double v = x->value[0];
	memset(x->value, 0, x->nvars * sizeof(double));
	return v;
}

void xdelegate_dist_perform64(t_xdelegate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long vectorsize, long flags, void *userparam)
{
	for(int i = 0; i < vectorsize; i++){
		if(ins[0][i] != 0){
#ifndef __clang__
			double buf[MAX_NVARS];
#else
			double buf[x->nvars];
#endif
			xdelegate_getvalues(x, buf);
			for(int j = 0; j < numouts; j++){
				outs[j][i] = buf[j];
			}
		}
	}
}

void xdelegate_process_perform64(t_xdelegate *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long vectorsize, long flags, void *userparam)
{
	long rem = x->nsamples_to_wait - (x->nsamples_waited + vectorsize);
	if(rem <= 0){	
		for(long i = 0; i < vectorsize; i++){
			if(x->nsamples_to_wait - x->nsamples_waited <= 0){
				while((x->nsamples_to_wait = xdelegate_getvalue(x)) == 0.){
					if(!(x->delegate_did_return)){
						break;
					}
				}
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
	memset(x->value, 0, x->nvars * sizeof(double));
	x->nsamples_to_wait = 0;
	x->nsamples_waited = 0;
	x->delegate_did_return = 0;
	if(x->mode == gensym("dist")){
		object_method(dsp64, gensym("dsp_add64"), x, xdelegate_dist_perform64, 0, NULL);
	}else if(x->mode == gensym("process.counting")){
		object_method(dsp64, gensym("dsp_add64"), x, xdelegate_process_perform64, 0, NULL);
	}
}

void xdelegate_list(t_xdelegate *x, t_symbol *msg, short argc, t_atom *argv)
{
	x->delegate_did_return = 1;
	long n = x->nvars;
	if(argc < n){
		n = argc;
	}
	for(int i = 0; i < n; i++){
		x->value[i] = atom_getfloat(argv + i);
	}
}

void xdelegate_float(t_xdelegate *x, double f)
{
	x->delegate_did_return = 1;
	x->value[0] = f;
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
	if(x->value){
		free(x->value);
	}
}

void *xdelegate_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_xdelegate *x = NULL;
	if((x = (t_xdelegate *)object_alloc((t_class *)xdelegate_class))){
  		dsp_setup((t_pxobject *)x, 1);
		x->outlet_delegation = outlet_new((t_object *)x, "generate");
		//x->mode = gensym("dist");
		t_dictionary *d = object_dictionaryarg(argc, argv);
		attr_dictionary_process(x, d);
		if(x->mode == gensym("process.counting") && x->nvars > 1){
			object_error((t_object *)x, "process.counting mode can only be univariate. setting nvars to 1");
			x->nvars = 1;
		}
		if(msg == gensym("x.delegate~")){
			if(argc && atom_gettype(argv) == A_SYM){
				x->mode = atom_getsym(argv);
			}
		}else if(msg == gensym("x.dist.delegate~")){
		}else if(msg == gensym("x.process.counting.delegate~")){
			x->mode = gensym("process.counting");
		}
		for(int i = 0; i < x->nvars; i++){
			outlet_new((t_object *)x, "signal");
		}
		x->value = (double *)calloc(x->nvars, sizeof(double));
		x->nsamples_to_wait = x->nsamples_waited = 0;
	}
	return x;
}

#ifdef WIN_VERSION
__declspec(dllexport)
#endif
int main(void)
{
	t_class *c = (t_class *)class_new("x.delegate~", (method)xdelegate_new, (method)xdelegate_free, sizeof(t_xdelegate), 0L, A_GIMME, 0);
	class_addmethod(c, (method)xdelegate_assist, "assist", A_CANT, 0);
    	class_addmethod(c, (method)xdelegate_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)xdelegate_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)xdelegate_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)xdelegate_int, "int", A_LONG, 0);
	
	CLASS_ATTR_LONG(c, "nvars", 0, t_xdelegate, nvars);
	CLASS_ATTR_INVISIBLE(c, "nvars", 0);
	CLASS_ATTR_MIN(c, "nvars", 0, "1");
#ifndef __clang__
	CLASS_ATTR_MAX(c, "nvars", 0, MAX_NVARS_STR);
#endif
	CLASS_ATTR_DEFAULT(c, "nvars", 0, "1");

	CLASS_ATTR_SYM(c, "mode", 0, t_xdelegate, mode);
	CLASS_ATTR_INVISIBLE(c, "mode", 0);
	CLASS_ATTR_ENUM(c, "mode", 0, "dist process");
	CLASS_ATTR_DEFAULT(c, "mode", 0, "dist");

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	
    	class_dspinit(c);

	class_register(CLASS_BOX, c);
	common_symbols_init();

	xdelegate_class = c;

	return 0;
}
