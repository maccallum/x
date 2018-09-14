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

namespace x
{
	namespace random
	{
		// beta distribution
		template<class _RealType = double>
		class beta_distribution
		{
		public:
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __alpha_;
				result_type __beta_;
			public:
				typedef beta_distribution distribution_type;
				explicit param_type(result_type __alpha = 1, result_type __beta = 1)
					: __alpha_(__alpha), __beta_(__beta) {}
				result_type alpha() const {return __alpha_;}
				result_type beta() const {return __beta_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__alpha_ == __y.__alpha_ && __x.__beta_ == __y.__beta_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit beta_distribution(result_type __alpha = 1, result_type __beta = 1)
				: __p_(param_type(__alpha, __beta)) {}
			explicit beta_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type alpha() const {return __p_.alpha();}
			result_type beta() const {return __p_.beta();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return 0;}
			result_type max() const {return 1.;}

			friend bool operator==(const beta_distribution& __x,
					       const beta_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const beta_distribution& __x,
					       const beta_distribution& __y)
			{return !(__x == __y);}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		beta_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::gamma_distribution<result_type> g1(__p.alpha(), 1.);
			std::gamma_distribution<result_type> g2(__p.beta(), 1.);
			result_type a = g1(__g);
			return a / (a + g2(__g));
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const beta_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.alpha() << __sp << __x.beta();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   beta_distribution<_RT>& __x)
		{
			typedef beta_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __alpha;
			result_type __beta;
			__is >> __alpha >> __beta;
			if (!__is.fail())
				__x.param(param_type(__alpha, __beta));
			return __is;
		}

		// kumaraswamy distribution
		template<class _RealType = double>
		class kumaraswamy_distribution
		{
		public:
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __alpha_;
				result_type __beta_;
			public:
				typedef kumaraswamy_distribution distribution_type;
				explicit param_type(result_type __alpha = 1, result_type __beta = 1)
					: __alpha_(__alpha), __beta_(__beta) {}
				result_type alpha() const {return __alpha_;}
				result_type beta() const {return __beta_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__alpha_ == __y.__alpha_ && __x.__beta_ == __y.__beta_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit kumaraswamy_distribution(result_type __alpha = 1, result_type __beta = 1)
				: __p_(param_type(__alpha, __beta)) {}
			explicit kumaraswamy_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type alpha() const {return __p_.alpha();}
			result_type beta() const {return __p_.beta();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return 0;}
			result_type max() const {return 1.;}

			friend bool operator==(const kumaraswamy_distribution& __x,
					       const kumaraswamy_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const kumaraswamy_distribution& __x,
					       const kumaraswamy_distribution& __y)
			{return !(__x == __y);}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		kumaraswamy_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::gamma_distribution<result_type> g1(1.0, 1.0);
			std::gamma_distribution<result_type> g2(__p.beta(), 1.0);
			result_type a = g1(__g);
			result_type beta = a / (a + g2(__g));
			return powf(beta, 1. / __p.alpha());
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const kumaraswamy_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.alpha() << __sp << __x.beta();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   kumaraswamy_distribution<_RT>& __x)
		{
			typedef kumaraswamy_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __alpha;
			result_type __beta;
			__is >> __alpha >> __beta;
			if (!__is.fail())
				__x.param(param_type(__alpha, __beta));
			return __is;
		}

		// dirichlet distribution
		template<class _RealType = double>
		class dirichlet_distribution
		{
		public:
			// types
			typedef _RealType result_type;

			class param_type
			{
				std::vector<double> __p_;
			public:
				typedef dirichlet_distribution distribution_type;
				param_type() {}
				template<class _InputIterator>
				param_type(_InputIterator __f, _InputIterator __l)
					: __p_(__f, __l) {__init();}
#ifndef _LIBCPP_CXX03_LANG
				param_type(std::initializer_list<double> __wl)
					: __p_(__wl.begin(), __wl.end()) {__init();}
#endif  // _LIBCPP_CXX03_LANG
				template<class _UnaryOperation>
				param_type(size_t __nw, double __xmin, double __xmax,
					   _UnaryOperation __fw);

				std::vector<double> alpha() const;

				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__p_ == __y.__p_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}

			private:
				void __init();

				friend class dirichlet_distribution;

				template <class _CharT, class _Traits, class _IT>
				friend
				std::basic_ostream<_CharT, _Traits>&
				operator<<(std::basic_ostream<_CharT, _Traits>& __os,
					   const dirichlet_distribution<_IT>& __x);

				template <class _CharT, class _Traits, class _IT>
				friend
				std::basic_istream<_CharT, _Traits>&
				operator>>(std::basic_istream<_CharT, _Traits>& __is,
					   dirichlet_distribution<_IT>& __x);
			};

		private:
			param_type __p_;

		public:
			// constructor and reset functions
			dirichlet_distribution() {}
			template<class _InputIterator>
			dirichlet_distribution(_InputIterator __f, _InputIterator __l)
				: __p_(__f, __l) {}
#ifndef _LIBCPP_CXX03_LANG
			dirichlet_distribution(std::initializer_list<double> __wl)
				: __p_(__wl) {}
#endif  // _LIBCPP_CXX03_LANG
			template<class _UnaryOperation>
			dirichlet_distribution(size_t __nw, double __xmin, double __xmax,
					      _UnaryOperation __fw)
				: __p_(__nw, __xmin, __xmax, __fw) {}
			explicit dirichlet_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			std::vector<result_type> operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> std::vector<result_type> operator()(_URNG& __g, const param_type& __p);

