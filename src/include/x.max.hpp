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

#include <random>
#include "pcg_random.hpp"
#include "x.proxy.hpp"
#include "x.dist.hpp"

#ifndef __X_MAX_HPP__
#define __X_MAX_HPP__

typedef struct _maxobj
{
	t_object ob;
	void *myobj;
	t_atom **buf;
	size_t *n;
	int delegation_status;
} t_maxobj;

template <typename arg1_type>
struct _dist_1arg_maxobj
{
	t_maxobj ob;
	arg1_type a1;
};

template <typename arg1_type, typename arg2_type>
struct _dist_2arg_maxobj
{
	t_maxobj ob;
	arg1_type a1;
	arg2_type a2;
};

template <typename vec1_type>
struct _dist_1vec_maxobj
{
	t_maxobj ob;
	std::vector<vec1_type> vec1;
};

template <typename vec1_type, typename vec2_type>
struct _dist_2vec_maxobj
{
	t_maxobj ob;
	std::vector<vec1_type> vec1;
	std::vector<vec2_type> vec2;
};

// Random devices
typedef struct _maxobj t_random_device_maxobj;

// Seed sequences
typedef struct _maxobj t_seed_seq_from_maxobj;

// Generators
typedef struct _maxobj t_generator_pcg32_maxobj;
typedef struct _maxobj t_generator_mt19937_64_maxobj;

