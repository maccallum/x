namespace x
{
	namespace dist
	{
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
			std::gamma_distribution<result_type> g1(__p.alpha());
			std::gamma_distribution<result_type> g2(__p.alpha());
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
	}
}
