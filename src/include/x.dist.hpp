namespace x
{
	namespace dist
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
			result_type kumaraswamy() const {return __p_.beta();}
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
				std::cout << "alpha[" << i << " = " << alpha[i] << "\n";
				std::gamma_distribution d(alpha[i], 1.0);
				
				vec[i] = d(__g);
				std::cout << "gamma(" << alpha[i] << ", " << 1.0 << ") = " << vec[i] << "\n";
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
	}
}