// Uniform
typedef struct _dist_2arg_maxobj<int, int> t_dist_uniform_int_maxobj;
typedef struct _dist_2arg_maxobj<double, double> t_dist_uniform_real_maxobj;
// Related to Bernoulli trials
typedef struct _dist_1arg_maxobj<double> t_dist_bernoulli_maxobj;
typedef struct _dist_2arg_maxobj<int, double> t_dist_binomial_maxobj;
typedef struct _dist_1arg_maxobj<double> t_dist_geometric_maxobj;
typedef struct _dist_2arg_maxobj<int, double> t_dist_negative_binomial_maxobj;
// Rate-based distributions
typedef struct _dist_1arg_maxobj<double> t_dist_poisson_maxobj;
typedef struct _dist_1arg_maxobj<double> t_dist_exponential_maxobj;
typedef struct _dist_2arg_maxobj<double, double> t_dist_gamma_maxobj;
typedef struct _dist_2arg_maxobj<double, double> t_dist_weibull_maxobj;
typedef struct _dist_2arg_maxobj<double, double> t_dist_extreme_value_maxobj;
typedef struct _dist_2arg_maxobj<double, double> t_dist_beta_maxobj;
// Related to Normal distribution
typedef struct _dist_2arg_maxobj<double, double> t_dist_normal_maxobj;
typedef struct _dist_2arg_maxobj<double, double> t_dist_lognormal_maxobj;
typedef struct _dist_1arg_maxobj<double> t_dist_chi_squared_maxobj;
typedef struct _dist_2arg_maxobj<double, double> t_dist_cauchy_maxobj;
typedef struct _dist_2arg_maxobj<double, double> t_dist_fisher_f_maxobj;
typedef struct _dist_1arg_maxobj<double> t_dist_student_t_maxobj;
// Piecewise distributions
typedef struct _dist_1vec_maxobj<long> t_dist_discrete_maxobj;
typedef struct _dist_2vec_maxobj<double, double> t_dist_piecewise_constant_maxobj;
typedef struct _dist_2vec_maxobj<double, double> t_dist_piecewise_linear_maxobj;
typedef struct _dist_1vec_maxobj<double> t_dist_dirichlet_maxobj;

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
				return (t_object *)x;
			}

			static void msg_anything(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(!argc){
					object_error((t_object *)_x, "doesn't understand message %s", msg->s_name);
					return;
				}
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

			int main(void)
			{
				class_addmethod(_c, (method)msg_anything, "anything", A_GIMME, 0);
				class_addmethod(_c, (method)msg_list, "list", A_GIMME, 0);
				class_addmethod(_c, (method)msg_int, "int", A_LONG, 0);
				class_addmethod(_c, (method)msg_float, "float", A_FLOAT, 0);
				class_addmethod(_c, (method)msg_bang, "bang", 0);
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
			atom_setlong(a, (long)v);
		}

		template <typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr>
		void atom_set(t_atom *a, T v)
		{
			atom_setfloat(a, (double)v);
		}

		class random_device_obj : public obj
		{
			std::random_device rd;
		public:
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *xx = obj::newobj(msg, argc, argv);
				t_maxobj *x = (t_maxobj *)xx;
				random_device_obj *o = new random_device_obj;
				obj::obj_init_without_delegation(xx, (obj *)o);
				x->myobj = (void *)o;
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
					t_atom out[n];
					for(int i = 0; i < n; i++){
						atom_setlong(out + i, x->rd());
					}
					outlet_list(x->outlet_main(), _sym_list, n, out);
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

			int main(void)
			{
				int ret = obj::main();
				class_addmethod(max_class(), (method)msg_generate, "generate", A_GIMME, 0);
				class_addmethod(max_class(), (method)msg_min, "min", 0);
				class_addmethod(max_class(), (method)msg_max, "max", 0);
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
				seed_source.context(x->outlet_delegation());
				_x->n = seed_source.buffer_len_address();
				_x->buf = seed_source.buffer_address();
				typename seed_seq_from_obj_base::result_type buf[n];
				seed_source.generate(buf, buf + n);
				t_atom out[n];
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
			}

			static void freeobj(t_maxobj *x)
			{
				if(x->buf){
					sysmem_freeptr(x->buf);
				}
				
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
				x::proxy::seed_seq_from_delegate<seed_seq_from_delegate_base, std::random_device> seed_source;
				seed_source.context(outlet_delegation());
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
				return _rng_is_valid;
			}
			
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *x = obj::newobj(msg, argc, argv);
				if(x){
					t_maxobj *xx = (t_maxobj *)x;
					rng_obj<rng_type> *o = new rng_obj<rng_type>;
					obj::obj_init(x, (obj *)o);
					xx->myobj = (void *)o;
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
				t_atom out[n];
				for(int i = 0; i < n; i++){
					atom_set(out + i, x->_rng());
				}
				outlet_list(x->outlet_main(), _sym_list, n, out);
			}

			static void msg_min(t_maxobj *_x)
			{
				rng_obj<rng_type> *x = (rng_obj<rng_type> *)(_x->myobj);
				t_atom a;
				atom_set(&a, x->min());
				outlet_anything(x->outlet_main(), _sym_min, 1, &a);
				//outlet_int(x->outlet_main(), x->min());
			}

			static void msg_max(t_maxobj *_x)
			{
				rng_obj<rng_type> *x = (rng_obj<rng_type> *)(_x->myobj);
				t_atom a;
				atom_set(&a, x->max());
				outlet_anything(x->outlet_main(), _sym_max, 1, &a);
				//outlet_int(x->outlet_main(), x->max());
			}

			static void freeobj(t_maxobj *x)
			{
				if(x->buf){
					sysmem_freeptr(x->buf);
				}
				
				if(x->myobj){
					delete ((rng_obj<rng_type> *)(x->myobj));
				}
			}

			int main(void)
			{
				rng_obj_base::main();
				class_addmethod(max_class(), (method)msg_generate, "generate", A_GIMME, 0);
				class_addmethod(max_class(), (method)msg_min, "min", 0);
				class_addmethod(max_class(), (method)msg_max, "max", 0);
				return 0;
			}
		};
		using generator_pcg32_obj = rng_obj<pcg32>;
		generator_pcg32_obj _generator_pcg32_obj;
		t_object *generator_pcg32_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_pcg32_obj.newobj(msg, argc, argv);
		}

		using generator_mt19937_64_obj = rng_obj<std::mt19937_64>;
		generator_mt19937_64_obj _generator_mt19937_64_obj;
		t_object *generator_mt19937_64_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _generator_mt19937_64_obj.newobj(msg, argc, argv);
		}
		
		using rng_delegate_uint32 = x::proxy::delegate<uint32_t, t_atom, x::max::atom_get<uint32_t>, x::max::atom_set/*<uint32_t>*/>;
		using rng_delegate_uint64 = x::proxy::delegate<uint64_t, t_atom, x::max::atom_get<uint64_t>, x::max::atom_set/*<uint64_t>*/>;
		template <typename dist_type, typename result_type, bool multivariate=false>
		class dist_obj : public obj, public dist_type
		{
		public:
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *x = obj::newobj(msg, argc, argv);
				if(x){
					t_maxobj *xx = (t_maxobj *)x;
					dist_obj<dist_type, result_type> *o = new dist_obj<dist_type, result_type>;
					obj::obj_init(x, (obj *)o);
					xx->myobj = (void *)o;
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
				((dist_obj<dist_type, result_type> *)(_x->myobj))->init_delegate(_x, (rng_delegate_uint64 *)rng);
				t_atom a;
				atom_set(&a, d(*rng));
				outlet_atoms(((dist_obj<dist_type, result_type> *)(_x->myobj))->outlet_main(), 1, &a);
				((dist_obj<dist_type, result_type> *)(_x->myobj))->finalize_delegate(_x, (rng_delegate_uint64 *)rng);
			}

			template <typename rng_type, bool U=multivariate>
			static typename std::enable_if<U>::type _generate(t_maxobj *_x, dist_type d, rng_type *rng)
			{
				((dist_obj<dist_type, result_type> *)(_x->myobj))->init_delegate(_x, (rng_delegate_uint64 *)rng);
				std::vector<result_type> vec = d(*rng);
				size_t n = vec.size();
				t_atom a[n];
				for(int i = 0; i < n; i++){
					atom_set(a + i, vec[i]);
				}
				outlet_atoms(((dist_obj<dist_type, result_type> *)(_x->myobj))->outlet_main(), n, a);
				((dist_obj<dist_type, result_type> *)(_x->myobj))->finalize_delegate(_x, (rng_delegate_uint64 *)rng);
			}
			
			template <typename ...params>
			static void generate(t_maxobj *_x, params... ps)
			{
				dist_obj<dist_type, result_type> *x = (dist_obj<dist_type, result_type> *)(_x->myobj);
				dist_type d = dist_type(ps...);
				size_t n = 1;
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
			}

			template <typename ...params>
			static void min(t_maxobj *_x, params... ps)
			{
				dist_type d(ps...);
				t_atom a;
				atom_set(&a, d.min());
				outlet_anything(((dist_obj<dist_type, result_type> *)(_x->myobj))->outlet_main(), _sym_min, 1, &a);
			}

			template <typename ...params>
			static void max(t_maxobj *_x, params... ps)
			{
				dist_type d(ps...);
				t_atom a;
				atom_set(&a, d.max());
				outlet_anything(((dist_obj<dist_type, result_type> *)(_x->myobj))->outlet_main(), _sym_max, 1, &a);
			}

			static void freeobj(t_maxobj *x)
			{
				if(x->buf){
					sysmem_freeptr(x->buf);
				}
				
				if(x->myobj){
					delete ((dist_obj<dist_type, result_type> *)(x->myobj));
				}
			}

			int main(void)
			{
				obj::main();
				return 0;
			}
		};

		template <typename dist_obj_type, typename struct_type, typename arg1_type, const char *arg1_type_str, const char *arg1_name, const char *arg1_default>
		class dist_1arg_obj : public dist_obj_type
		{
		public:
			void process_attr(t_object *x, t_dictionary *d, const char *attr_name)
			{
				t_symbol *attr_name_sym = gensym(attr_name);
				t_symbol *attr_name_sym_space = gensym((std::string(attr_name) + std::string(" ")).c_str());
				if(dictionary_hasentry(d, attr_name_sym)){
					// user instantiated the obj with the attribute name specified as an argument
				}else if(dictionary_hasentry(d, attr_name_sym_space)){
					// default value of the attr
					attr_name_sym = attr_name_sym_space;
				}
				long argc = 0;
				t_atom *argv = NULL;
				dictionary_getatoms(d, attr_name_sym, &argc, &argv);
				if(argc && argv){
					object_attr_setvalueof(x, attr_name_sym_space, argc, argv);
				}
			}
			
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *_x = dist_obj_type::newobj(msg, argc, argv);
				if(_x){
					if(t_dictionary *d = object_dictionaryarg(argc, argv)){
						process_attr(_x, d, arg1_name);
					}
				}
				return _x;
			}
			
			static void msg_generate(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				dist_obj_type::generate(_x, ((struct_type *)_x)->a1);
			}

			static void msg_min(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					dist_obj_type::msg_anything(_x, msg, argc, argv);
				}else{
					dist_obj_type::min(_x, ((struct_type *)_x)->a1);
				}
			}

			static void msg_max(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					dist_obj_type::msg_anything(_x, msg, argc, argv);
				}else{
					dist_obj_type::max(_x, ((struct_type *)_x)->a1);
				}
			}

			static void msg_arg1(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					object_attr_setvalueof(_x, gensym((std::string(arg1_name) + std::string(" ")).c_str()), argc, argv);
				}else{
					dist_obj_type *x = ((dist_obj_type *)(_x->myobj)) ;
					t_atom a;
					atom_set(&a, ((struct_type *)_x)->a1);
					outlet_anything(x->outlet_main(), gensym(arg1_name), 1, &a);
				}
			}

			static void msg_paramnames(t_maxobj *_x)
			{
				dist_obj_type *x = ((dist_obj_type *)(_x->myobj));
				t_atom a;
				atom_setsym(&a, gensym(arg1_name));
				outlet_anything(x->outlet_main(), ps_paramnames, 1, &a);
			}

			int main(void)
			{
				dist_obj_type::main();
				t_class *c = dist_obj_type::max_class();
				class_addmethod(c, (method)msg_generate, "generate", A_GIMME, 0);
				class_addmethod(c, (method)msg_min, "min", A_GIMME, 0);
				class_addmethod(c, (method)msg_max, "max", A_GIMME, 0);
				class_addmethod(c, (method)msg_paramnames, "paramnames", 0);
				class_addmethod(c, (method)msg_arg1, arg1_name, A_GIMME, 0);
				class_addattr(c, attr_offset_new((std::string(arg1_name) + std::string(" ")).c_str(), gensym(arg1_type_str), 0, (method)0L,(method)0L,calcoffset(struct_type, a1)));		
				CLASS_ATTR_DEFAULT(c, (std::string(arg1_name) + std::string(" ")).c_str(), 0, arg1_default);
				return 0;
			}
		};

		template <typename dist_obj_type, typename struct_type, typename arg1_type, typename arg2_type, const char *arg1_type_str, const char *arg2_type_str, const char *arg1_name, const char *arg2_name, const char *arg1_default, const char *arg2_default>
		class dist_2arg_obj : public dist_1arg_obj<dist_obj_type, struct_type, arg1_type, arg1_type_str, arg1_name, arg1_default>
		{
		public:
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *_x = dist_1arg_obj<dist_obj_type, struct_type, arg1_type, arg1_type_str, arg1_name, arg1_default>::newobj(msg, argc, argv);
				if(_x){
					if(t_dictionary *d = object_dictionaryarg(argc, argv)){
						dist_1arg_obj<dist_obj_type, struct_type, arg1_type, arg1_type_str, arg1_name, arg1_default>::process_attr(_x, d, arg2_name);
					}
				}
				return _x;
			}

			static void msg_generate(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				dist_obj_type::generate(_x, ((struct_type *)_x)->a1, ((struct_type *)_x)->a2);
			}

			static void msg_min(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					dist_obj_type::msg_anything(_x, msg, argc, argv);
				}else{
					dist_obj_type::min(_x, ((struct_type *)_x)->a1, ((struct_type *)_x)->a2);
				}
			}

			static void msg_max(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					dist_obj_type::msg_anything(_x, msg, argc, argv);
				}else{
					dist_obj_type::max(_x, ((struct_type *)_x)->a1, ((struct_type *)_x)->a2);
				}
			}
			
			static void msg_arg2(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					object_attr_setvalueof(_x, gensym((std::string(arg2_name) + std::string(" ")).c_str()), argc, argv);
				}else{
					dist_obj_type *x = ((dist_obj_type *)(_x->myobj)) ;
					t_atom a;
					atom_set(&a, ((struct_type *)_x)->a2);
					outlet_anything(x->outlet_main(), gensym(arg2_name), 1, &a);
				}
			}

			static void msg_paramnames(t_maxobj *_x)
			{
				dist_obj_type *x = ((dist_obj_type *)(_x->myobj));
				t_atom a[2];
				atom_setsym(a, gensym(arg1_name));
				atom_setsym(a + 1, gensym(arg2_name));
				outlet_anything(x->outlet_main(), ps_paramnames, 2, a);
			}

			int main(void)
			{
				dist_obj_type::main();
				t_class *c = dist_obj_type::max_class();
				class_addmethod(c, (method)msg_generate, "generate", A_GIMME, 0);
				class_addmethod(c, (method)msg_min, "min", A_GIMME, 0);
				class_addmethod(c, (method)msg_max, "max", A_GIMME, 0);
				class_addmethod(c, (method)msg_paramnames, "paramnames", 0);
				class_addmethod(c, (method)dist_1arg_obj<dist_obj_type, struct_type, arg1_type, arg1_type_str, arg1_name, arg1_default>::msg_arg1, arg1_name, A_GIMME, 0);
				class_addattr(c, attr_offset_new((std::string(arg1_name) + std::string(" ")).c_str(), gensym(arg1_type_str), 0, (method)0L,(method)0L,calcoffset(struct_type, a1)));		
				CLASS_ATTR_DEFAULT(c, (std::string(arg1_name) + std::string(" ")).c_str(), 0, arg1_default);
				
				class_addmethod(c, (method)msg_arg2, arg2_name, A_GIMME, 0);
				class_addattr(c, attr_offset_new((std::string(arg2_name) + std::string(" ")).c_str(), gensym(arg2_type_str), 0, (method)0L,(method)0L,calcoffset(struct_type, a2)));		
				CLASS_ATTR_DEFAULT(c, (std::string(arg2_name) + std::string(" ")).c_str(), 0, arg2_default);
				return 0;
			}
		};

		template <typename dist_obj_type, typename struct_type, typename vec1_type, const char *vec1_type_str, const char *vec1_name, long vec1_argc, vec1_type vec1_default[]>
		class dist_1vec_obj : public dist_obj_type
		{
		public:
			template <typename T>
			std::vector<T> init_vec(long argc, T *argv)
			{
				std::vector<T> vec(argc);
				for(int i = 0; i < argc; i++){
					vec[i] = argv[i];
				}
				return vec;
			}

			template <typename T>
			std::vector<T> process_attr(t_object *x, t_dictionary *d, const char *attr_name, long def_argc, T *def_argv)
			{
				t_symbol *attr_name_sym = gensym(attr_name);
				t_symbol *attr_name_sym_space = gensym((std::string(attr_name) + std::string(" ")).c_str());
				if(dictionary_hasentry(d, attr_name_sym)){
					// user instantiated the obj with the attribute name specified as an argument
				}else if(dictionary_hasentry(d, attr_name_sym_space)){
					// default value of the attr
					attr_name_sym = attr_name_sym_space;
				}else{
					return init_vec(def_argc, def_argv);
				}
				long argc = 0;
				t_atom *argv = NULL;
				dictionary_getatoms(d, attr_name_sym, &argc, &argv);
				if(argc && argv){
					T buf[argc];
					for(int i = 0; i < argc; i++){
						buf[i] = atom_get<T>(argv + i);
					}
					return init_vec(argc, buf);
				}
				return std::vector<T>(0);
			}
			
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *_x = dist_obj_type::newobj(msg, argc, argv);
				if(_x){
					if(t_dictionary *d = object_dictionaryarg(argc, argv)){
						((struct_type *)_x)->vec1 = process_attr(_x, d, vec1_name, vec1_argc, vec1_default);
					}
				}
				return _x;
			}
			
			static void msg_generate(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				std::vector<vec1_type> vec1 = ((struct_type *)_x)->vec1;
				dist_obj_type::generate(_x, vec1.begin(), vec1.end());
			}

			static void msg_min(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					dist_obj_type::msg_anything(_x, msg, argc, argv);
				}else{
					std::vector<vec1_type> vec1 = ((struct_type *)_x)->vec1;
					dist_obj_type::min(_x, vec1.begin(), vec1.end());
				}
			}

			static void msg_max(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					dist_obj_type::msg_anything(_x, msg, argc, argv);
				}else{
					std::vector<vec1_type> vec1 = ((struct_type *)_x)->vec1;
					dist_obj_type::max(_x, vec1.begin(), vec1.end());
				}
			}

			static t_max_err vec1_get(t_maxobj *x, t_object *attr, long *argc, t_atom **argv)
			{
				std::vector<vec1_type> vec1 = (((struct_type *)x)->vec1);
				long args = vec1.size();
				*argv = (t_atom *)sysmem_resizeptr(*argv, args * sizeof(t_atom));
				*argc = args;
				for(int i = 0; i < args; i++){
					atom_set((*argv) + i, vec1[i]);
				}
				return 0;
			}
			
			static t_max_err vec1_set(t_maxobj *x, t_object *attr, long argc, t_atom *argv)
			{
				std::vector<vec1_type> vec1(argc);
				for(int i = 0; i < argc; i++){
					vec1[i] = atom_get<typename std::vector<vec1_type>::value_type>(argv + i);
				}
				(((struct_type *)x)->vec1) = vec1;
				return 0;
			}

			static void msg_vec1(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					object_attr_setvalueof(_x, gensym((std::string(vec1_name) + std::string(" ")).c_str()), argc, argv);
				}else{
					dist_obj_type *x = ((dist_obj_type *)(_x->myobj)) ;
					std::vector<vec1_type> vec1 = (((struct_type *)x)->vec1);
					long args = vec1.size();
					t_atom a[args];
					for(int i = 0; i < args; i++){
						atom_set(a + i, vec1[i]);
					}
					outlet_anything(x->outlet_main(), gensym(vec1_name), args, a);
				}
			}

			static void msg_paramnames(t_maxobj *_x)
			{
				dist_obj_type *x = ((dist_obj_type *)(_x->myobj));
				t_atom a;
				atom_setsym(&a, gensym(vec1_name));
				outlet_anything(x->outlet_main(), ps_paramnames, 1, &a);
			}

			int main(void)
			{
				dist_obj_type::main();
				t_class *c = dist_obj_type::max_class();
				class_addmethod(c, (method)msg_generate, "generate", A_GIMME, 0);
				class_addmethod(c, (method)msg_min, "min", A_GIMME, 0);
				class_addmethod(c, (method)msg_max, "max", A_GIMME, 0);
				class_addmethod(c, (method)msg_paramnames, "paramnames", 0);
				class_addmethod(c, (method)msg_vec1, vec1_name, A_GIMME, 0);
				class_addattr((c), attr_offset_array_new((std::string(vec1_name) + std::string(" ")).c_str(), gensym(vec1_type_str), 4096, 0, (method)0L, (method)0L, calcoffset(struct_type, vec1), calcoffset(struct_type, vec1)));
				CLASS_ATTR_ACCESSORS(c, (std::string(vec1_name) + std::string(" ")).c_str(), vec1_get, vec1_set);
				return 0;
			}
		};

		template <typename dist_obj_type, typename struct_type, typename vec1_type, typename vec2_type, const char *vec1_type_str, const char *vec2_type_str, const char *vec1_name, const char *vec2_name, long vec1_argc, vec1_type vec1_default[], long vec2_argc, vec2_type vec2_default[]>
		class dist_2vec_obj : public dist_1vec_obj<dist_obj_type, struct_type, vec1_type, vec1_type_str, vec1_name, vec1_argc, vec1_default>
		{
		public:
			t_object *newobj(t_symbol *msg, short argc, t_atom *argv)
			{
				t_object *_x = dist_1vec_obj<dist_obj_type, struct_type, vec1_type, vec1_type_str, vec1_name, vec1_argc, vec1_default>::newobj(msg, argc, argv);
				if(_x){
					if(t_dictionary *d = object_dictionaryarg(argc, argv)){
						((struct_type *)_x)->vec2 = dist_1vec_obj<dist_obj_type, struct_type, vec1_type, vec1_type_str, vec1_name, vec1_argc, vec1_default>::process_attr(_x, d, vec2_name, vec2_argc, vec2_default);
					}
				}
				return _x;
			}
			
			static void msg_generate(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				std::vector<vec1_type> vec1 = ((struct_type *)_x)->vec1;
				std::vector<vec2_type> vec2 = ((struct_type *)_x)->vec2;
				dist_obj_type::generate(_x, vec1.begin(), vec1.end(), vec2.begin());
			}

			static void msg_min(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					dist_obj_type::msg_anything(_x, msg, argc, argv);
				}else{
					std::vector<vec1_type> vec1 = ((struct_type *)_x)->vec1;
					std::vector<vec2_type> vec2 = ((struct_type *)_x)->vec2;
					dist_obj_type::min(_x, vec1.begin(), vec1.end(), vec2.begin());
				}
			}

			static void msg_max(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					dist_obj_type::msg_anything(_x, msg, argc, argv);
				}else{
					std::vector<vec1_type> vec1 = ((struct_type *)_x)->vec1;
					std::vector<vec2_type> vec2 = ((struct_type *)_x)->vec2;
					dist_obj_type::max(_x, vec1.begin(), vec1.end(), vec2.begin());
				}
			}

			static t_max_err vec2_get(t_maxobj *x, t_object *attr, long *argc, t_atom **argv)
			{
				std::vector<vec2_type> vec2 = (((struct_type *)x)->vec2);
				long args = vec2.size();
				*argv = (t_atom *)sysmem_resizeptr(*argv, args * sizeof(t_atom));
				*argc = args;
				for(int i = 0; i < args; i++){
					atom_set((*argv) + i, vec2[i]);
				}
				return 0;
			}
			
			static t_max_err vec2_set(t_maxobj *x, t_object *attr, long argc, t_atom *argv)
			{
				std::vector<vec2_type> vec2(argc);
				for(int i = 0; i < argc; i++){
					vec2[i] = atom_get<typename std::vector<vec2_type>::value_type>(argv + i);
				}
				(((struct_type *)x)->vec2) = vec2;
				return 0;
			}

			static void msg_vec2(t_maxobj *_x, t_symbol *msg, short argc, t_atom *argv)
			{
				if(argc){
					object_attr_setvalueof(_x, gensym((std::string(vec2_name) + std::string(" ")).c_str()), argc, argv);
				}else{
					dist_obj_type *x = ((dist_obj_type *)(_x->myobj)) ;
					std::vector<vec2_type> vec2 = (((struct_type *)x)->vec2);
					long args = vec2.size();
					t_atom a[args];
					for(int i = 0; i < args; i++){
						atom_set(a + i, vec2[i]);
					}
					outlet_anything(x->outlet_main(), gensym(vec2_name), args, a);
				}
			}

			static void msg_paramnames(t_maxobj *_x)
			{
				dist_obj_type *x = ((dist_obj_type *)(_x->myobj));
				t_atom a[2];
				atom_setsym(a, gensym(vec1_name));
				atom_setsym(a + 1, gensym(vec2_name));
				outlet_anything(x->outlet_main(), ps_paramnames, 2, a);
			}

			int main(void)
			{
				dist_obj_type::main();
				t_class *c = dist_obj_type::max_class();
				class_addmethod(c, (method)msg_generate, "generate", A_GIMME, 0);
				class_addmethod(c, (method)msg_min, "min", A_GIMME, 0);
				class_addmethod(c, (method)msg_max, "max", A_GIMME, 0);
				class_addmethod(c, (method)msg_paramnames, "paramnames", 0);
				class_addmethod(c, (method)dist_1vec_obj<dist_obj_type, struct_type, vec1_type, vec1_type_str, vec1_name, vec1_argc, vec1_default>::msg_vec1, vec1_name, A_GIMME, 0);
				class_addattr((c), attr_offset_array_new((std::string(vec1_name) + std::string(" ")).c_str(), gensym(vec1_type_str), 4096, 0, (method)0L, (method)0L, calcoffset(struct_type, vec1), calcoffset(struct_type, vec1)));

				t_object *theattr = (t_object *)class_attr_get(c, gensym((std::string(vec1_name) + std::string(" ")).c_str()));
				t_max_err (*getter)(t_maxobj *x, t_object *attr, long *argc, t_atom **argv) = dist_1vec_obj<dist_obj_type, struct_type, vec1_type, vec1_type_str, vec1_name, vec1_argc, vec1_default>::vec1_get;
				object_method(theattr, gensym("setmethod"), USESYM(get), getter);
				t_max_err (*setter)(t_maxobj *x, t_object *attr, long argc, t_atom *argv) = dist_1vec_obj<dist_obj_type, struct_type, vec1_type, vec1_type_str, vec1_name, vec1_argc, vec1_default>::vec1_set;
				object_method(theattr, gensym("setmethod"), USESYM(set), setter);

				class_addmethod(c, (method)msg_vec2, vec2_name, A_GIMME, 0);
				class_addattr((c), attr_offset_array_new((std::string(vec2_name) + std::string(" ")).c_str(), gensym(vec2_type_str), 4096, 0, (method)0L, (method)0L, calcoffset(struct_type, vec2), calcoffset(struct_type, vec2)));
				CLASS_ATTR_ACCESSORS(c, (std::string(vec2_name) + std::string(" ")).c_str(), vec2_get, vec2_set);
				return 0;
			}
		};

		const char double_str[] = "float64";
		const char int_str[] = "int32";
		const char alpha_str[] = "alpha";
		const char beta_str[] = "beta";
		const char mean_str[] = "mean";
		const char lambda_str[] = "lambda";
		const char stddev_str[] = "stddev";
		const char probabilities_str[] = "probabilities";
		const char intervals_str[] = "intervals";
		const char densities_str[] = "densities";
		const char a_str[] = "a";
		const char b_str[] = "b";
		const char k_str[] = "k";
		const char m_str[] = "m";
		const char n_str[] = "n";
		const char p_str[] = "p";
		const char s_str[] = "s";
		const char t_str[] = "t";
		const char one_str[] = "1";
		const char onef_str[] = "1.0";
		const char half_str[] = "0.5";
		const char zero_str[] = "0";
		const char zerof_str[] = "0.";

		// Uniform
		using dist_uniform_int_obj = dist_2arg_obj<dist_obj<std::uniform_int_distribution<long>, long>, t_dist_uniform_int_maxobj, long, long, int_str, int_str, a_str, b_str, zero_str, one_str>;
		dist_uniform_int_obj _dist_uniform_int_obj;
		t_object *dist_uniform_int_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_uniform_int_obj.newobj(msg, argc, argv);
		}

		using dist_uniform_real_obj = dist_2arg_obj<dist_obj<std::uniform_real_distribution<double>, double>, t_dist_uniform_real_maxobj, double, double, double_str, double_str, a_str, b_str, zerof_str, onef_str>;
		dist_uniform_real_obj _dist_uniform_real_obj;
		t_object *dist_uniform_real_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_uniform_real_obj.newobj(msg, argc, argv);
		}

		// Related to Bernoulli trials
		using dist_bernoulli_obj = dist_1arg_obj<dist_obj<std::bernoulli_distribution, long>, t_dist_bernoulli_maxobj, double, double_str, p_str, half_str>;
		dist_bernoulli_obj _dist_bernoulli_obj;
		t_object *dist_bernoulli_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_bernoulli_obj.newobj(msg, argc, argv);
		}

		using dist_binomial_obj = dist_2arg_obj<dist_obj<std::binomial_distribution<long>, long>, t_dist_binomial_maxobj, long, double, int_str, double_str, t_str, p_str, one_str, half_str>;
		dist_binomial_obj _dist_binomial_obj;
		t_object *dist_binomial_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_binomial_obj.newobj(msg, argc, argv);
		}

		using dist_geometric_obj = dist_1arg_obj<dist_obj<std::geometric_distribution<long>, long>, t_dist_geometric_maxobj, double, double_str, p_str, half_str>;
		dist_geometric_obj _dist_geometric_obj;
		t_object *dist_geometric_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_geometric_obj.newobj(msg, argc, argv);
		}

		using dist_negative_binomial_obj = dist_2arg_obj<dist_obj<std::negative_binomial_distribution<long>, long>, t_dist_negative_binomial_maxobj, long, double, int_str, double_str, k_str, p_str, one_str, half_str>;
		dist_negative_binomial_obj _dist_negative_binomial_obj;
		t_object *dist_negative_binomial_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_negative_binomial_obj.newobj(msg, argc, argv);
		}
		
		// Rate-based distributions
		using dist_poisson_obj = dist_1arg_obj<dist_obj<std::poisson_distribution<long>, long>, t_dist_poisson_maxobj, double, double_str, mean_str, onef_str>;
		dist_poisson_obj _dist_poisson_obj;
		t_object *dist_poisson_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_poisson_obj.newobj(msg, argc, argv);
		}

		using dist_exponential_obj = dist_1arg_obj<dist_obj<std::exponential_distribution<double>, double>, t_dist_exponential_maxobj, double, double_str, lambda_str, onef_str>;
		dist_exponential_obj _dist_exponential_obj;
		t_object *dist_exponential_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_exponential_obj.newobj(msg, argc, argv);
		}
		
		using dist_gamma_obj = dist_2arg_obj<dist_obj<std::gamma_distribution<double>, double>, t_dist_gamma_maxobj, double, double, double_str, double_str, alpha_str, beta_str, onef_str, onef_str>;
		dist_gamma_obj _dist_gamma_obj;
		t_object *dist_gamma_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_gamma_obj.newobj(msg, argc, argv);
		}
		
		using dist_weibull_obj = dist_2arg_obj<dist_obj<std::weibull_distribution<double>, double>, t_dist_weibull_maxobj, double, double, double_str, double_str, a_str, b_str, onef_str, onef_str>;
		dist_weibull_obj _dist_weibull_obj;
		t_object *dist_weibull_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_weibull_obj.newobj(msg, argc, argv);
		}

		using dist_extreme_value_obj = dist_2arg_obj<dist_obj<std::extreme_value_distribution<double>, double>, t_dist_extreme_value_maxobj, double, double, double_str, double_str, a_str, b_str, onef_str, onef_str>;
		dist_extreme_value_obj _dist_extreme_value_obj;
		t_object *dist_extreme_value_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_extreme_value_obj.newobj(msg, argc, argv);
		}

		using dist_beta_obj = dist_2arg_obj<dist_obj<x::dist::beta_distribution<double>, double>, t_dist_beta_maxobj, double, double, double_str, double_str, alpha_str, beta_str, onef_str, onef_str>;
		dist_beta_obj _dist_beta_obj;
		t_object *dist_beta_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_beta_obj.newobj(msg, argc, argv);
		}

		// Related to Normal distribution
		using dist_normal_obj = dist_2arg_obj<dist_obj<std::normal_distribution<double>, double>, t_dist_normal_maxobj, double, double, double_str, double_str, mean_str, stddev_str, onef_str, onef_str>;
		dist_normal_obj _dist_normal_obj;
		t_object *dist_normal_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_normal_obj.newobj(msg, argc, argv);
		}

		using dist_lognormal_obj = dist_2arg_obj<dist_obj<std::lognormal_distribution<double>, double>, t_dist_lognormal_maxobj, double, double, double_str, double_str, m_str, s_str, onef_str, onef_str>;
		dist_lognormal_obj _dist_lognormal_obj;
		t_object *dist_lognormal_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_lognormal_obj.newobj(msg, argc, argv);
		}

		using dist_chi_squared_obj = dist_1arg_obj<dist_obj<std::chi_squared_distribution<double>, double>, t_dist_chi_squared_maxobj, double, double_str, n_str, onef_str>;
		dist_chi_squared_obj _dist_chi_squared_obj;
		t_object *dist_chi_squared_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_chi_squared_obj.newobj(msg, argc, argv);
		}

		using dist_cauchy_obj = dist_2arg_obj<dist_obj<std::cauchy_distribution<double>, double>, t_dist_cauchy_maxobj, double, double, double_str, double_str, a_str, b_str, onef_str, onef_str>;
		dist_cauchy_obj _dist_cauchy_obj;
		t_object *dist_cauchy_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_cauchy_obj.newobj(msg, argc, argv);
		}

		using dist_fisher_f_obj = dist_2arg_obj<dist_obj<std::fisher_f_distribution<double>, double>, t_dist_fisher_f_maxobj, double, double, double_str, double_str, m_str, n_str, onef_str, onef_str>;
		dist_fisher_f_obj _dist_fisher_f_obj;
		t_object *dist_fisher_f_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_fisher_f_obj.newobj(msg, argc, argv);
		}

		using dist_student_t_obj = dist_1arg_obj<dist_obj<std::student_t_distribution<double>, double>, t_dist_student_t_maxobj, double, double_str, n_str, onef_str>;
		dist_student_t_obj _dist_student_t_obj;
		t_object *dist_student_t_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_student_t_obj.newobj(msg, argc, argv);
		}

		// Piecewise distributions
		long dist_discrete_arg_default[] = {1};
		using dist_discrete_obj = dist_1vec_obj<dist_obj<std::discrete_distribution<long>, long>, t_dist_discrete_maxobj, long, int_str, probabilities_str, 1, dist_discrete_arg_default>;
		dist_discrete_obj _dist_discrete_obj;
		t_object *dist_discrete_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_discrete_obj.newobj(msg, argc, argv);
		}

		double dist_piecewise_constant_vec1_default[] = {0., 1.};
		double dist_piecewise_constant_vec2_default[] = {1.};
		using dist_piecewise_constant_obj = dist_2vec_obj<dist_obj<std::piecewise_constant_distribution<double>, double>, t_dist_piecewise_constant_maxobj, double, double, double_str, double_str, intervals_str, densities_str, 2, dist_piecewise_constant_vec1_default, 1, dist_piecewise_constant_vec2_default>;
		dist_piecewise_constant_obj _dist_piecewise_constant_obj;
		t_object *dist_piecewise_constant_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_piecewise_constant_obj.newobj(msg, argc, argv);
		}

		double dist_piecewise_linear_vec1_default[] = {0., 1.};
		double dist_piecewise_linear_vec2_default[] = {1.};
		using dist_piecewise_linear_obj = dist_2vec_obj<dist_obj<std::piecewise_linear_distribution<double>, double>, t_dist_piecewise_linear_maxobj, double, double, double_str, double_str, intervals_str, densities_str, 2, dist_piecewise_linear_vec1_default, 1, dist_piecewise_linear_vec2_default>;
		dist_piecewise_linear_obj _dist_piecewise_linear_obj;
		t_object *dist_piecewise_linear_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_piecewise_linear_obj.newobj(msg, argc, argv);
		}

		double dist_dirichlet_arg_default[] = {0.5, 0.5};
		using dist_dirichlet_obj = dist_1vec_obj<dist_obj<x::dist::dirichlet_distribution<double>, double, true>, t_dist_dirichlet_maxobj, double, double_str, alpha_str, 2, dist_dirichlet_arg_default>;
		dist_dirichlet_obj _dist_dirichlet_obj;
		t_object *dist_dirichlet_newobj(t_symbol *msg, short argc, t_atom *argv)
		{
			return _dist_dirichlet_obj.newobj(msg, argc, argv);
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

template<> void x::proxy::seed_seq_from_delegate<x::max::random_device_delegate_base, std::random_device>::callback(x::max::random_device_delegate_base *o, size_t n)
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
