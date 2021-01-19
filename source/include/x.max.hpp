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
#include "ext_critical.h"
#include "jpatcher_utils.h"

#include <random>
#include "pcg_random.hpp"
#include "x.proxy.hpp"
#include "x.random.hpp"
#include "x.global.h"

#ifndef __X_MAX_HPP__
#define __X_MAX_HPP__

typedef struct _maxobj
{
	t_object ob;
	void *myobj;
	t_atom **buf;
	size_t *n;
	int delegation_status;
	t_critical lock;
} t_maxobj;

namespace x
{
	namespace max
	{
		t_symbol *ps_generate = gensym("generate");
		t_symbol *ps_paramnames = gensym("paramnames");
		class obj
		{
		private:
			t_class *_c = NULL;
			void *_outlet_main = NULL, *_outlet_delegation = NULL;
		public:
			void obj_init(t_object *x, obj *o)
			{
				o->outlet_delegation(outlet_new((t_object *)x, NULL));
				o->outlet_main(outlet_new((t_object *)x, NULL));
			}

			void obj_init_without_delegation(t_object *x, obj *o)
			{
				o->outlet_main(outlet_new((t_object *)x, NULL));
			}
			
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_maxobj *x = (t_maxobj *)object_alloc(_c);
				x->myobj = NULL;
				x->buf = NULL;
				x->n = NULL;
				x->delegation_status = 0;
				critical_new(&(x->lock));
				return (t_object *)x;
			}

			static void msg_anything(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(!argc){
					object_error((t_object *)_x, "doesn't understand message %s", msg->s_name);
					return;
				}
				critical_enter(_x->lock);
				if(_x->buf){
					if(*(_x->buf)){
						if(*(_x->n) != argc){
							*(_x->buf) = (t_atom *)sysmem_resizeptr(*(_x->buf), sizeof(t_atom) * argc);
							*(_x->n) = argc;
						}
					}else{
						*(_x->n) = argc;
						*(_x->buf) = (t_atom *)sysmem_newptr(sizeof(t_atom) * argc);
					}
				}else{
					// this wasn't triggered by a delegation---ignore
					object_error((t_object *)_x, "doesn't understand message %s", msg->s_name);
					return;
				}
				memcpy(*(_x->buf), argv, argc * sizeof(t_atom));
				_x->delegation_status = 1;
				critical_exit(_x->lock);
			}

			static void msg_list(t_maxobj *x, t_symbol *msg, short argc, t_atom *argv)
			{
				msg_anything(x, msg, argc, argv);
			}

			static void msg_int(t_maxobj *x, long i)
			{
				t_atom a;
				atom_setlong(&a, i);
				msg_anything(x, NULL, 1, &a);
			}

			static void msg_float(t_maxobj *x, double f)
			{
				t_atom a;
				atom_setfloat(&a, f);
				msg_anything(x, NULL, 1, &a);
			}

			static void msg_bang(t_maxobj *x)
			{
			}

			static void msg_assist(t_maxobj *x, void *b, long io, long index, char *s)
			{
				switch (io) {
				case 1:
					switch (index) {
					case 0:
						strncpy_zero(s, "generate", 512);
						break;
					}
					break;
				case 2:
					switch(index){
					case 0:
						strncpy_zero(s, "Main outlet", 512);
						break;
					case 1:
						strncpy_zero(s, "Delegation outlet", 512);
						break;
					}
					break;
				}
			}

			int main(void)
			{
				class_addmethod(_c, (method)msg_anything, "anything", A_GIMME, 0);
				class_addmethod(_c, (method)msg_list, "list", A_GIMME, 0);
				class_addmethod(_c, (method)msg_int, "int", A_LONG, 0);
				class_addmethod(_c, (method)msg_float, "float", A_FLOAT, 0);
				class_addmethod(_c, (method)msg_bang, "bang", 0);
				class_addmethod(_c, (method)msg_assist, "assist", A_CANT, 0);
				return 0;
			}

			void *outlet_main(void)
			{
				return _outlet_main;
			}

			void outlet_main(void *o)
			{
				_outlet_main = o;
			}

			void *outlet_delegation(void)
			{
				return _outlet_delegation;
			}

			void outlet_delegation(void *o)
			{
				_outlet_delegation = o;
			}

			t_class *max_class(void)
			{
				return _c;
			}

			void max_class(t_class *c)
			{
				_c = c;
			}
		};

		template <typename T>
		typename std::enable_if<std::is_integral<T>::value, T>::type atom_get(t_atom *a)
		{
			return (T)atom_getlong(a);
		}

		template <typename T>
		typename std::enable_if<std::is_floating_point<T>::value, T>::type atom_get(t_atom *a)
		{
			return (T)atom_getfloat(a);
		}

		template <typename T, typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
		void atom_set(t_atom *a, T v)
		{
			atom_setlong(a, (long long)v);
		}

		template <typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr>
		void atom_set(t_atom *a, T v)
		{
			atom_setfloat(a, (double)v);
		}

		template <typename T>
		void atom_setv(T v, long *argc, t_atom **argv)
		{
			if(!(*argv)){
				*argv = (t_atom *)sysmem_newptr(sizeof(t_atom));
			}
			*argc = 1;
			atom_set(*argv, v);
		}

		template <typename T>
		void atom_setv(std::vector<T> v, long *argc, t_atom **argv)
		{
			size_t n = v.size();
			if(!(*argv)){
				*argv = (t_atom *)sysmem_newptr(sizeof(t_atom) * n);
			}
			*argc = n;
			for(size_t i = 0; i < n; i++){
				atom_set((*argv) + i, v[i]);
			}
		}

		template <typename T, bool multivariate>
		typename std::enable_if<std::is_integral<T>::value && multivariate, std::vector<T>>::type atom_getv(long argc, t_atom *argv)
		{
			std::vector<T> v(argc);
			for(size_t i = 0; i < argc; i++){
				v[i] = (T)atom_get<T>(argv + i);
			}
			return v;
		}

		template <typename T, bool multivariate>
		typename std::enable_if<std::is_floating_point<T>::value && multivariate, std::vector<T>>::type atom_getv(long argc, t_atom *argv)
		{
			std::vector<T> v(argc);
			for(size_t i = 0; i < argc; i++){
				v[i] = (T)atom_get<T>(argv + i);
			}
			return v;
		}

		template <typename T, bool multivariate>
		typename std::enable_if<std::is_integral<T>::value && !multivariate, T>::type atom_getv(long argc, t_atom *argv)
		{
			return (T)atom_get<T>(argv);
		}

		template <typename T, bool multivariate>
		typename std::enable_if<std::is_floating_point<T>::value && !multivariate, T>::type atom_getv(long argc, t_atom *argv)
		{
			return (T)atom_get<T>(argv);
		}

