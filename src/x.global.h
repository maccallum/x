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

#ifndef __x_global_h__
#define __x_global_h__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xglobal_conduit
{
	t_object ob;
	void *outlet_main, *outlet_delegation;
	t_symbol *msg;
	short argc;
	t_atom *argv;
} t_xglobal_conduit;

#define XGLOBAL_REGISTER "xglobal_register"
#define XGLOBAL_REGISTER_ATTR "@xglobal_register"

#ifdef __cplusplus
}
#endif

#endif
