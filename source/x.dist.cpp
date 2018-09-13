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
#include "x.max.hpp"

void *xdist_new(t_symbol *msg, short argc, t_atom *argv)
{
	char buf[128];
	sprintf(buf, "x.dist.%s", atom_getsym(argv)->s_name);
	return object_new_typed(CLASS_BOX, gensym(buf), argc - 1, argv + 1);
		// x->outlet_delegation = outlet_new((t_object *)x, NULL);
		// x->outlet_main = outlet_new((t_object *)x, NULL);
		// t_object *conduit = object_findregistered(ps_xdist_conduit, atom_getsym(argv));
		// if(!conduit){
		// 	t_atom a[2];
		// 	atom_setsym(a, gensym(XDIST_REGISTER_ATTR));
		// 	atom_setlong(a + 1, 1);
			
		// 	object_new(CLASS_NOBOX, ps_xdist_conduit, atom_getsym(argv));
		// 	conduit = object_findregistered(ps_xdist_conduit, atom_getsym(argv));
		// }
		// if(conduit){
		// 	x->conduit = (t_xdist_conduit *)conduit;
		// }
}

void ext_main(void *r)
{
	t_class *c;
	c = class_new("x.dist", (method)xdist_new, NULL, sizeof(t_maxobj), 0L, A_GIMME, 0);
	class_register(CLASS_BOX, c);

	// class_addmethod(c, (method)xdist_int,	"int", A_LONG, 0);
	// class_addmethod(c, (method)xdist_float, "float", A_FLOAT, 0);
	// class_addmethod(c, (method)xdist_anything, "list", A_GIMME, 0);
	// class_addmethod(c, (method)xdist_anything, "anything", A_GIMME, 0);
	// class_addmethod(c, (method)xdist_assist, "assist", A_CANT, 0);
	// class_addmethod(c, (method)xdist_notify, "notify", A_CANT, 0);

	// class_register(CLASS_BOX, c);
	// xdist_class = c;
}