			// property functions
			std::vector<double> alpha() const {return __p_.alpha();}

			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}

			result_type min() const {return 0;}
			result_type max() const {return 1.;}

			friend bool operator==(const dirichlet_distribution& __x,
					       const dirichlet_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const dirichlet_distribution& __x,
					       const dirichlet_distribution& __y)
			{return !(__x == __y);}

			template <class _CharT, class _Traits, class _IT>
			friend
			std::basic_ostream<_CharT, _Traits>&
			operator<<(std::basic_ostream<_CharT, _Traits>& __os,
				   const dirichlet_distribution<_IT>& __x);

			template <class _CharT, class _Traits, class _IT>
			friend
			std::basic_istream<_CharT, _Traits>&
			operator>>(std::basic_istream<_CharT, _Traits>& __is,
				   dirichlet_distribution<_IT>& __x);
		};

		template<class _RealType>
		template<class _UnaryOperation>
		dirichlet_distribution<_RealType>::param_type::param_type(size_t __nw,
									double __xmin,
									double __xmax,
									_UnaryOperation __fw)
		{
			// if (__nw > 1)
			// 	{
			// 		__p_.reserve(__nw - 1);
			// 		double __d = (__xmax - __xmin) / __nw;
			// 		double __d2 = __d / 2;
			// 		for (size_t __k = 0; __k < __nw; ++__k)
			// 			__p_.push_back(__fw(__xmin + __k * __d + __d2));
			// 		__init();
			// 	}
		}

		template<class _RealType>
		void
		dirichlet_distribution<_RealType>::param_type::__init()
		{
			// if (!__p_.empty())
			// 	{
			// 		if (__p_.size() > 1)
			// 			{
			// 				double __s = accumulate(__p_.begin(), __p_.end(), 0.0);
			// 				for (std::vector<double>::iterator __i = __p_.begin(), __e = __p_.end();
			// 				     __i < __e; ++__i)
			// 					*__i /= __s;
			// 				std::vector<double> __t(__p_.size() - 1);
			// 				partial_sum(__p_.begin(), __p_.end() - 1, __t.begin());
			// 				swap(__p_, __t);
			// 			}
			// 		else
			// 			{
			// 				__p_.clear();
			// 				__p_.shrink_to_fit();
			// 			}
			// 	}
		}

		template<class _RealType>
		std::vector<double>
		dirichlet_distribution<_RealType>::param_type::alpha() const
		{
			// size_t __n = __p_.size();
			// std::vector<double> __p(__n+1);
			// adjacent_difference(__p_.begin(), __p_.end(), __p.begin());
			// if (__n > 0)
			// 	__p[__n] = 1 - __p_[__n-1];
			// else
			// 	__p[0] = 1;
			// return __p;
			return __p_;
		}

		template<class _RealType>
		template<class _URNG>
		std::vector<_RealType>
		dirichlet_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::vector<_RealType> alpha = __p.alpha();
			size_t n = alpha.size();
			std::vector<_RealType> vec(n);
			_RealType sum = 0;
			for(size_t i = 0; i < n; i++){
				std::gamma_distribution d(alpha[i], 1.0);
				
				vec[i] = d(__g);
				sum += vec[i];
			}
			for(size_t i = 0; i < n; i++){
				vec[i] /= sum;
			}
			return vec;
			//uniform_real_distribution<double> __gen;
			//return static_cast<_RealType>(
			//upper_bound(__p.__p_.begin(), __p.__p_.end(), __gen(__g)) -
			//__p.__p_.begin());
		}

		template <class _CharT, class _Traits, class _IT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const dirichlet_distribution<_IT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			size_t __n = __x.__p_.__p_.size();
			__os << __n;
			for (size_t __i = 0; __i < __n; ++__i)
				__os << __sp << __x.__p_.__p_[__i];
			return __os;
		}

		template <class _CharT, class _Traits, class _IT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   dirichlet_distribution<_IT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			size_t __n;
			__is >> __n;
			std::vector<double> __p(__n);
			for (size_t __i = 0; __i < __n; ++__i)
				__is >> __p[__i];
			if (!__is.fail())
				swap(__x.__p_.__p_, __p);
			return __is;
		}

		// multinomial_distribution

		template<class _IntType = int>
		class multinomial_distribution
		{
		public:
			// types
			typedef _IntType result_type;

			class param_type
			{
				result_type __n_ = 1;
				std::vector<double> __p_;
			public:
				typedef multinomial_distribution distribution_type;

				param_type() : __p_({0.5, 0.5}) {}
				template<class _InputIterator>
				param_type(result_type __n, _InputIterator __f, _InputIterator __l)
					: __n_(__n), __p_(__f, __l) {__init();}
#ifndef _LIBCPP_CXX03_LANG
				param_type(result_type __n, std::initializer_list<double> __wl)
					: __n_(__n), __p_(__wl.begin(), __wl.end()) {__init();}
#endif  // _LIBCPP_CXX03_LANG
				template<class _UnaryOperation>
				param_type(result_type __n, size_t __nw, double __xmin, double __xmax,
					   _UnaryOperation __fw);

				result_type n() const {return __n_;}
				std::vector<double> p() const;

				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__p_ == __y.__p_ && __x.__n_ == __y.__n_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}

			private:
				void __init();

				friend class multinomial_distribution;

				template <class _CharT, class _Traits, class _IT>
				friend
				std::basic_ostream<_CharT, _Traits>&
				operator<<(std::basic_ostream<_CharT, _Traits>& __os,
					   const multinomial_distribution<_IT>& __x);

				template <class _CharT, class _Traits, class _IT>
				friend
				std::basic_istream<_CharT, _Traits>&
				operator>>(std::basic_istream<_CharT, _Traits>& __is,
					   multinomial_distribution<_IT>& __x);
			};

		private:
			param_type __p_;

		public:
			// constructor and reset functions
			multinomial_distribution() {}
			template<class _InputIterator>
			multinomial_distribution(result_type __n, _InputIterator __f, _InputIterator __l)
				: __p_(__n, __f, __l) {}