		class random_device_obj : public obj
		{
			x::random::random_device rd;
		public:
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *xx = obj::newobj(msg, argc, argv);
				t_maxobj *x = (t_maxobj *)xx;
				random_device_obj *o = new random_device_obj;
				obj::obj_init_without_delegation(xx, (obj *)o);
				x->myobj = (void *)o;
				t_dictionary *d = object_dictionaryarg(argc, argv);
				if(dictionary_hasentry(d, gensym(XGLOBAL_REGISTER))){
					object_subscribe(gensym("xglobal_conduit"), max_class()->c_sym, gensym("xglobal_conduit"), x);
				}
				return xx;
			}

			static void msg_generate(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				random_device_obj *x = (random_device_obj *)(_x->myobj);
				t_atom_long n = 1;
				if(argc == 1){
					n = atom_getlong(argv);
				}
				if(n == 1){
					outlet_int(x->outlet_main(), x->rd());
				}else{
#ifndef __clang__
					t_atom *out = (t_atom *)sysmem_newptr(n * sizeof(t_atom));
#else
					t_atom out[n];
#endif
					for(int i = 0; i < n; i++){
						atom_setlong(out + i, x->rd());
					}
					outlet_list(x->outlet_main(), _sym_list, n, out);
#ifndef __clang__
					sysmem_freeptr(out);
#endif
				}
			}

			static void msg_min(t_maxobj *_x)
			{
				random_device_obj *x = (random_device_obj *)(_x->myobj);
				t_atom a;
				atom_set(&a, x->rd.min());
				outlet_anything(x->outlet_main(), _sym_min, 1, &a);
				//outlet_int(x->outlet_main(), x->rd.min());
			}

			static void msg_max(t_maxobj *_x)
			{
				random_device_obj *x = (random_device_obj *)(_x->myobj);
				t_atom a;
				atom_set(&a, x->rd.max());
				outlet_anything(x->outlet_main(), _sym_max, 1, &a);
				//outlet_int(x->outlet_main(), x->rd.max());
			}

			static void freeobj(t_maxobj *x)
			{
				if(x->myobj){
					delete ((random_device_obj *)(x->myobj));
				}
			}

			static void notify(t_maxobj *_x, t_symbol *s, t_symbol *msg, t_xglobal_conduit *sender, void *data)
			{
				if(msg == gensym("sendmessage")){
					random_device_obj *x = (random_device_obj *)(_x->myobj);
					x->outlet_main(sender->outlet_main);
					//x->outlet_delegation(sender->outlet_delegation);
					t_atom rv;
					object_method_typed(_x, sender->msg, sender->argc, sender->argv, &rv);
				}
			}

			static void doc(t_maxobj *_x)
			{
				random_device_obj *x = (random_device_obj *)(_x->myobj);
				x::random::random_device d;
				t_atom as[3];
				t_symbol *doc = gensym("doc");
				t_symbol *desc = gensym("desc");
				// t_symbol *s = gensym("long");
				// t_symbol *desc = gensym(d.desc_long);
				// atom_setsym(as, s);
				// atom_setsym(as + 1, desc);
				// outlet_anything(x->outlet_main(), doc, 2, as);
					
				t_symbol *s = gensym("short");
				t_symbol *text = gensym(d.desc_short);
				atom_setsym(as, desc);
				atom_setsym(as + 1, s);
				atom_setsym(as + 2, text);
				outlet_anything(x->outlet_main(), doc, 3, as);

				atom_setsym(as, gensym("done"));
				outlet_anything(x->outlet_main(), doc, 1, as);
			}

