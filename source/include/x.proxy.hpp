/*
Copyright (c) 2018-2021 John MacCallum

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

#include "pcg_extras.hpp"
#ifndef __X_PROXY_HPP__
#define __X_PROXY_HPP__

namespace x
{
	namespace proxy
	{
		template <typename _unboxed_type, typename _boxed_type, _unboxed_type (*unbox_fn)(_boxed_type*), void (*box_fn)(_boxed_type*, _unboxed_type)>
		class delegate
		{
		public:
			typedef _unboxed_type unboxed_type;
			typedef _boxed_type boxed_type;
		private:
			void *_context = NULL;
			size_t _buffer_len = 0;
			boxed_type *_buffer = NULL;
			bool _busy = false;
		public:
			delegate(void) {}
			delegate(void *context) :
				_context(context)
			{}

			void context(void *context)
			{
				_context = context;
			}

			void *context(void)
			{
				return _context;
			}

			void buffer(boxed_type *b)
			{
				_buffer = b;
			}

			boxed_type *buffer(void)
			{
				return _buffer;
			}

			boxed_type **buffer_address(void)
			{
				return &_buffer;
			}

			void buffer_len(size_t n)
			{
				_buffer_len = n;
			}

			size_t buffer_len(void)
			{
				return _buffer_len;
			}

			size_t *buffer_len_address(void)
			{
				return &_buffer_len;
			}

			boxed_type *clear(void)
			{
				boxed_type *b = _buffer;
				_buffer = NULL;
				_buffer_len = 0;
				return b;
			}

			void operator()(void (*callback)(delegate*, size_t), size_t n)
			{
				callback(this, n);
			}

			unboxed_type unbox(boxed_type *v)
			{
				return unbox_fn(v);
			}
		};

		template <typename delegate_type>
		class random_device_delegate : public delegate_type, public std::random_device
		{
		public:
			typedef std::random_device::result_type result_type;
			random_device_delegate(void) {}
			
			static void callback(delegate_type*, size_t);
			
			result_type operator()()
			{
				delegate_type::operator()(callback, 1);
				if(delegate_type::buffer()){
					return delegate_type::unbox(delegate_type::buffer());
				}else{
					return 0;
				}
			}
		};

		template <typename random_device_type>
		class seed_seq_from : public random_device_type
		{
		private:
			random_device_type _rng;
		public:
			typedef uint_least32_t result_type;
			
			template<typename... Args>
			seed_seq_from(Args&&... args) :
				_rng(std::forward<Args>(args)...)
			{}

			template<typename Iter>
			void generate(Iter start, Iter finish)
			{
				for (auto i = start; i != finish; ++i){
					*i = result_type(random_device_type::operator()());
				}
			}

			constexpr size_t size() const
			{
				return (sizeof(typename random_device_type::result_type) > sizeof(result_type)
					&& random_device_type::max() > ~size_t(0UL))
					? ~size_t(0UL)
					: size_t(random_device_type::max());
			}
		};

		template <typename delegate_type, typename random_device_type>
		class seed_seq_from_delegate : public delegate_type, public pcg_extras::seed_seq_from<std::random_device>
		{
		private:
			random_device_type _rng;
		public:
			typedef uint_least32_t result_type;
			
			template<typename... Args>
			seed_seq_from_delegate(Args&&... args) :
				_rng(std::forward<Args>(args)...)
			{}
			
			static void callback(delegate_type*, size_t);
			
			template <typename Iter>
			void generate(Iter start, Iter finish)
			{
				size_t n = finish - start;
				delegate_type::operator()(callback, n);
				if(delegate_type::buffer()){
					typename delegate_type::boxed_type *buffer = delegate_type::buffer();
					for(int i = 0; i < delegate_type::buffer_len(); i++){
						start[i] = delegate_type::unbox(buffer + i);
					}
				}else{
				}
			}

			constexpr size_t size() const
			{
				return (sizeof(typename random_device_type::result_type) > sizeof(result_type)
					&& random_device_type::max() > ~size_t(0UL))
					? ~size_t(0UL)
					: size_t(random_device_type::max());
			}
		};

		template <typename delegate_type, typename _result_type, _result_type MIN, _result_type MAX>
		class rng_delegate : public delegate_type
		{
		public:
			typedef _result_type result_type;

			static void callback(delegate_type*, size_t);
			
			result_type operator()()
			{
				delegate_type::operator()(callback, 1);
				if(delegate_type::buffer() && delegate_type::buffer_len() > 0){
					return delegate_type::unbox(delegate_type::buffer());
				} else {
					return 0;
				}
			}

			static constexpr result_type min()
			{
				return MIN;
			}

			static constexpr result_type max()
			{
				return MAX;
			}
		};
	}
}

#endif