#ifndef _LIBCPP_CXX03_LANG
			multinomial_distribution(result_type __n, std::initializer_list<double> __wl)
				: __p_(__n, __wl) {}
#endif  // _LIBCPP_CXX03_LANG
			template<class _UnaryOperation>
			multinomial_distribution(result_type __n, size_t __nw, double __xmin, double __xmax,
					      _UnaryOperation __fw)
				: __p_(__n, __nw, __xmin, __xmax, __fw) {}
			explicit multinomial_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			std::vector<result_type> operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> std::vector<result_type> operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type n() const {return __p_.n();}
			std::vector<double> p() const {return __p_.p();}

			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}

			result_type min() const {return 0;}
			result_type max() const {return __p_.n();}

			friend bool operator==(const multinomial_distribution& __x,
					       const multinomial_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const multinomial_distribution& __x,
					       const multinomial_distribution& __y)
			{return !(__x == __y);}

			template <class _CharT, class _Traits, class _IT>
			friend
			std::basic_ostream<_CharT, _Traits>&
			operator<<(std::basic_ostream<_CharT, _Traits>& __os,
				   const multinomial_distribution<_IT>& __x);

			template <class _CharT, class _Traits, class _IT>
			friend
			std::basic_istream<_CharT, _Traits>&
			operator>>(std::basic_istream<_CharT, _Traits>& __is,
				   multinomial_distribution<_IT>& __x);
		};

		template<class _IntType>
		template<class _UnaryOperation>
		multinomial_distribution<_IntType>::param_type::param_type(result_type __n,
									   size_t __nw,
									double __xmin,
									double __xmax,
									_UnaryOperation __fw)
		{
			if (__nw > 1)
				{
					__p_.reserve(__nw - 1);
					double __d = (__xmax - __xmin) / __nw;
					double __d2 = __d / 2;
					for (size_t __k = 0; __k < __nw; ++__k)
						__p_.push_back(__fw(__xmin + __k * __d + __d2));
					__n_ = __n;
					__init();
				}
		}

		template<class _IntType>
		void
		multinomial_distribution<_IntType>::param_type::__init()
		{
			// result_type __sum = _VSTD::accumulate(__p_.begin(),
			// 				      __p_.end(),
			// 				      result_type());
			// for(size_t __i = 0; __i < __p_.size(); ++__i){
			// 	__p_[__i] /= __sum;
			// }
			// if (!__p_.empty())
			// 	{
			// 		if (__p_.size() > 1)
			// 			{
			// 				double __s = _VSTD::accumulate(__p_.begin(), __p_.end(), 0.0);
			// 				for (_VSTD::vector<double>::iterator __i = __p_.begin(), __e = __p_.end();
			// 				     __i < __e; ++__i)
			// 					*__i /= __s;
			// 				vector<double> __t(__p_.size() - 1);
			// 				_VSTD::partial_sum(__p_.begin(), __p_.end() - 1, __t.begin());
			// 				swap(__p_, __t);
			// 			}
			// 		else
			// 			{
			// 				__p_.clear();
			// 				__p_.shrink_to_fit();
			// 			}
			// 	}
		}

		template<class _IntType>
		std::vector<double>
		multinomial_distribution<_IntType>::param_type::p() const
		{
			// size_t __n = __p_.size();
			// _VSTD::vector<double> __p(__n+1);
			// _VSTD::adjacent_difference(__p_.begin(), __p_.end(), __p.begin());
			// if (__n > 0)
			// 	__p[__n] = 1 - __p_[__n-1];
			// else
			// 	__p[0] = 1;
			// return __p;
			return __p_;
		}

		template<class _IntType>
		template<class _URNG>
		std::vector<_IntType>
		multinomial_distribution<_IntType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::vector<double> p = __p.p();
			long pn = p.size();
			long n = __p.n();
			std::vector<result_type> r(pn);
			std::fill(r.begin(), r.end(), 0);
			double norm = 0;
			long n_sum = 0;
			double p_sum = 0;
			for(long i = 0; i < pn; i++){
				norm += p[i];
			}
			for(long i = 0; i < pn; i++){
				if(p[i] > 0){
					std::binomial_distribution d(n - n_sum, p[i] / (norm - p_sum));
					r[i] = d(__g);
				}
				n_sum += r[i];
				p_sum += p[i];
				if(n_sum == n){
					break;
				}
			}
			return r;
		}

		template <class _CharT, class _Traits, class _IT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const multinomial_distribution<_IT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			size_t __n = __x.__p_.__p_.size();
			__os << __n;
			for (size_t __i = 0; __i < __n; ++__i)
				__os << __sp << __x.__p_.__p_[__i];
			return __os;
		}

		template <class _CharT, class _Traits, class _IT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   multinomial_distribution<_IT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			size_t __n;
			__is >> __n;
			std::vector<double> __p(__n);
			for (size_t __i = 0; __i < __n; ++__i)
				__is >> __p[__i];
			if (!__is.fail())
				swap(__x.__p_.__p_, __p);
			return __is;
		}

		// hypergeometric_distribution

		template<class _IntType = int>
		class hypergeometric_distribution
		{
		public:
			// types
			typedef _IntType result_type;

			class param_type
			{
				result_type __n_, __M_, __N_;
			public:
				typedef hypergeometric_distribution distribution_type;

				explicit param_type(result_type __n = 1, result_type __M = 1, result_type __N = 2);

				result_type n() const {return __n_;}
				result_type M() const {return __M_;}
				result_type N() const {return __N_;}

				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__n_ == __y.__n_ && __x.__M_ == __y.__M_ && __x.__N_ == __y.__N_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}

				friend class hypergeometric_distribution;
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit hypergeometric_distribution(result_type __n = 1, result_type __M = 1, result_type __N = 2)
				: __p_(param_type(__n, __M, __N)) {}
			explicit hypergeometric_distribution(const param_type& __p) : __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type n() const {return __p_.n();}
			result_type M() const {return __p_.M();}
			result_type N() const {return __p_.N();}

			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}

			result_type min() const {return 0;}
			result_type max() const {return __p_.n();}

			friend bool operator==(const hypergeometric_distribution& __x,
					       const hypergeometric_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const hypergeometric_distribution& __x,
					       const hypergeometric_distribution& __y)
			{return !(__x == __y);}
		};

		template<class _IntType>
		hypergeometric_distribution<_IntType>::param_type::param_type(const result_type __n, const result_type __M, const result_type __N)
			: __n_(__n), __M_(__M), __N_(__N)
		{
			// if (0 < __p_ && __p_ < 1)
			// 	{
			// 		__r0_ = static_cast<result_type>((__t_ + 1) * __p_);
			// 		__pr_ = _VSTD::exp(__libcpp_lgamma(__t_ + 1.) -
			// 				   __libcpp_lgamma(__r0_ + 1.) -
			// 				   __libcpp_lgamma(__t_ - __r0_ + 1.) + __r0_ * _VSTD::log(__p_) +
			// 				   (__t_ - __r0_) * _VSTD::log(1 - __p_));
			// 		__odds_ratio_ = __p_ / (1 - __p_);
			// 	}
		}

		template<class _IntType>
		template<class _URNG>
		_IntType
		hypergeometric_distribution<_IntType>::operator()(_URNG& __g, const param_type& __p)
		{
			_IntType a = __p.M();
			_IntType b = __p.N();
			_IntType k = 0;
			if(__p.n() > __p.N() / 2){
				for(long i = 0; i < __p.n(); i++){
					std::uniform_real_distribution d(0., 1.);
					double r = d(__g);
					if(b * r < a){
						k++;
						if(k == __p.M()){
							return k;
						}
						a--;
					}
					b--;
				}
				return k;
			}else{
				for(long i = 0; i < __p.N() - __p.n(); i++){
					std::uniform_real_distribution d(0., 1.);
					double r = d(__g);
					if(b * r < a){
						k++;
						if(k == __p.M()){
							return __p.M() - k;
						}
						a--;
					}
					b--;
				}
				return __p.M() - k;
			}
		}

		template <class _CharT, class _Traits, class _IntType>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const hypergeometric_distribution<_IntType>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			return __os << __x.n() << __sp << __x.M() << __sp << __x.N();
		}

		template <class _CharT, class _Traits, class _IntType>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   hypergeometric_distribution<_IntType>& __x)
		{
			typedef hypergeometric_distribution<_IntType> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __n, __M, __N;
			__is >> __n >> __M >> __N;
			if (!__is.fail())
				__x.param(param_type(__n, __M, __N));
			return __is;
		}

		// multivariate_hypergeometric_distribution

		template<class _IntType = int>
		class multivariate_hypergeometric_distribution
		{
		public:
			// types
			typedef _IntType result_type;

			class param_type
			{
				result_type __n_ = 1;
				std::vector<result_type> __M_;
			public:
				typedef multivariate_hypergeometric_distribution distribution_type;

				param_type() : __M_({1, 1}) {}
				template<class _InputIterator>
				param_type(result_type __n, _InputIterator __f, _InputIterator __l)
					: __n_(__n), __M_(__f, __l) {__init();}
#ifndef _LIBCPP_CXX03_LANG
				param_type(result_type __n, std::initializer_list<double> __wl)
					: __n_(__n), __M_(__wl.begin(), __wl.end()) {__init();}
#endif  // _LIBCPP_CXX03_LANG
				template<class _UnaryOperation>
				param_type(result_type __n, size_t __nw, double __xmin, double __xmax,
					   _UnaryOperation __fw);

				result_type n() const {return __n_;}
				std::vector<result_type> M() const;

				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__M_ == __y.__M_ && __x.__n_ == __y.__n_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}

			private:
				void __init();

				friend class multivariate_hypergeometric_distribution;

				template <class _CharT, class _Traits, class _IT>
				friend
				std::basic_ostream<_CharT, _Traits>&
				operator<<(std::basic_ostream<_CharT, _Traits>& __os,
					   const multivariate_hypergeometric_distribution<_IT>& __x);

				template <class _CharT, class _Traits, class _IT>
				friend
				std::basic_istream<_CharT, _Traits>&
				operator>>(std::basic_istream<_CharT, _Traits>& __is,
					   multivariate_hypergeometric_distribution<_IT>& __x);
			};

		private:
			param_type __p_;

		public:
			// constructor and reset functions
			multivariate_hypergeometric_distribution() {}
			template<class _InputIterator>
			multivariate_hypergeometric_distribution(result_type __n, _InputIterator __f, _InputIterator __l)
				: __p_(__n, __f, __l) {}
