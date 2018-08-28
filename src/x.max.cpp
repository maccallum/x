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

#include "x.max.hpp"

#ifndef X_MAX_TYPE
#define X_MAX_TYPE
#define C_T X_MAX_CLASS
#define x_max_obj_name(c, t) "x." STRING(c)
#else
#define C_T EVAL(X_MAX_CLASS, EVAL(_, X_MAX_TYPE))
#define x_max_obj_name(c, t) "x." STRING(c) "." STRING(t)
#endif

#define STRING(x) #x
#define PASTE(x, y) x ## y
#define EVAL(x, y) PASTE(x, y)
#define x_max_obj_newobj EVAL(C_T, _newobj)
#define x_max_obj_freeobj EVAL(C_T, _obj::freeobj)
#define x_max_obj_struct EVAL(t_, EVAL(C_T, _maxobj))
#define x_max_obj_call_max_class EVAL(_, EVAL(C_T, _obj.max_class(c)))
#define x_max_obj_call_main EVAL(_, EVAL(C_T, _obj.main()))

namespace x
{
	namespace max
	{
		int main(void)
		{
			t_class *c = NULL;
			c = newclass(x_max_obj_name(X_MAX_CLASS, X_MAX_TYPE), x_max_obj_newobj, x_max_obj_freeobj, sizeof(x_max_obj_struct));
			if(c){
				x_max_obj_call_max_class;
				x_max_obj_call_main;
				class_register(CLASS_BOX, c);
			}
			return 0;
		}
	}
}

int main(void)
{
	return x::max::main();
}
