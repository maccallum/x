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

// based on the master.c and servant.c examples from the sdk

#include "ext.h"
#include "ext_obex.h"
#include "jpatcher_api.h"
#include "x.global.h"

typedef struct _xglobal
{
        t_object ob;
	void *outlet_main, *outlet_delegation;
	t_xglobal_conduit *conduit;
} t_xglobal;

void *xglobal_conduit_new(t_symbol *name);
void xglobal_conduit_free(t_xglobal_conduit *x);
//void xglobal_conduit_retain(t_xglobal_conduit *x);
//void xglobal_conduit_release(t_xglobal_conduit *x);

t_class *xglobal_class;
t_class *xglobal_conduit_class;

static t_symbol *ps_nothing;
static t_symbol *ps_xglobal_conduit;

void xglobal_anything(t_xglobal *x, t_symbol *s, long ac, t_atom *av)
{
	x->conduit->outlet_main = x->outlet_main;
	x->conduit->outlet_delegation = x->outlet_delegation;
	x->conduit->msg = s;
	x->conduit->argc = ac;
	x->conduit->argv = av;
	object_notify(x->conduit, gensym("sendmessage"), NULL);
}

void xglobal_bang(t_xglobal *x)
{
	xglobal_anything(x, gensym("bang"), 0, NULL);
}

void xglobal_int(t_xglobal *x, long n)
{
	t_atom a;
	atom_setlong(&a, n);
	xglobal_anything(x, gensym("int"), 1, &a);
}

void xglobal_float(t_xglobal *x, double f)
{
	t_atom a;
	atom_setfloat(&a, f);
	xglobal_anything(x, gensym("float"), 1, &a);
}



void xglobal_notify(t_xglobal *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("free")) {
		// the xglobal_conduit is going away, this would be fatal, but will never actually happen
	}
}

void xglobal_assist(t_xglobal *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) {
		sprintf(s, "Messages for my sweet servant");
	}else{
		sprintf(s, "I am outlet %ld", a);
	}
}

void xglobal_free(t_xglobal *x)
{
	object_method(x->conduit, gensym("release"));
}

void *xglobal_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_xglobal *x = NULL;

	if((x = (t_xglobal *)object_alloc(xglobal_class))){
		x->outlet_delegation = outlet_new((t_object *)x, NULL);
		x->outlet_main = outlet_new((t_object *)x, NULL);
		t_object *conduit = object_findregistered(ps_xglobal_conduit, atom_getsym(argv));
		if(!conduit){
			t_atom a[2];
			atom_setsym(a, gensym(XGLOBAL_REGISTER_ATTR));
			atom_setlong(a + 1, 1);
			object_new_typed(CLASS_BOX, atom_getsym(argv), 2, a);
			object_new(CLASS_NOBOX, ps_xglobal_conduit, atom_getsym(argv));
			conduit = object_findregistered(ps_xglobal_conduit, atom_getsym(argv));
		}
		if(conduit){
			x->conduit = (t_xglobal_conduit *)conduit;
		}
	}
	return x;
}

void ext_main(void *r)
{
	t_class *c;
	c = class_new("x.global", (method)xglobal_new, (method)xglobal_free, sizeof(t_xglobal), 0L, A_GIMME, 0);

	class_addmethod(c, (method)xglobal_int,	"int", A_LONG, 0);
	class_addmethod(c, (method)xglobal_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)xglobal_anything, "list", A_GIMME, 0);
	class_addmethod(c, (method)xglobal_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)xglobal_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)xglobal_notify, "notify", A_CANT, 0);

	class_register(CLASS_BOX, c);
	xglobal_class = c;

	c = class_new("xglobal_conduit", (method)xglobal_conduit_new, (method)xglobal_conduit_free, sizeof(t_xglobal_conduit), 0L, A_CANT, 0);

	//class_addmethod(c, (method)xglobal_conduit_retain, "retain", 0);
	//class_addmethod(c, (method)xglobal_conduit_release, "release", 0);

	class_register(CLASS_NOBOX, c);
	xglobal_conduit_class = c;

	ps_nothing = gensym("");
	ps_xglobal_conduit = gensym("xglobal_conduit");
}

//////////////////////////////////////////////////////////

// void xglobal_conduit_retain(t_xglobal_conduit *x)
// {
// 	x->usagecount++;
// }

// void xglobal_conduit_release(t_xglobal_conduit *x)
// {
// 	x->usagecount--;
// 	if (x->usagecount <= 0) { // commit object
// 		suicide_free(x);
// 	}
// }

void xglobal_conduit_free(t_xglobal_conduit *x)
{
	object_unregister(x);
}

void *xglobal_conduit_new(t_symbol *name)
{
	t_xglobal_conduit *x = NULL;

	if(name && name != ps_nothing){
		x = (t_xglobal_conduit *)object_alloc(xglobal_conduit_class);
		if(x){
			//x->usagecount = 0;
			object_register(ps_xglobal_conduit, name, x);
		}
	}
	return x;
}