			int main(void)
			{
				int ret = obj::main();
				class_addmethod(max_class(), (method)msg_generate, "generate", A_GIMME, 0);
				class_addmethod(max_class(), (method)msg_min, "min", 0);
				class_addmethod(max_class(), (method)msg_max, "max", 0);
				class_addmethod(max_class(), (method)notify, "notify", A_CANT, 0);
				class_addmethod(max_class(), (method)doc, "doc", 0);
				return ret;
			}
		};

		random_device_obj _random_device_obj;
		t_object *random_device_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _random_device_obj.newobj(msg, argc, argv);
		}		
		
		using random_device_delegate_base = x::proxy::delegate<uint_least32_t,
								       t_atom,
								       atom_get<uint_least32_t>,
								       atom_set<uint_least32_t>
								       >;
		using seed_seq_from_base = x::proxy::random_device_delegate<random_device_delegate_base>;
		using seed_seq_from_obj_base = x::proxy::seed_seq_from<seed_seq_from_base>;

		class seed_seq_from_obj : public obj
		{
		public:
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *x = obj::newobj(msg, argc, argv);
				if(x){
					t_maxobj *xx = (t_maxobj *)x;
					seed_seq_from_obj *o = new seed_seq_from_obj;
					obj::obj_init(x, (obj *)o);
					xx->myobj = (void *)o;
					return x;
				}
				return NULL;
			}
			
			static void msg_generate(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				obj *x = (obj *)(_x->myobj);
				size_t n = 1;
				if(argc && atom_gettype(argv) == A_LONG){
					n = atom_getlong(argv);
				}
				x::proxy::seed_seq_from<x::proxy::random_device_delegate<x::proxy::delegate<uint_least32_t, t_atom, atom_get<uint_least32_t>, atom_set/*<uint_least32_t>*/>>> seed_source;
#ifndef __clang__
				typename seed_seq_from_obj_base::result_type* buf = (seed_seq_from_obj_base::result_type*)sysmem_newptr(n * sizeof(seed_seq_from_obj_base::result_type));
				if (!buf) {
					object_error((t_object*)x, "ran out of memory!");
					return;
				}
				t_atom* out = (t_atom*)sysmem_newptr(n * sizeof(t_atom));
				if (!out) {
					object_error((t_object*)x, "ran out of memory!");
					return;
				}
#else
				typename seed_seq_from_obj_base::result_type buf[n];
				t_atom out[n];
#endif
				critical_enter(_x->lock);
				_x->n = seed_source.buffer_len_address();
				_x->buf = seed_source.buffer_address();
				seed_source.context(x->outlet_delegation());
				seed_source.generate(buf, buf + n);
				for(int i = 0; i < n; i++){
					atom_set(out + i, buf[i]);
				}
				outlet_list(x->outlet_main(), _sym_list, n, out);
				t_atom *ssbuf = seed_source.clear();
				if(ssbuf){
					sysmem_freeptr(ssbuf);
					_x->buf = NULL;
					_x->n = 0;
				}
				critical_exit(_x->lock);
#ifndef __clang__
				sysmem_freeptr(buf);
				sysmem_freeptr(out);
#endif
			}

			static void freeobj(t_maxobj *x)
			{
				if(x->buf){
					sysmem_freeptr(x->buf);
				}
				critical_free(x->lock);
				if(x->myobj){
					delete ((seed_seq_from_obj *)(x->myobj));
				}
			}

			int main(void)
			{
				int ret = obj::main();
				class_addmethod(max_class(), (method)msg_generate, "generate", A_GIMME, 0);
				return ret;
			}
		};

		seed_seq_from_obj _seed_seq_from_obj;
		t_object *seed_seq_from_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _seed_seq_from_obj.newobj(msg, argc, argv);
		}

		using seed_seq_from_delegate_base = x::proxy::delegate<uint_least32_t, t_atom, atom_get<uint_least32_t>, atom_set/*<uint_least32_t>*/>;
		using rng_obj_base = obj;
		
		template <typename rng_type>
		class rng_obj : public obj, public rng_type
		{
		public:
			rng_type _rng;
			bool _rng_is_valid = false;
		public:
			bool init_rng(t_maxobj *x)
			{
				x::proxy::seed_seq_from_delegate<seed_seq_from_delegate_base, x::random::random_device> seed_source;
				seed_source.context(outlet_delegation());
			        critical_enter(x->lock);
				x->buf = seed_source.buffer_address();
				x->n = seed_source.buffer_len_address();
				x->delegation_status = 0;

				rng_type rng = rng_type(seed_source);
				if(x->delegation_status > 0){
					_rng = rng;
					_rng_is_valid = true;
					x->delegation_status = 0;
				}
				t_atom *buf = seed_source.clear();
				if(buf){
					sysmem_freeptr(buf);
					x->buf = NULL;
					x->n = 0;
				}
				critical_exit(x->lock);
				return _rng_is_valid;
			}
			
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *x = obj::newobj(msg, argc, argv);
				if(x){
					t_maxobj *xx = (t_maxobj *)x;
					critical_new(&(xx->lock));
					rng_obj<rng_type> *o = new rng_obj<rng_type>;
					obj::obj_init(x, (obj *)o);
					xx->myobj = (void *)o;

					t_dictionary *d = object_dictionaryarg(argc, argv);
					if(dictionary_hasentry(d, gensym(XGLOBAL_REGISTER))){
						object_subscribe(gensym("xglobal_conduit"), max_class()->c_sym, gensym("xglobal_conduit"), x);
					}
					return (t_object *)x;
				}
				return NULL;
			}
			
			static void msg_generate(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				rng_obj<rng_type> *x = (rng_obj<rng_type> *)(_x->myobj);
				if(!x->_rng_is_valid){
					if(!x->init_rng(_x)){
						object_error((t_object *)_x, "rng has not been initialized. make sure to connect a seed_seq or seed_seq_from object.");
						return;
					}
				}
				size_t n = 1;
				if(argc && atom_gettype(argv) == A_LONG){
					n = atom_getlong(argv);
				}
#ifndef __clang__
				t_atom *out = (t_atom *)sysmem_newptr(n * sizeof(t_atom));
#else
				t_atom out[n];
#endif
				for(int i = 0; i < n; i++){
					long v = x->_rng();
					atom_set(out + i, x->_rng());
				}
				outlet_list(x->outlet_main(), _sym_list, n, out);
#ifndef __clang__
				sysmem_freeptr(out);
#endif
			}

			static void msg_min(t_maxobj *_x)
			{
				rng_obj<rng_type> *x = (rng_obj<rng_type> *)(_x->myobj);
				t_atom a;
				atom_set(&a, x->min());
				outlet_anything(x->outlet_main(), _sym_min, 1, &a);
			}

			static void msg_max(t_maxobj *_x)
			{
				rng_obj<rng_type> *x = (rng_obj<rng_type> *)(_x->myobj);
				t_atom a;
				atom_set(&a, x->max());
				outlet_anything(x->outlet_main(), _sym_max, 1, &a);
			}

			static void freeobj(t_maxobj *x)
			{
				if(x->buf){
					sysmem_freeptr(x->buf);
				}
				critical_free(x->lock);				
				if(x->myobj){
					delete ((rng_obj<rng_type> *)(x->myobj));
				}
			}

			static void doc(t_maxobj *_x)
			{
				rng_obj<rng_type> *x = (rng_obj<rng_type> *)(_x->myobj);
				rng_type d;
				t_atom as[3];
				t_symbol *doc = gensym("doc");
				t_symbol *desc = gensym("desc");
				// t_symbol *s = gensym("long");
				// t_symbol *desc = gensym(d.desc_long);
				// atom_setsym(as, s);
				// atom_setsym(as + 1, desc);
				// outlet_anything(x->outlet_main(), doc, 2, as);

				t_symbol *s = gensym("short");
				t_symbol *text = gensym(d.desc_short);
				atom_setsym(as, desc);
				atom_setsym(as + 1, s);
				atom_setsym(as + 2, text);
				outlet_anything(x->outlet_main(), doc, 3, as);

				atom_setsym(as, gensym("done"));
				outlet_anything(x->outlet_main(), doc, 1, as);
			}

			static void notify(t_maxobj *_x, t_symbol *s, t_symbol *msg, t_xglobal_conduit *sender, void *data)
			{
				if(msg == gensym("sendmessage")){
					rng_obj<rng_type> *x = (rng_obj<rng_type> *)(_x->myobj);
					x->outlet_main(sender->outlet_main);
					x->outlet_delegation(sender->outlet_delegation);
					t_atom rv;
					object_method_typed(_x, sender->msg, sender->argc, sender->argv, &rv);
				}
			}

			int main(void)
			{
				rng_obj_base::main();
				class_addmethod(max_class(), (method)msg_generate, "generate", A_GIMME, 0);
				class_addmethod(max_class(), (method)msg_min, "min", 0);
				class_addmethod(max_class(), (method)msg_max, "max", 0);
				class_addmethod(max_class(), (method)notify, "notify", A_CANT, 0);
				class_addmethod(max_class(), (method)doc, "doc", 0);
				return 0;
			}
		};
		using generator_pcg32_obj = rng_obj<x::random::pcg32>;
		generator_pcg32_obj _generator_pcg32_obj;
		t_object *generator_pcg32_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_pcg32_obj.newobj(msg, argc, argv);
		}

		using generator_pcg64_obj = rng_obj<x::random::pcg64>;
		generator_pcg64_obj _generator_pcg64_obj;
		t_object *generator_pcg64_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_pcg64_obj.newobj(msg, argc, argv);
		}

		using generator_minstd_rand0_obj = rng_obj<x::random::minstd_rand0>;
		generator_minstd_rand0_obj _generator_minstd_rand0_obj;
		t_object *generator_minstd_rand0_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_minstd_rand0_obj.newobj(msg, argc, argv);
		}

		using generator_minstd_rand_obj = rng_obj<x::random::minstd_rand>;
		generator_minstd_rand_obj _generator_minstd_rand_obj;
		t_object *generator_minstd_rand_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_minstd_rand_obj.newobj(msg, argc, argv);
		}

		using generator_mt19937_obj = rng_obj<x::random::mt19937>;
		generator_mt19937_obj _generator_mt19937_obj;
		t_object *generator_mt19937_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_mt19937_obj.newobj(msg, argc, argv);
		}

		using generator_mt19937_64_obj = rng_obj<x::random::mt19937_64>;
		generator_mt19937_64_obj _generator_mt19937_64_obj;
		t_object *generator_mt19937_64_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_mt19937_64_obj.newobj(msg, argc, argv);
		}

		using generator_ranlux24_obj = rng_obj<x::random::ranlux24>;
		generator_ranlux24_obj _generator_ranlux24_obj;
		t_object *generator_ranlux24_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_ranlux24_obj.newobj(msg, argc, argv);
		}

		using generator_ranlux48_obj = rng_obj<x::random::ranlux48>;
		generator_ranlux48_obj _generator_ranlux48_obj;
		t_object *generator_ranlux48_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_ranlux48_obj.newobj(msg, argc, argv);
		}

		using generator_knuth_b_obj = rng_obj<x::random::knuth_b>;
		generator_knuth_b_obj _generator_knuth_b_obj;
		t_object *generator_knuth_b_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_knuth_b_obj.newobj(msg, argc, argv);
		}
		
		using rng_delegate_uint32 = x::proxy::delegate<uint32_t, t_atom, x::max::atom_get<uint32_t>, x::max::atom_set>;
		using rng_delegate_uint64 = x::proxy::delegate<uint64_t, t_atom, x::max::atom_get<uint64_t>, x::max::atom_set>;

		template <typename xparam_type, typename result_type, const char param1_name[], typename param1_type, bool param1_vec>
		class param_type_1 : public xparam_type
		{
		public:
			int nargs = 1;
			const char *names_str[1] = {param1_name};
			t_symbol *names_sym[1] = {gensym(param1_name)};
			static void p1(xparam_type *p, long *argc, t_atom **argv)
			{
				atom_setv(p->param1(), argc, argv);
			}
			static void p1(xparam_type *p, long argc, t_atom *argv)
			{
				*p = xparam_type(atom_getv<param1_type, param1_vec>(argc, argv));
			}
			void (*getters[1])(xparam_type*, long*, t_atom**) = {p1};
			void (*setters[1])(xparam_type*, long, t_atom*) = {p1};
		};

		template <typename xparam_type, typename result_type, const char param1_name[], typename param1_type, bool param1_vec, const char param2_name[], typename param2_type, bool param2_vec>
		class param_type_2 : public xparam_type
		{
		public:
			int nargs = 2;
			const char *names_str[2] = {param1_name, param2_name};
			t_symbol *names_sym[2] = {gensym(param1_name), gensym(param2_name)};
			static void p1(xparam_type *p, long *argc, t_atom **argv)
			{
				atom_setv(p->param1(), argc, argv);
			}
			static void p1(xparam_type *p, long argc, t_atom *argv)
			{
				*p = xparam_type(atom_getv<param1_type, param1_vec>(argc, argv), p->param2());
			}
			static void p2(xparam_type *p, long *argc, t_atom **argv)
			{
				atom_setv(p->param2(), argc, argv);
			}
			static void p2(xparam_type *p, long argc, t_atom *argv)
			{
				*p = xparam_type(p->param1(), atom_getv<param2_type, param2_vec>(argc, argv));
			}
			void (*getters[2])(xparam_type*, long*, t_atom**) = {p1, p2};
			void (*setters[2])(xparam_type*, long, t_atom*) = {p1, p2};
		};

		template <typename xparam_type, typename result_type, const char param1_name[], typename param1_type, bool param1_vec, const char param2_name[], typename param2_type, bool param2_vec, const char param3_name[], typename param3_type, bool param3_vec>
		class param_type_3 : public xparam_type
		{
		public:
			int nargs = 3;
			const char *names_str[3] = {param1_name, param2_name, param3_name};
			t_symbol *names_sym[3] = {gensym(param1_name), gensym(param2_name), gensym(param3_name)};
			static void p1(xparam_type *p, long *argc, t_atom **argv)
			{
				atom_setv(p->param1(), argc, argv);
			}
			static void p1(xparam_type *p, long argc, t_atom *argv)
			{
				*p = xparam_type(atom_getv<param1_type, param1_vec>(argc, argv), p->param2(), p->param3());
			}
			static void p2(xparam_type *p, long *argc, t_atom **argv)
			{
				atom_setv(p->param2(), argc, argv);
			}
			static void p2(xparam_type *p, long argc, t_atom *argv)
			{
				*p = xparam_type(p->param1(), atom_getv<param2_type, param2_vec>(argc, argv), p->param3());
			}
			static void p3(xparam_type *p, long *argc, t_atom **argv)
			{
				atom_setv(p->param3(), argc, argv);
			}
			static void p3(xparam_type *p, long argc, t_atom *argv)
			{
				*p = xparam_type(p->param1(), p->param2(), atom_getv<param3_type, param3_vec>(argc, argv));
			}
			void (*getters[3])(xparam_type*, long*, t_atom**) = {p1, p2, p3};
			void (*setters[3])(xparam_type*, long, t_atom*) = {p1, p2, p3};
		};

		const char alpha_str[] = "alpha";
		const char beta_str[] = "beta";
		const char mean_str[] = "mean";
		const char lambda_str[] = "lambda";
		const char stddev_str[] = "stddev";
		const char probabilities_str[] = "probabilities";
		const char intervals_str[] = "intervals";
		const char densities_str[] = "densities";
		const char mu_str[] = "mu";
		const char rho_str[] = "rho";
		const char sigma_str[] = "sigma";
		const char sigmax_str[] = "sigmax";
		const char sigmay_str[] = "sigmay";
		const char a_str[] = "a";
		const char b_str[] = "b";
		const char k_str[] = "k";
		const char M_str[] = "M";
		const char m_str[] = "m";
		const char N_str[] = "N";
		const char n_str[] = "n";
		const char p_str[] = "p";
		const char s_str[] = "s";
		const char t_str[] = "t";
		
		template <typename dist_type, typename result_type, bool multivariate=false, typename xparam_type=param_type_2<x::random::uniform_real_distribution_param_type, double, a_str, double, false, b_str, double, false>>
			class dist_obj : public obj, public dist_type, public xparam_type
			{
			public:
				t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
				{
					t_object *x = obj::newobj(msg, argc, argv);
					if(x){
						t_maxobj *xx = (t_maxobj *)x;
						dist_obj<dist_type, result_type, multivariate, xparam_type> *o = new dist_obj<dist_type, result_type, multivariate, xparam_type>;
						obj::obj_init(x, (obj *)o);
						xx->myobj = (void *)o;
						critical_new(&(xx->lock));
						t_dictionary *d = object_dictionaryarg(argc, argv);
						for(int i = 0; i < o->nargs; i++){
							t_symbol *s = gensym(o->names_str[i]);
							if(dictionary_hasentry(d, s)){
								long ac = 0;
								t_atom *av = NULL;
								dictionary_getatoms(d, s, &ac, &av);
								o->setters[i](o, ac, av);
							}
						}
						return x;
					}
					return NULL;
				}

				template <typename delegate_type>
				void init_delegate(t_maxobj *_x, delegate_type *d)
				{
					d->context(outlet_delegation());
					_x->n = d->buffer_len_address();
					_x->buf = d->buffer_address();
				}

				template <typename delegate_type>
				void finalize_delegate(t_maxobj *_x, delegate_type *d)
				{
					if(_x->buf){
						if(*(_x->buf)){
							sysmem_freeptr(*(_x->buf));
						}
					}
					_x->buf = NULL;
					_x->n = NULL;
				}

				template <typename rng_type, bool U=multivariate>
				static typename std::enable_if<!U>::type _generate(t_maxobj *_x, dist_type d, rng_type *rng)
				{
					critical_enter(_x->lock);
					((dist_obj<dist_type, result_type> *)(_x->myobj))->init_delegate(_x, (rng_delegate_uint64 *)rng);
					t_atom a;
					atom_set(&a, d(*rng));
					outlet_atoms(((dist_obj<dist_type, result_type> *)(_x->myobj))->outlet_main(), 1, &a);
					((dist_obj<dist_type, result_type> *)(_x->myobj))->finalize_delegate(_x, (rng_delegate_uint64 *)rng);
					critical_exit(_x->lock);
				}

				template <typename rng_type, bool U=multivariate>
				static typename std::enable_if<U>::type _generate(t_maxobj *_x, dist_type d, rng_type *rng)
				{
					((dist_obj<dist_type, result_type> *)(_x->myobj))->init_delegate(_x, (rng_delegate_uint64 *)rng);
					std::vector<result_type> vec = d(*rng);
					size_t n = vec.size();
#ifndef __clang__
					t_atom *a = (t_atom *)sysmem_newptr(sizeof(t_atom) * n);
#else
					t_atom a[n];
#endif
					for(int i = 0; i < n; i++){
						atom_set(a + i, vec[i]);
					}
					outlet_atoms(((dist_obj<dist_type, result_type> *)(_x->myobj))->outlet_main(), n, a);
					((dist_obj<dist_type, result_type> *)(_x->myobj))->finalize_delegate(_x, (rng_delegate_uint64 *)rng);
#ifndef __clang__
					sysmem_freeptr(a);
#endif
				}
			
				static void generate(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
				{
					dist_obj<dist_type, result_type, multivariate, xparam_type> *x = (dist_obj<dist_type, result_type, multivariate, xparam_type> *)(_x->myobj);
					dist_type d = dist_type(*((xparam_type *)x));
					size_t n = 1;
					critical_enter(_x->lock);
					_x->n = &n;
					t_atom mina, maxa;
					t_atom *p = &mina;
					_x->buf = &p;
					outlet_anything(x->outlet_delegation(), _sym_min, 0, NULL);
					p = &maxa;
					_x->buf = &p;
					outlet_anything(x->outlet_delegation(), _sym_max, 0, NULL);
					uint64_t min = atom_getlong(&mina);
					uint64_t max = atom_getlong(&maxa);
					if(max == 0xFFFFFF){
						switch(min){
						case 0:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFF> rng;
								_generate(_x, d, &rng);
							}
							break;
						case 1:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFF> rng;
								_generate(_x, d, &rng);
							}
							break;
						}
					}else if(max == 0x7FFFFFFE){
						switch(min){
						case 0:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0x7FFFFFFE> rng;
								_generate(_x, d, &rng);
							}
							break;
						case 1:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0x7FFFFFFE> rng;
								_generate(_x, d, &rng);
							}
							break;
						}
					}else if(max == 0xFFFFFFFF){
						switch(min){
						case 0:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFF> rng;
								_generate(_x, d, &rng);
							}
							break;
						case 1:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFF> rng;
								_generate(_x, d, &rng);
							}
							break;
						}
					}else if(max == 0xFFFFFFFFFFFF){
						switch(min){
						case 0:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFF> rng;
								_generate(_x, d, &rng);
							}
							break;
						case 1:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFF> rng;
								_generate(_x, d, &rng);
							}
							break;
						}
					}else if(max == 0xFFFFFFFFFFFFFFFF){
						switch(min){
						case 0:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFFFFFF> rng;
								_generate(_x, d, &rng);
							}
							break;
						case 1:
							{
								x::proxy::rng_delegate<rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFFFFFF> rng;
								_generate(_x, d, &rng);
							}
							break;
						}
					}else{
					}
					critical_exit(_x->lock);
				}

				static void min(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
				{
					if(argc){
						msg_anything(_x, msg, argc, argv);
						return;
					}
					dist_obj<dist_type, result_type, multivariate, xparam_type> *x = (dist_obj<dist_type, result_type, multivariate, xparam_type> *)(_x->myobj);
					dist_type d = dist_type(*((xparam_type *)x));
					t_atom a;
					atom_set(&a, d.min());
					outlet_anything(((dist_obj<dist_type, result_type> *)(_x->myobj))->outlet_main(), _sym_min, 1, &a);
				}

				static void max(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
				{
					if(argc){
						msg_anything(_x, msg, argc, argv);
						return;
					}
					dist_obj<dist_type, result_type, multivariate, xparam_type> *x = (dist_obj<dist_type, result_type, multivariate, xparam_type> *)(_x->myobj);
					dist_type d = dist_type(*((xparam_type *)x));
					t_atom a;
					atom_set(&a, d.max());
					outlet_anything(((dist_obj<dist_type, result_type> *)(_x->myobj))->outlet_main(), _sym_max, 1, &a);
				}

				static void paramnames(t_maxobj *_x)
				{
					dist_obj<dist_type, result_type, multivariate, xparam_type> *x = (dist_obj<dist_type, result_type, multivariate, xparam_type> *)(_x->myobj);
#ifndef __clang__
					t_atom *a = (t_atom *)sysmem_newptr(x->nargs * sizeof(t_atom));
#else
					t_atom a[x->nargs];
#endif
					if (!a) {
						object_error((t_object*)x, "out of memory!");
						return;
					}
					for(int i = 0; i < x->nargs; i++){
						atom_setsym(a + i, x->names_sym[i]);
					}
					outlet_anything(x->outlet_main(), ps_paramnames, x->nargs, a);
#ifndef __clang__
					sysmem_freeptr(a);
#endif
				}

				static void freeobj(t_maxobj *x)
				{
					if(x->buf){
						sysmem_freeptr(x->buf);
					}
					critical_free(x->lock);				
					if(x->myobj){
						delete ((dist_obj<dist_type, result_type> *)(x->myobj));
					}
				}

				static void param(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
				{
					dist_obj<dist_type, result_type, multivariate, xparam_type> *x = (dist_obj<dist_type, result_type, multivariate, xparam_type> *)(_x->myobj);
					t_symbol *name = msg;
					for(int i = 0; i < x->nargs; i++){
						if(name == x->names_sym[i]){
							if(argc){
								x->setters[i](x, argc, argv);
							}else{
								long ac = 0;
								t_atom *av = NULL;
								x->getters[i](x, &ac, &av);
								outlet_anything(x->outlet_main(), name, ac, av);
								if(av){
									sysmem_freeptr(av);
								}
							}
							break;
						}
					}
				}

				static t_max_err attr_get(t_maxobj *_x, t_object *attr, long *argc, t_atom **argv)
				{
					dist_obj<dist_type, result_type, multivariate, xparam_type> *x = (dist_obj<dist_type, result_type, multivariate, xparam_type> *)(_x->myobj);
					t_symbol *name = (t_symbol *)object_method(attr, gensym("getname"));
					for(int i = 0; i < x->nargs; i++){
						if(!strncmp(name->s_name, x->names_sym[i]->s_name, strlen(name->s_name) - 1)){
							x->getters[i](x, argc, argv);
							break;
						}
					}
					return MAX_ERR_NONE;
				}

				static t_max_err attr_set(t_maxobj *_x, t_object *attr, long argc, t_atom *argv)
				{
					dist_obj<dist_type, result_type, multivariate, xparam_type> *x = (dist_obj<dist_type, result_type, multivariate, xparam_type> *)(_x->myobj);
					t_symbol *name = (t_symbol *)object_method(attr, gensym("getname"));
					for(int i = 0; i < ((xparam_type *)x)->nargs; i++){
						if(!strncmp(name->s_name, x->names_sym[i]->s_name, strlen(name->s_name) - 1)){
							x->setters[i](x, argc, argv);
							break;
						}
					}
					return 0;
				}

				static void doc(t_maxobj *_x)
				{
					dist_obj<dist_type, result_type, multivariate, xparam_type> *x = (dist_obj<dist_type, result_type, multivariate, xparam_type> *)(_x->myobj);
					dist_type d = dist_type(*((xparam_type *)x));
				        t_atom as[3];
					t_symbol *doc = gensym("doc");
					t_symbol *desc = gensym("desc");
					// t_symbol *s = gensym("long");
					// t_symbol *desc = gensym(d.desc_long);
					// atom_setsym(as, s);
					// atom_setsym(as + 1, desc);
					// outlet_anything(x->outlet_main(), doc, 2, as);
					
					t_symbol *s = gensym("short");
					t_symbol *text = gensym(d.desc_short);
					atom_setsym(as, desc);
					atom_setsym(as + 1, s);
					atom_setsym(as + 2, text);
					outlet_anything(x->outlet_main(), doc, 3, as);

					desc = gensym("param");
					atom_setsym(as, desc);
					for(int i = 0; i < d.nparams; i++){
						s = ((xparam_type *)x)->names_sym[i];
						text = gensym(d.param_desc_list[i]);
						atom_setsym(as + 1, s);
						atom_setsym(as + 2, text);
						outlet_anything(x->outlet_main(), doc, 3, as);
					}

					atom_setsym(as, gensym("done"));
					outlet_anything(x->outlet_main(), doc, 1, as);
				}

				int main(void)
				{
					obj::main();
				
					t_class *c = max_class();
					class_addmethod(c, (method)generate, "generate", A_GIMME, 0);
					class_addmethod(c, (method)min, "min", A_GIMME, 0);
					class_addmethod(c, (method)max, "max", A_GIMME, 0);
					class_addmethod(c, (method)paramnames, "paramnames", 0);
					for(int i = 0; i < xparam_type::nargs; i++){
						class_addmethod(c, (method)param, xparam_type::names_str[i], A_GIMME, 0);
						const char *name = (std::string(xparam_type::names_str[i]) + std::string(" ")).c_str();
						class_addattr(c, attr_offset_new(name, gensym("atom"), 0, (method)0L,(method)0L,calcoffset(t_maxobj, ob)));
					
						t_object *theattr = (t_object *)class_attr_get(c, gensym(name));
						object_method(theattr, gensym("setmethod"), USESYM(get), attr_get);
						object_method(theattr, gensym("setmethod"), USESYM(set), attr_set);
					}
					class_addmethod(c, (method)doc, "doc", 0);
					return 0;
				}
			};

		// Uniform
		using dist_uniform_int_obj = dist_obj<x::random::uniform_int_distribution<long>, long, false, param_type_2<x::random::uniform_int_distribution_param_type, long, a_str, long, false, b_str, long, false>>;
		dist_uniform_int_obj _dist_uniform_int_obj;
		t_object *dist_uniform_int_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_uniform_int_obj.newobj(msg, argc, argv);
		}

		using dist_uniform_real_obj = dist_obj<x::random::uniform_real_distribution<double>, double, false, param_type_2<x::random::uniform_real_distribution_param_type, double, a_str, double, false, b_str, double, false>>;
		dist_uniform_real_obj _dist_uniform_real_obj;
		t_object *dist_uniform_real_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_uniform_real_obj.newobj(msg, argc, argv);
		}

		// Related to Bernoulli trials
		using dist_bernoulli_obj = dist_obj<x::random::bernoulli_distribution<long>, long, false, param_type_1<x::random::bernoulli_distribution_param_type, long, p_str, double, false>>;
		dist_bernoulli_obj _dist_bernoulli_obj;
		t_object *dist_bernoulli_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_bernoulli_obj.newobj(msg, argc, argv);
		}

		using dist_binomial_obj = dist_obj<x::random::binomial_distribution<long>, long, false, param_type_2<x::random::binomial_distribution_param_type, long, t_str, long, false, p_str, double, false>>;
		dist_binomial_obj _dist_binomial_obj;
		t_object *dist_binomial_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_binomial_obj.newobj(msg, argc, argv);
		}

		using dist_geometric_obj = dist_obj<x::random::geometric_distribution<long>, long, false, param_type_1<x::random::geometric_distribution_param_type, long, p_str, double, false>>;
		dist_geometric_obj _dist_geometric_obj;
		t_object *dist_geometric_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_geometric_obj.newobj(msg, argc, argv);
		}

		using dist_negative_binomial_obj = dist_obj<x::random::negative_binomial_distribution<long>, long, false, param_type_2<x::random::negative_binomial_distribution_param_type, long, k_str, long, false, p_str, double, false>>;
		dist_negative_binomial_obj _dist_negative_binomial_obj;
		t_object *dist_negative_binomial_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_negative_binomial_obj.newobj(msg, argc, argv);
		}

		using dist_multinomial_obj = dist_obj<x::random::multinomial_distribution<long>, long, true, param_type_2<x::random::multinomial_distribution_param_type, long, n_str, long, false, p_str, double, true>>;
		dist_multinomial_obj _dist_multinomial_obj;
		t_object *dist_multinomial_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_multinomial_obj.newobj(msg, argc, argv);
		}

		using dist_hypergeometric_obj = dist_obj<x::random::hypergeometric_distribution<long>, long, false, param_type_3<x::random::hypergeometric_distribution_param_type, long, n_str, long, false, M_str, long, false, N_str, long, false>>;
		dist_hypergeometric_obj _dist_hypergeometric_obj;
		t_object *dist_hypergeometric_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_hypergeometric_obj.newobj(msg, argc, argv);
		}

		using dist_multivariate_hypergeometric_obj = dist_obj<x::random::multivariate_hypergeometric_distribution<long>, long, true, param_type_2<x::random::multivariate_hypergeometric_distribution_param_type, long, n_str, long, false, M_str, long, true>>;
		dist_multivariate_hypergeometric_obj _dist_multivariate_hypergeometric_obj;
		t_object *dist_multivariate_hypergeometric_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_multivariate_hypergeometric_obj.newobj(msg, argc, argv);
		}
		
		// Rate-based distributions
		using dist_poisson_obj = dist_obj<x::random::poisson_distribution<long>, long, false, param_type_1<x::random::poisson_distribution_param_type, long, mean_str, double, false>>;
		dist_poisson_obj _dist_poisson_obj;
		t_object *dist_poisson_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_poisson_obj.newobj(msg, argc, argv);
		}

		using dist_exponential_obj = dist_obj<x::random::exponential_distribution<double>, double, false, param_type_1<x::random::exponential_distribution_param_type, double, lambda_str, double, false>>;
		dist_exponential_obj _dist_exponential_obj;
		t_object *dist_exponential_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_exponential_obj.newobj(msg, argc, argv);
		}
		
		using dist_gamma_obj = dist_obj<x::random::gamma_distribution<double>, double, false, param_type_2<x::random::gamma_distribution_param_type, double, alpha_str, double, false, beta_str, double, false>>;
		dist_gamma_obj _dist_gamma_obj;
		t_object *dist_gamma_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_gamma_obj.newobj(msg, argc, argv);
		}
		
		using dist_weibull_obj = dist_obj<x::random::weibull_distribution<double>, double, false, param_type_2<x::random::weibull_distribution_param_type, double, a_str, double, false, b_str, double, false>>;
		dist_weibull_obj _dist_weibull_obj;
		t_object *dist_weibull_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_weibull_obj.newobj(msg, argc, argv);
		}

		using dist_extreme_value_obj = dist_obj<x::random::extreme_value_distribution<double>, double, false, param_type_2<x::random::extreme_value_distribution_param_type, double, a_str, double, false, b_str, double, false>>;
		dist_extreme_value_obj _dist_extreme_value_obj;
		t_object *dist_extreme_value_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_extreme_value_obj.newobj(msg, argc, argv);
		}

		using dist_dirichlet_obj = dist_obj<x::random::dirichlet_distribution<double>, double, true, param_type_1<x::random::dirichlet_distribution_param_type, double, alpha_str, double, true>>;
		dist_dirichlet_obj _dist_dirichlet_obj;
		t_object *dist_dirichlet_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_dirichlet_obj.newobj(msg, argc, argv);
		}

		using dist_beta_obj = dist_obj<x::random::beta_distribution<double>, double, false, param_type_2<x::random::beta_distribution_param_type, double, alpha_str, double, false, beta_str, double, false>>;
		dist_beta_obj _dist_beta_obj;
		t_object *dist_beta_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_beta_obj.newobj(msg, argc, argv);
		}

		using dist_kumaraswamy_obj = dist_obj<x::random::kumaraswamy_distribution<double>, double, false, param_type_2<x::random::kumaraswamy_distribution_param_type, double, alpha_str, double, false, beta_str, double, false>>;
		dist_kumaraswamy_obj _dist_kumaraswamy_obj;
		t_object *dist_kumaraswamy_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_kumaraswamy_obj.newobj(msg, argc, argv);
		}

		using dist_laplace_obj = dist_obj<x::random::laplace_distribution<double>, double, false, param_type_2<x::random::laplace_distribution_param_type, double, mu_str, double, false, sigma_str, double, false>>;
		dist_laplace_obj _dist_laplace_obj;
		t_object *dist_laplace_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_laplace_obj.newobj(msg, argc, argv);
		}

		using dist_erlang_obj = dist_obj<x::random::erlang_distribution<double>, double, false, param_type_2<x::random::erlang_distribution_param_type, double, k_str, long, false, lambda_str, double, false>>;
		dist_erlang_obj _dist_erlang_obj;
		t_object *dist_erlang_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_erlang_obj.newobj(msg, argc, argv);
		}

		// Related to Normal distribution
		using dist_normal_obj = dist_obj<x::random::normal_distribution<double>, double, false, param_type_2<x::random::normal_distribution_param_type, double, mean_str, double, false, stddev_str, double, false>>;
		dist_normal_obj _dist_normal_obj;
		t_object *dist_normal_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_normal_obj.newobj(msg, argc, argv);
		}

		using dist_gaussian_tail_obj = dist_obj<x::random::gaussian_tail_distribution<double>, double, false, param_type_2<x::random::gaussian_tail_distribution_param_type, double, a_str, double, false, sigma_str, double, false>>;
		dist_gaussian_tail_obj _dist_gaussian_tail_obj;
		t_object *dist_gaussian_tail_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_gaussian_tail_obj.newobj(msg, argc, argv);
		}

		using dist_bivariate_normal_obj = dist_obj<x::random::bivariate_normal_distribution<double>, double, true, param_type_3<x::random::bivariate_normal_distribution_param_type, double, sigmax_str, double, false, sigmay_str, double, false, rho_str, double, false>>;
		dist_bivariate_normal_obj _dist_bivariate_normal_obj;
		t_object *dist_bivariate_normal_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_bivariate_normal_obj.newobj(msg, argc, argv);
		}

		using dist_exponential_power_obj = dist_obj<x::random::exponential_power_distribution<double>, double, false, param_type_2<x::random::exponential_power_distribution_param_type, double, alpha_str, double, false, beta_str, double, false>>;
		dist_exponential_power_obj _dist_exponential_power_obj;
		t_object *dist_exponential_power_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_exponential_power_obj.newobj(msg, argc, argv);
		}

		using dist_rayleigh_tail_obj = dist_obj<x::random::rayleigh_tail_distribution<double>, double, false, param_type_2<x::random::rayleigh_tail_distribution_param_type, double, a_str, double, false, sigma_str, double, false>>;
		dist_rayleigh_tail_obj _dist_rayleigh_tail_obj;
		t_object *dist_rayleigh_tail_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_rayleigh_tail_obj.newobj(msg, argc, argv);
		}

		using dist_logistic_obj = dist_obj<x::random::logistic_distribution<double>, double, false, param_type_2<x::random::logistic_distribution_param_type, double, mu_str, double, false, s_str, double, false>>;
		dist_logistic_obj _dist_logistic_obj;
		t_object *dist_logistic_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_logistic_obj.newobj(msg, argc, argv);
		}

		using dist_lognormal_obj = dist_obj<x::random::lognormal_distribution<double>, double, false, param_type_2<x::random::lognormal_distribution_param_type, double, m_str, double, false, s_str, double, false>>;
		dist_lognormal_obj _dist_lognormal_obj;
		t_object *dist_lognormal_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_lognormal_obj.newobj(msg, argc, argv);
		}

		using dist_chi_squared_obj = dist_obj<x::random::chi_squared_distribution<double>, double, false, param_type_1<x::random::chi_squared_distribution_param_type, double, n_str, double, false>>;
		dist_chi_squared_obj _dist_chi_squared_obj;
		t_object *dist_chi_squared_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_chi_squared_obj.newobj(msg, argc, argv);
		}

		using dist_cauchy_obj = dist_obj<x::random::cauchy_distribution<double>, double, false, param_type_2<x::random::cauchy_distribution_param_type, double, a_str, double, false, b_str, double, false>>;
		dist_cauchy_obj _dist_cauchy_obj;
		t_object *dist_cauchy_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_cauchy_obj.newobj(msg, argc, argv);
		}

		using dist_fisher_f_obj = dist_obj<x::random::fisher_f_distribution<double>, double, false, param_type_2<x::random::fisher_f_distribution_param_type, double, m_str, double, false, n_str, double, false>>;
		dist_fisher_f_obj _dist_fisher_f_obj;
		t_object *dist_fisher_f_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_fisher_f_obj.newobj(msg, argc, argv);
		}

		using dist_student_t_obj = dist_obj<x::random::student_t_distribution<double>, double, false, param_type_1<x::random::student_t_distribution_param_type, double, n_str, double, false>>;
		dist_student_t_obj _dist_student_t_obj;
		t_object *dist_student_t_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_student_t_obj.newobj(msg, argc, argv);
		}

		using dist_rayleigh_obj = dist_obj<x::random::rayleigh_distribution<double>, double, false, param_type_1<x::random::rayleigh_distribution_param_type, double, sigma_str, double, false>>;
		dist_rayleigh_obj _dist_rayleigh_obj;
		t_object *dist_rayleigh_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_rayleigh_obj.newobj(msg, argc, argv);
		}

		using dist_discrete_obj = dist_obj<x::random::discrete_distribution<long>, long, false, param_type_1<x::random::discrete_distribution_param_type, long, probabilities_str, double, true>>;
		dist_discrete_obj _dist_discrete_obj;
		t_object *dist_discrete_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_discrete_obj.newobj(msg, argc, argv);
		}

		using dist_piecewise_constant_obj = dist_obj<x::random::piecewise_constant_distribution<double>, double, false, param_type_2<x::random::piecewise_constant_distribution_param_type, double, intervals_str, double, true, densities_str, double, true>>;
		dist_piecewise_constant_obj _dist_piecewise_constant_obj;
		t_object *dist_piecewise_constant_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_piecewise_constant_obj.newobj(msg, argc, argv);
		}

		using dist_piecewise_linear_obj = dist_obj<x::random::piecewise_linear_distribution<double>, double, false, param_type_2<x::random::piecewise_linear_distribution_param_type, double, intervals_str, double, true, densities_str, double, true>>;
		dist_piecewise_linear_obj _dist_piecewise_linear_obj;
		t_object *dist_piecewise_linear_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_piecewise_linear_obj.newobj(msg, argc, argv);
		}

		t_class *newclass(const char *name, t_object *(*newobj)(t_symbol*, short, t_atom*), void (*freeobj)(t_maxobj*), size_t struct_size)
		{
			t_class *c = class_new(name, (method)newobj, (method)freeobj, struct_size, 0L, A_GIMME, 0);
			c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
			common_symbols_init();
			return c;
		}
	}
}