#ifndef _LIBCPP_CXX03_LANG
			multivariate_hypergeometric_distribution(result_type __n, std::initializer_list<double> __wl)
				: __p_(__n, __wl) {}
#endif  // _LIBCPP_CXX03_LANG
			template<class _UnaryOperation>
			multivariate_hypergeometric_distribution(result_type __n, size_t __nw, double __xmin, double __xmax,
					      _UnaryOperation __fw)
				: __p_(__n, __nw, __xmin, __xmax, __fw) {}
			explicit multivariate_hypergeometric_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			std::vector<result_type> operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> std::vector<result_type> operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type n() const {return __p_.n();}
			std::vector<double> M() const {return __p_.p();}

			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}

			result_type min() const {return 0;}
			result_type max() const {return __p_.n();}

			friend bool operator==(const multivariate_hypergeometric_distribution& __x,
					       const multivariate_hypergeometric_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const multivariate_hypergeometric_distribution& __x,
					       const multivariate_hypergeometric_distribution& __y)
			{return !(__x == __y);}

			template <class _CharT, class _Traits, class _IT>
			friend
			std::basic_ostream<_CharT, _Traits>&
			operator<<(std::basic_ostream<_CharT, _Traits>& __os,
				   const multivariate_hypergeometric_distribution<_IT>& __x);

			template <class _CharT, class _Traits, class _IT>
			friend
			std::basic_istream<_CharT, _Traits>&
			operator>>(std::basic_istream<_CharT, _Traits>& __is,
				   multivariate_hypergeometric_distribution<_IT>& __x);
		};

		template<class _IntType>
		template<class _UnaryOperation>
		multivariate_hypergeometric_distribution<_IntType>::param_type::param_type(result_type __n,
									   size_t __nw,
									double __xmin,
									double __xmax,
									_UnaryOperation __fw)
		{
			if (__nw > 1)
				{
					__M_.reserve(__nw - 1);
					double __d = (__xmax - __xmin) / __nw;
					double __d2 = __d / 2;
					for (size_t __k = 0; __k < __nw; ++__k)
						__M_.push_back(__fw(__xmin + __k * __d + __d2));
					__n_ = __n;
					__init();
				}
		}

		template<class _IntType>
		void
		multivariate_hypergeometric_distribution<_IntType>::param_type::__init()
		{
			// result_type __sum = _VSTD::accumulate(__p_.begin(),
			// 				      __p_.end(),
			// 				      result_type());
			// for(size_t __i = 0; __i < __p_.size(); ++__i){
			// 	__p_[__i] /= __sum;
			// }
			// if (!__p_.empty())
			// 	{
			// 		if (__p_.size() > 1)
			// 			{
			// 				double __s = _VSTD::accumulate(__p_.begin(), __p_.end(), 0.0);
			// 				for (_VSTD::vector<double>::iterator __i = __p_.begin(), __e = __p_.end();
			// 				     __i < __e; ++__i)
			// 					*__i /= __s;
			// 				vector<double> __t(__p_.size() - 1);
			// 				_VSTD::partial_sum(__p_.begin(), __p_.end() - 1, __t.begin());
			// 				swap(__p_, __t);
			// 			}
			// 		else
			// 			{
			// 				__p_.clear();
			// 				__p_.shrink_to_fit();
			// 			}
			// 	}
		}

		template<class _IntType>
		std::vector<_IntType>
		multivariate_hypergeometric_distribution<_IntType>::param_type::M() const
		{
			// size_t __n = __p_.size();
			// _VSTD::vector<double> __p(__n+1);
			// _VSTD::adjacent_difference(__p_.begin(), __p_.end(), __p.begin());
			// if (__n > 0)
			// 	__p[__n] = 1 - __p_[__n-1];
			// else
			// 	__p[0] = 1;
			// return __p;
			return __M_;
		}

		template<class _IntType>
		template<class _URNG>
		std::vector<_IntType>
		multivariate_hypergeometric_distribution<_IntType>::operator()(_URNG& __g, const param_type& __p)
		{
			_IntType n = __p.n();
			std::vector<_IntType> M = __p.M();
			std::vector<_IntType> M_cumsum(M.size());
			std::vector<_IntType> m(M.size());
			std::fill(m.begin(), m.end(), 0);
			_IntType N = 0;
			
			for(int i = 0; i < M.size(); i++){
				N += M[i];
				M_cumsum[i] = N;
			}
			double b = N;
			for(int i = 0; i < n; i++){
				std::uniform_real_distribution d(0., 1.);
				double u = d(__g);
				u *= b;

				for(int j = 0; j < M.size(); j++){
					if(u < M_cumsum[j]){
						M[j]--;
						m[j]++;
						for(int k = j; k < M.size(); k++){
							M_cumsum[k]--;
						}
						break;
					}
				}
				b--;
			}
			return m;
		}

		template <class _CharT, class _Traits, class _IT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const multivariate_hypergeometric_distribution<_IT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			size_t __n = __x.__p_.__p_.size();
			__os << __n;
			for (size_t __i = 0; __i < __n; ++__i)
				__os << __sp << __x.__p_.__p_[__i];
			return __os;
		}

		template <class _CharT, class _Traits, class _IT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   multivariate_hypergeometric_distribution<_IT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			size_t __n;
			__is >> __n;
			std::vector<double> __p(__n);
			for (size_t __i = 0; __i < __n; ++__i)
				__is >> __p[__i];
			if (!__is.fail())
				swap(__x.__p_.__p_, __p);
			return __is;
		}

		// laplace distribution
		template<class _RealType = double>
		class laplace_distribution
		{
		public:
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __mu_;
				result_type __sigma_;
			public:
				typedef laplace_distribution distribution_type;
				explicit param_type(result_type __mu = 0, result_type __sigma = 1)
					: __mu_(__mu), __sigma_(__sigma) {}
				result_type mu() const {return __mu_;}
				result_type sigma() const {return __sigma_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__mu_ == __y.__mu_ && __x.__sigma_ == __y.__sigma_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit laplace_distribution(result_type __mu = 1, result_type __sigma = 1)
				: __p_(param_type(__mu, __sigma)) {}
			explicit laplace_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type mu() const {return __p_.mu();}
			result_type sigma() const {return __p_.sigma();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return -std::numeric_limits<result_type>::infinity();}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const laplace_distribution& __x,
					       const laplace_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const laplace_distribution& __x,
					       const laplace_distribution& __y)
			{return !(__x == __y);}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		laplace_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::exponential_distribution<result_type> ed(1.);
			std::uniform_real_distribution<result_type> ud(0, 1);
			result_type u = ud(__g);
			result_type x = ed(__g);
			if(u < 0.5){
				x = -x;
			}
			return __p.mu() + __p.sigma() * x;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const laplace_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.mu() << __sp << __x.sigma();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   laplace_distribution<_RT>& __x)
		{
			typedef laplace_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __mu;
			result_type __sigma;
			__is >> __mu >> __sigma;
			if (!__is.fail())
				__x.param(param_type(__mu, __sigma));
			return __is;
		}

		// rayleigh_distribution

		template<class _RealType = double>
		class rayleigh_distribution
		{
		public:
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __sigma_;
			public:
				typedef rayleigh_distribution distribution_type;

				explicit param_type(result_type __sigma = 1) : __sigma_(__sigma) {}

				result_type sigma() const {return __sigma_;}

				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__sigma_ == __y.__sigma_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit rayleigh_distribution(result_type __sigma = 1)
				: __p_(param_type(__sigma)) {}
			explicit rayleigh_distribution(const param_type& __p) : __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type sigma() const {return __p_.sigma();}

			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}

			result_type min() const {return 0;}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const rayleigh_distribution& __x,
					       const rayleigh_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const rayleigh_distribution& __x,
					       const rayleigh_distribution& __y)
			{return !(__x == __y);}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		rayleigh_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution d(0., 1.);
			result_type u = d(__g);
			return __p.sigma() * sqrt(-2. * log(u));
		}

		template <class _CharT, class _Traits, class _RealType>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const rayleigh_distribution<_RealType>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			return __os << __x.sigma();
		}

		template <class _CharT, class _Traits, class _RealType>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   rayleigh_distribution<_RealType>& __x)
		{
			typedef rayleigh_distribution<_RealType> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __sigma;
			__is >> __sigma;
			if (!__is.fail())
				__x.param(param_type(__sigma));
			return __is;
		}

		// erlang distribution
		template<class _RealType = double>
		class erlang_distribution
		{
		public:
			// types
			typedef _RealType result_type;

			class param_type
			{
				long __k_;
				result_type __lambda_;
			public:
				typedef erlang_distribution distribution_type;
				explicit param_type(long __k = 1, result_type __lambda = 1)
					: __k_(__k), __lambda_(__lambda) {}
				long k() const {return __k_;}
				result_type lambda() const {return __lambda_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__k_ == __y.__k_ && __x.__lambda_ == __y.__lambda_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit erlang_distribution(long __k = 1, result_type __lambda = 1)
				: __p_(param_type(__k, __lambda)) {}
			explicit erlang_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			long k() const {return __p_.k();}
			result_type lambda() const {return __p_.lambda();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return 0;}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const erlang_distribution& __x,
					       const erlang_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const erlang_distribution& __x,
					       const erlang_distribution& __y)
			{return !(__x == __y);}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		erlang_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution<result_type> d(0, 1);
			std::cout << "k = " << __p.k() << "\n";
			long k = __p.k();
			result_type product = 1.;
			for(long i = 0; i < k; i++){
				result_type u = 0.;
				while((u = d(__g)) == 0.){}
				product *= u;
			}
			return (-1. / __p.lambda()) * log(product);
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const erlang_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.k() << __sp << __x.lambda();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   erlang_distribution<_RT>& __x)
		{
			typedef erlang_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			long __k;
			result_type __lambda;
			__is >> __k >> __lambda;
			if (!__is.fail())
				__x.param(param_type(__k, __lambda));
			return __is;
		}

		// params
		class uniform_int_distribution_param_type : public std::uniform_int_distribution<long>::param_type
		{
		public:
			uniform_int_distribution_param_type(void) : std::uniform_int_distribution<long>::param_type() {}
			uniform_int_distribution_param_type(long p1, long p2) : std::uniform_int_distribution<long>::param_type(p1, p2) {}
			long param1(void){return a();}
			long param2(void){return b();}
		};
		
		class uniform_real_distribution_param_type : public std::uniform_real_distribution<double>::param_type
		{
		public:
			uniform_real_distribution_param_type(void) : std::uniform_real_distribution<double>::param_type() {}
			uniform_real_distribution_param_type(double p1, double p2) : std::uniform_real_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
		};

		class bernoulli_distribution_param_type : public std::bernoulli_distribution::param_type
		{
		public:
			bernoulli_distribution_param_type(void) : std::bernoulli_distribution::param_type() {}
			bernoulli_distribution_param_type(double p1) : std::bernoulli_distribution::param_type(p1) {}
			double param1(void){return p();}
		};
		
		class binomial_distribution_param_type : public std::binomial_distribution<long>::param_type
		{
		public:
			binomial_distribution_param_type(void) : std::binomial_distribution<long>::param_type() {}
			binomial_distribution_param_type(long p1, double p2) : std::binomial_distribution<long>::param_type(p1, p2) {}
			long param1(void){return t();}
			double param2(void){return p();}
		};

		class geometric_distribution_param_type : public std::geometric_distribution<long>::param_type
		{
		public:
			geometric_distribution_param_type(void) : std::geometric_distribution<long>::param_type() {}
			geometric_distribution_param_type(double p1) : std::geometric_distribution<long>::param_type(p1) {}
			double param1(void){return p();}
		};
		
		class negative_binomial_distribution_param_type : public std::negative_binomial_distribution<long>::param_type
		{
		public:
			negative_binomial_distribution_param_type(void) : std::negative_binomial_distribution<long>::param_type() {}
			negative_binomial_distribution_param_type(long p1, double p2) : std::negative_binomial_distribution<long>::param_type(p1, p2) {}
			long param1(void){return k();}
			double param2(void){return p();}
		};

		class multinomial_distribution_param_type : public x::random::multinomial_distribution<long>::param_type
		{
		public:
			multinomial_distribution_param_type(void) : x::random::multinomial_distribution<long>::param_type() {}
			multinomial_distribution_param_type(long p1, std::vector<double> p2) : x::random::multinomial_distribution<long>::param_type(p1, p2.begin(), p2.end()) {}
			long param1(void){return n();}
			std::vector<double> param2(void){return p();}
		};

		class hypergeometric_distribution_param_type : public x::random::hypergeometric_distribution<long>::param_type
		{
		public:
			hypergeometric_distribution_param_type(void) : x::random::hypergeometric_distribution<long>::param_type() {}
			hypergeometric_distribution_param_type(long p1, long p2, long p3) : x::random::hypergeometric_distribution<long>::param_type(p1, p2, p3) {}
			long param1(void){return n();}
			long param2(void){return M();}
			long param3(void){return N();}
		};

		class multivariate_hypergeometric_distribution_param_type : public x::random::multivariate_hypergeometric_distribution<long>::param_type
		{
		public:
			multivariate_hypergeometric_distribution_param_type(void) : x::random::multivariate_hypergeometric_distribution<long>::param_type() {}
			multivariate_hypergeometric_distribution_param_type(long p1, std::vector<long> p2) : x::random::multivariate_hypergeometric_distribution<long>::param_type(p1, p2.begin(), p2.end()) {}
			long param1(void){return n();}
			std::vector<long> param2(void){return M();}
		};
		
		class poisson_distribution_param_type : public std::poisson_distribution<long>::param_type
		{
		public:
			poisson_distribution_param_type(void) : std::poisson_distribution<long>::param_type() {}
			poisson_distribution_param_type(double p1) : std::poisson_distribution<long>::param_type(p1) {}
			double param1(void){return mean();}
		};

		class exponential_distribution_param_type : public std::exponential_distribution<double>::param_type
		{
		public:
			exponential_distribution_param_type(void) : std::exponential_distribution<double>::param_type() {}
			exponential_distribution_param_type(double p1) : std::exponential_distribution<double>::param_type(p1) {}
			double param1(void){return lambda();}
		};

		class gamma_distribution_param_type : public std::gamma_distribution<double>::param_type
		{
		public:
			gamma_distribution_param_type(void) : std::gamma_distribution<double>::param_type() {}
			gamma_distribution_param_type(double p1, double p2) : std::gamma_distribution<double>::param_type(p1, p2) {}
			double param1(void){return alpha();}
			double param2(void){return beta();}
		};

		class weibull_distribution_param_type : public std::weibull_distribution<double>::param_type
		{
		public:
			weibull_distribution_param_type(void) : std::weibull_distribution<double>::param_type() {}
			weibull_distribution_param_type(double p1, double p2) : std::weibull_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
		};

		class extreme_value_distribution_param_type : public std::extreme_value_distribution<double>::param_type
		{
		public:
			extreme_value_distribution_param_type(void) : std::extreme_value_distribution<double>::param_type() {}
			extreme_value_distribution_param_type(double p1, double p2) : std::extreme_value_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
		};

		class beta_distribution_param_type : public x::random::beta_distribution<double>::param_type
		{
		public:
			beta_distribution_param_type(void) : x::random::beta_distribution<double>::param_type() {}
			beta_distribution_param_type(double p1, double p2) : x::random::beta_distribution<double>::param_type(p1, p2) {}
			double param1(void){return alpha();}
			double param2(void){return beta();}
		};

		class kumaraswamy_distribution_param_type : public x::random::kumaraswamy_distribution<double>::param_type
		{
		public:
			kumaraswamy_distribution_param_type(void) : x::random::kumaraswamy_distribution<double>::param_type() {}
			kumaraswamy_distribution_param_type(double p1, double p2) : x::random::kumaraswamy_distribution<double>::param_type(p1, p2) {}
			double param1(void){return alpha();}
			double param2(void){return beta();}
		};

		class laplace_distribution_param_type : public x::random::laplace_distribution<double>::param_type
		{
		public:
			laplace_distribution_param_type(void) : x::random::laplace_distribution<double>::param_type() {}
			laplace_distribution_param_type(double p1, double p2) : x::random::laplace_distribution<double>::param_type(p1, p2) {}
			double param1(void){return mu();}
			double param2(void){return sigma();}
		};

		class erlang_distribution_param_type : public x::random::erlang_distribution<double>::param_type
		{
		public:
			erlang_distribution_param_type(void) : x::random::erlang_distribution<double>::param_type() {}
			erlang_distribution_param_type(double p1, double p2) : x::random::erlang_distribution<double>::param_type(p1, p2) {}
			long param1(void){return k();}
			double param2(void){return lambda();}
		};

		class normal_distribution_param_type : public std::normal_distribution<double>::param_type
		{
		public:
			normal_distribution_param_type(void) : std::normal_distribution<double>::param_type() {}
			normal_distribution_param_type(double p1, double p2) : std::normal_distribution<double>::param_type(p1, p2) {}
			double param1(void){return mean();}
			double param2(void){return stddev();}
		};

		class lognormal_distribution_param_type : public std::lognormal_distribution<double>::param_type
		{
		public:
			lognormal_distribution_param_type(void) : std::lognormal_distribution<double>::param_type() {}
			lognormal_distribution_param_type(double p1, double p2) : std::lognormal_distribution<double>::param_type(p1, p2) {}
			double param1(void){return m();}
			double param2(void){return s();}
		};

		class chi_squared_distribution_param_type : public std::chi_squared_distribution<double>::param_type
		{
		public:
			chi_squared_distribution_param_type(void) : std::chi_squared_distribution<double>::param_type() {}
			chi_squared_distribution_param_type(double p1) : std::chi_squared_distribution<double>::param_type(p1) {}
			double param1(void){return n();}
		};

		class cauchy_distribution_param_type : public std::cauchy_distribution<double>::param_type
		{
		public:
			cauchy_distribution_param_type(void) : std::cauchy_distribution<double>::param_type() {}
			cauchy_distribution_param_type(double p1, double p2) : std::cauchy_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
		};

		class fisher_f_distribution_param_type : public std::fisher_f_distribution<double>::param_type
		{
		public:
			fisher_f_distribution_param_type(void) : std::fisher_f_distribution<double>::param_type() {}
			fisher_f_distribution_param_type(double p1, double p2) : std::fisher_f_distribution<double>::param_type(p1, p2) {}
			double param1(void){return m();}
			double param2(void){return n();}
		};

		class student_t_distribution_param_type : public std::student_t_distribution<double>::param_type
		{
		public:
			student_t_distribution_param_type(void) : std::student_t_distribution<double>::param_type() {}
			student_t_distribution_param_type(double p1) : std::student_t_distribution<double>::param_type(p1) {}
			double param1(void){return n();}
		};

		class rayleigh_distribution_param_type : public rayleigh_distribution<double>::param_type
		{
		public:
			rayleigh_distribution_param_type(void) : rayleigh_distribution<double>::param_type() {}
			rayleigh_distribution_param_type(double p1) : rayleigh_distribution<double>::param_type(p1) {}
			double param1(void){return sigma();}
		};
		
		class discrete_distribution_param_type : public std::discrete_distribution<long>::param_type
		{
		public:
			discrete_distribution_param_type(void) : std::discrete_distribution<long>::param_type() {}
			template <class _InputIterator>
			discrete_distribution_param_type(_InputIterator __f, _InputIterator __l) : std::discrete_distribution<long>::param_type(__f, __l) {}
			discrete_distribution_param_type(std::vector<double> v) : discrete_distribution_param_type(v.begin(), v.end()) {}
			std::vector<double> param1(void){return probabilities();}
		};

		class piecewise_constant_distribution_param_type : public std::piecewise_constant_distribution<double>::param_type
		{
		public:
			piecewise_constant_distribution_param_type(void) : std::piecewise_constant_distribution<double>::param_type() {}
			template <class _InputIteratorB, class _InputIteratorW>
			piecewise_constant_distribution_param_type(_InputIteratorB __fB, _InputIteratorB __lB, _InputIteratorW __fW) : std::piecewise_constant_distribution<double>::param_type(__fB, __lB, __fW) {}
			piecewise_constant_distribution_param_type(std::vector<double> v1, std::vector<double> v2) : piecewise_constant_distribution_param_type(v1.begin(), v1.end(), v2.begin()) {}
			std::vector<double> param1(void){return intervals();}
			std::vector<double> param2(void){return densities();}
		};

		class piecewise_linear_distribution_param_type : public std::piecewise_linear_distribution<double>::param_type
		{
		public:
			piecewise_linear_distribution_param_type(void) : std::piecewise_linear_distribution<double>::param_type() {}
			template <class _InputIteratorB, class _InputIteratorW>
			piecewise_linear_distribution_param_type(_InputIteratorB __fB, _InputIteratorB __lB, _InputIteratorW __fW) : std::piecewise_linear_distribution<double>::param_type(__fB, __lB, __fW) {}
			piecewise_linear_distribution_param_type(std::vector<double> v1, std::vector<double> v2) : piecewise_linear_distribution_param_type(v1.begin(), v1.end(), v2.begin()) {}
			std::vector<double> param1(void){return intervals();}
			std::vector<double> param2(void){return densities();}
		};

		class dirichlet_distribution_param_type : public x::random::dirichlet_distribution<double>::param_type
		{
		public:
			dirichlet_distribution_param_type(void) : x::random::dirichlet_distribution<double>::param_type() {}
			template <class _InputIterator>
			dirichlet_distribution_param_type(_InputIterator __f, _InputIterator __l) : x::random::dirichlet_distribution<double>::param_type(__f, __l) {}
			dirichlet_distribution_param_type(std::vector<double> v) : dirichlet_distribution_param_type(v.begin(), v.end()) {}
			std::vector<double> param1(void){return alpha();}
		};
	}
}