template<> void x::proxy::random_device_delegate<x::max::random_device_delegate_base>::callback(x::max::random_device_delegate_base *o, size_t n)
{
	t_atom a;
	atom_setlong(&a, n);
	outlet_anything(o->context(), x::max::ps_generate, 1, &a);
}

template<> void x::proxy::seed_seq_from_delegate<x::max::random_device_delegate_base, x::random::random_device>::callback(x::max::random_device_delegate_base *o, size_t n)
{
	t_atom a;
	atom_setlong(&a, n);
	outlet_anything(o->context(), x::max::ps_generate, 1, &a);
}

void rng_delegate_callback(void *o_context, size_t n)
{
	t_atom a;
	atom_setlong(&a, n);
	outlet_anything(o_context, x::max::ps_generate, 1, &a);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 0, 0xFFFFFF>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 1, 0xFFFFFF>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 0, 0x7FFFFFFE>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 1, 0x7FFFFFFE>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFF>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFF>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFF>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFF>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 0, 0xFFFFFFFFFFFFFFFF>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

template<> void x::proxy::rng_delegate<x::max::rng_delegate_uint64, uint64_t, 1, 0xFFFFFFFFFFFFFFFF>::callback(x::max::rng_delegate_uint64 *o, size_t n)
{
	rng_delegate_callback(o->context(), n);
}

#endif
