/*
Copyright (c) 2018-2022 John MacCallum

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

#include "version.h"

namespace pcg
{
#ifdef PCG_RAND_HPP_INCLUDED
#undef PCG_RAND_HPP_INCLUDED
#endif
#include "pcg_random.hpp"
}

struct x_param_validation
{
	bool valid;
	char const *desc;
};

namespace x
{
	constexpr char const* copyright = "X Copyright 2018-2022, John MacCallum";
	constexpr char const* version = X_VERSION;
	namespace random
	{
		class random_device : public std::random_device
		{
		public:
			using std::random_device::random_device;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Non-deterministic random number generator using hardware entropy source.";
		};
		
		class pcg32 : public pcg::pcg32
		{
		public:
			using pcg::pcg32::pcg32;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Melissa O'Neill's 32-bit Permuted Congruential Generator";
		};

		class pcg64 : public pcg::pcg64
		{
		public:
			using pcg::pcg64::pcg64;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Melissa O'Neill's 64-bit Permuted Congruential Generator";
		};

		class minstd_rand0 : public std::minstd_rand0
		{
		public:
			using std::minstd_rand0::minstd_rand0;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Discovered in 1969 by Lewis, Goodman and Miller, adopted as \"Minimal standard\" in 1988 by Park and Miller";
		};

		class minstd_rand : public std::minstd_rand
		{
		public:
			using std::minstd_rand::minstd_rand;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Newer \"Minimum standard\", recommended by Park, Miller, and Stockmeyer in 1993";
		};

		class mt19937 : public std::mt19937
		{
		public:
			using std::mt19937::mt19937;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "32-bit Mersenne Twister by Matsumoto and Nishimura, 1998";
		};

		class mt19937_64 : public std::mt19937_64
		{
		public:
			using std::mt19937_64::mt19937_64;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "64-bit Mersenne Twister by Matsumoto and Nishimura, 2000";
		};

		class ranlux24 : public std::ranlux24
		{
		public:
			using std::ranlux24::ranlux24;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "24-bit RANLUX generator by Martin Lüscher and Fred James, 1994";
		};

		class ranlux48 : public std::ranlux48
		{
		public:
			using std::ranlux48::ranlux48;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "48-bit RANLUX generator by Martin Lüscher and Fred James, 1994";
		};

		class knuth_b : public std::knuth_b
		{
		public:
			using std::knuth_b::knuth_b;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Knuth's generator shuffles sequences generated with minstd_rand0";
		};
		
		template <typename IntType = int>
		//using uniform_int_distribution = std::uniform_int_distribution<IntType>;
		class uniform_int_distribution : public std::uniform_int_distribution<IntType>
		{
		public:
			using std::uniform_int_distribution<IntType>::uniform_int_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random integers with equal probability in the range [a,b] (inclusive).";
			static const int nparams = 2;
			static constexpr const char * const param_a_desc = "Lower bound (inclusive)";
			static constexpr const char * const param_b_desc = "Upper bound (inclusive)";
			static constexpr const char * const param_desc_list[nparams] = {param_a_desc, param_b_desc};
			struct x_param_validation validparams()
			{
				if(this->a() < this->b()){
					return {true, ""};
				}else{
					return {false, "a < b"};
				}
			}

			double pdf(double x) {return 1. / ((this->b() - this->a()) + 1);}
			double cdf(double x) {return (floor(x) - this->a() + 1) / ((this->b() - this->a()) + 1);}
			static constexpr bool meandef = 1;
			double mean() {return (this->a() + this->b()) / 2.;}
			static constexpr bool mediandef = 1;
			double median() {return this->mean();}
			static constexpr  bool modedef = false;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return (pow((this->b() - this->a()) + 1, 2.) - 1) / 12;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return -((6 * (pow((this->b() - this->a()) + 1, 2.) + 1)) / (5 * (pow((this->b() - this->a()) + 1, 2.) - 1)));}
			
		};
		template <typename RealType = double>
		//using uniform_real_distribution = std::uniform_real_distribution<RealType>;
		class uniform_real_distribution : public std::uniform_real_distribution<RealType>
		{
		public:
			using std::uniform_real_distribution<RealType>::uniform_real_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random numbers in the range [a,b) where all intervals of the same length are equally probable.";
			static const int nparams = 2;
			static constexpr const char * const param_a_desc = "Lower bound (inclusive)";
			static constexpr const char * const param_b_desc = "Upper bound (inclusive)";
			static constexpr const char * const param_desc_list[nparams] = {param_a_desc, param_b_desc};
			struct x_param_validation validparams()
			{
				if(this->a() < this->b()){
					return {true, ""};
				}else{
					return {false, "a < b"};
				}
			}

			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		
		template <typename IntType = int>
		//using bernoulli_distribution = std::bernoulli_distribution;
		class bernoulli_distribution : public std::bernoulli_distribution
		{
		public:
			using std::bernoulli_distribution::bernoulli_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a 1 with probability p, and 0 with probability 1-p. Simulates a coin toss.";
			static const int nparams = 1;
			static constexpr const char * const param_p_desc = "Probability of success";
			static constexpr const char * const param_desc_list[nparams] = {param_p_desc};
			struct x_param_validation validparams()
			{
				if(this->p() >= 0 && this->p() <= 1){
					return {true, ""};
				}else{
					return {false, "0 <= p <= 1"};
				}
			}

			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename IntType = int>
		//using binomial_distribution = std::binomial_distribution<IntType>;
		class binomial_distribution : public std::binomial_distribution<IntType>
		{
		public:
			using std::binomial_distribution<IntType>::binomial_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random integers in the range [0,t], which represent the number of successes in t trials each with probability p.";
			static const int nparams = 2;
			static constexpr const char * const param_t_desc = "Number of trials";
			static constexpr const char * const param_p_desc = "Probability of success";
			static constexpr const char * const param_desc_list[nparams] = {param_t_desc, param_p_desc};
			struct x_param_validation validparams()
			{
				if(this->t() < 0){
					return {false, "t >= 0"};
				}else if(this->p() < 0 || this->p() > 1){
					return {false, "0 <= p <= 1"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename IntType = int>
		//using geometric_distribution = std::geometric_distribution<IntType>;
		class geometric_distribution : public std::geometric_distribution<IntType>
		{
		public:
			using std::geometric_distribution<IntType>::geometric_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random integers which represent the number of unsuccessful trials, each with probability p, before a first success.";
			static const int nparams = 1;
			static constexpr const char * const param_p_desc = "Probability of success";
			static constexpr const char * const param_desc_list[nparams] = {param_p_desc};
			struct x_param_validation validparams()
			{
				if(this->p() <= 0 || this->p() > 1){
					return {false, "0 < p <= 1"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename IntType = int>
		//using negative_binomial_distribution = std::negative_binomial_distribution<IntType>;
		class negative_binomial_distribution : public std::negative_binomial_distribution<IntType>
		{
		public:
			using std::negative_binomial_distribution<IntType>::negative_binomial_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random integers which represent the number of successful trials, each with probability p, before k unsuccessful trials.";
			static const int nparams = 2;
			static constexpr const char * const param_k_desc = "Number of failures";
			static constexpr const char * const param_p_desc = "Probability of success";
			static constexpr const char * const param_desc_list[nparams] = {param_k_desc, param_p_desc};
			struct x_param_validation validparams()
			{
				if(this->k() <= 0){
					return {false, "k > 0"};
				}else if(this->p() < 0 || this->p() > 1){
					return {false, "0 <= p <= 1"};
				}else{
					return {true, ""};
				}
			}

			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		
		template <typename IntType = int>
		//using poisson_distribution = std::poisson_distribution<IntType>;
		class poisson_distribution : public std::poisson_distribution<IntType>
		{
		public:
			using std::poisson_distribution<IntType>::poisson_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random integers which represent the number of events that occurred during a fixed interval with mean rate of events.";
			static const int nparams = 1;
			static constexpr const char * const param_mean_desc = "Mean number of events";
			static constexpr const char * const param_desc_list[nparams] = {param_mean_desc};
			struct x_param_validation validparams()
			{
				if(this->mean() <= 0){
					return {false, "mean > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using exponential_distribution = std::exponential_distribution<RealType>;
		class exponential_distribution : public std::exponential_distribution<RealType>
		{
		public:
			using std::exponential_distribution<RealType>::exponential_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random numbers which represent the interval between two independant events defined by a constant average rate of occurrence (lambda).";
			static const int nparams = 1;
			static constexpr const char * const param_lambda_desc = "Rate of events";
			static constexpr const char * const param_desc_list[nparams] = {param_lambda_desc};
			struct x_param_validation validparams()
			{
				if(this->lambda() <= 0){
					return {false, "lambda > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using gamma_distribution = std::gamma_distribution<RealType>;
		class gamma_distribution : public std::gamma_distribution<RealType>
		{
		public:
			using std::gamma_distribution<RealType>::gamma_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random numbers which are the sum of alpha exponential distributions, each with beta as its parameter.";
			static const int nparams = 2;
			static constexpr const char * const param_alpha_desc = "Shape";
			static constexpr const char * const param_beta_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_alpha_desc, param_beta_desc};
			struct x_param_validation validparams()
			{
				if(this->alpha() <= 0){
					return {false, "alpha > 0"};
				}else if(this->beta() <= 0){
					return {false, "beta > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using weibull_distribution = std::weibull_distribution<RealType>;
		class weibull_distribution : public std::weibull_distribution<RealType>
		{
		public:
			using std::weibull_distribution<RealType>::weibull_distribution;
			//static constexpr const char * const desc_long = "";
			//static constexpr const char * const desc_short = "Produces random numbers which represent the lifetime for which the death probability is proportional to the a-th power of time. b is a scale parameter.";
			static constexpr const char * const desc_short = "Produces random numbers which represent the lifetime of an object. a is a shape parameter which characterizes objects as having a high degree of early failure when a < 1 (\"infant mortality rate\"), random failure when a = 1, or that the object wears out over time when a > 1. b is a scale parameter.";
			static const int nparams = 2;
			static constexpr const char * const param_a_desc = "Shape";
			static constexpr const char * const param_b_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_a_desc, param_b_desc};
			struct x_param_validation validparams()
			{
				if(this->a() <= 0){
					return {false, "a > 0"};
				}else if(this->b() <= 0){
					return {false, "b > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using extreme_value_distribution = std::extreme_value_distribution<RealType>;
		class extreme_value_distribution : public std::extreme_value_distribution<RealType>
		{
		public:
			using std::extreme_value_distribution<RealType>::extreme_value_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random numbers which are the extreme of a number of samples of a random variable.";
			static const int nparams = 2;
			static constexpr const char * const param_a_desc = "Location";
			static constexpr const char * const param_b_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_a_desc, param_b_desc};
			struct x_param_validation validparams()
			{
				if(this->b() <= 0){
					return {false, "b > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <typename RealType = double>
		//using normal_distribution = std::normal_distribution<RealType>;
		class normal_distribution : public std::normal_distribution<RealType>
		{
		public:
			using std::normal_distribution<RealType>::normal_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces normally distributed random values around a mean.";
			static const int nparams = 2;
			static constexpr const char * const param_mean_desc = "Mean";
			static constexpr const char * const param_stddev_desc = "Standard deviation";
			static constexpr const char * const param_desc_list[nparams] = {param_mean_desc, param_stddev_desc};
			struct x_param_validation validparams()
			{
				if(this->stddev() <= 0){
					return {false, "stddev > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using lognormal_distribution = std::lognormal_distribution<RealType>;
		class lognormal_distribution : public std::lognormal_distribution<RealType>
		{
		public:
			using std::lognormal_distribution<RealType>::lognormal_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces numbers from a random variable, the logarathim of which is normally distributed.";
			static const int nparams = 2;
			static constexpr const char * const param_m_desc = "Mean";
			static constexpr const char * const param_s_desc = "Standard deviation";
			static constexpr const char * const param_desc_list[nparams] = {param_m_desc, param_s_desc};
			struct x_param_validation validparams()
			{
				if(this->s() <= 0){
					return {false, "s > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using chi_squared_distribution = std::chi_squared_distribution<RealType>;
		class chi_squared_distribution : public std::chi_squared_distribution<RealType>
		{
		public:
			using std::chi_squared_distribution<RealType>::chi_squared_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random numbers from the sum of the squares of k independent normal random variables.";
			static const int nparams = 1;
			static constexpr const char * const param_n_desc = "Degrees of freedom";
			static constexpr const char * const param_desc_list[nparams] = {param_n_desc};
			struct x_param_validation validparams()
			{
				if(this->n() <= 0){
					return {false, "n > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using cauchy_distribution = std::cauchy_distribution<RealType>;
		class cauchy_distribution : public std::cauchy_distribution<RealType>
		{
		public:
			using std::cauchy_distribution<RealType>::cauchy_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random numbers which are the ratio of two independent normally distributed random variables zero mean.";
			static const int nparams = 2;
			static constexpr const char * const param_a_desc = "Location";
			static constexpr const char * const param_b_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_a_desc, param_b_desc};
			struct x_param_validation validparams()
			{
				if(this->b() <= 0){
					return {false, "b > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using fisher_f_distribution = std::fisher_f_distribution<RealType>;
		class fisher_f_distribution : public std::fisher_f_distribution<RealType>
		{
		public:
			using std::fisher_f_distribution<RealType>::fisher_f_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values which are the result of dividing two independent chi-squared distributions of m and n degrees of freedom.";
			static const int nparams = 2;
			static constexpr const char * const param_m_desc = "First degree of freedom";
			static constexpr const char * const param_n_desc = "Second degree of freedom";
			static constexpr const char * const param_desc_list[nparams] = {param_m_desc, param_n_desc};
			struct x_param_validation validparams()
			{
				if(this->m() <= 0){
					return {false, "m > 0"};
				}else if(this->n() <= 0){
					return {false, "n > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using student_t_distribution = std::student_t_distribution<RealType>;
		class student_t_distribution : public std::student_t_distribution<RealType>
		{
		public:
			using std::student_t_distribution<RealType>::student_t_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values which approach a standard normal distribution as n (the sample size) increases.";
			static const int nparams = 1;
			static constexpr const char * const param_n_desc = "Degrees of freedom";
			static constexpr const char * const param_desc_list[nparams] = {param_n_desc};
			struct x_param_validation validparams()
			{
				if(this->n() <= 0){
					return {false, "n > 0"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <typename IntType = int>
		//using discrete_distribution = std::discrete_distribution<IntType>;
		class discrete_distribution : public std::discrete_distribution<IntType>
		{
		public:
			using std::discrete_distribution<IntType>::discrete_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values according to a list of n non-negative weights.";
			static const int nparams = 1;
			static constexpr const char * const param_probabilities_desc = "Probabilities";
			static constexpr const char * const param_desc_list[nparams] = {param_probabilities_desc};
			struct x_param_validation validparams()
			{
				if(this->probabilities().size() == 0){
					return {false, "probabilities must contain at least one value"};
				}else{
					for(int i = 0; i < this->probabilities().size(); i++){
						if(this->probabilities()[i] <= 0){
							return {false, "p_i > 0"};
						}
					}
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using piecewise_constant_distribution = std::piecewise_constant_distribution<RealType>;
		class piecewise_constant_distribution : public std::piecewise_constant_distribution<RealType>
		{
		public:
			using std::piecewise_constant_distribution<RealType>::piecewise_constant_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values that are uniformly distributed over a sequence of subintervals.";
			static const int nparams = 2;
			static constexpr const char * const param_intervals_desc = "Intervals";
			static constexpr const char * const param_densities_desc = "Densities";
			static constexpr const char * const param_desc_list[nparams] = {param_intervals_desc, param_densities_desc};
			struct x_param_validation validparams()
			{
				if(this->intervals().size() != (this->densities().size() + 1)){
					return {false, "intervals should contain one more element than densities"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};
		template <typename RealType = double>
		//using piecewise_linear_distribution = std::piecewise_linear_distribution<RealType>;
		class piecewise_linear_distribution : public std::piecewise_linear_distribution<RealType>
		{
		public:
			using std::piecewise_linear_distribution<RealType>::piecewise_linear_distribution;
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values that are distributed over a sequence of subintervals.";
			static const int nparams = 2;
			static constexpr const char * const param_intervals_desc = "Intervals";
			static constexpr const char * const param_densities_desc = "Densities";
			static constexpr const char * const param_desc_list[nparams] = {param_intervals_desc, param_densities_desc};
			struct x_param_validation validparams()
			{
				if(this->intervals().size() != (this->densities().size() + 1)){
					return {false, "intervals should contain one more element than densities"};
				}else{
					return {true, ""};
				}
			}
			
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		// beta distribution		
		template<class _RealType = double>
		class beta_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values over the interval [0,1], which represent a distribution of probabilities.";
			static const int nparams = 2;
			static constexpr const char * const param_alpha_desc = "First shape parameter";
			static constexpr const char * const param_beta_desc = "Second shape parameter";
			static constexpr const char * const param_desc_list[nparams] = {param_alpha_desc, param_beta_desc};
			struct x_param_validation validparams()
			{
				if(this->alpha() <= 0){
					return {false, "alpha > 0"};
				}else if(this->beta() <= 0){
					return {false, "beta > 0"};
				}else{
					return {true, ""};
				}
			}
			
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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

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

		template <class _RealType>
		template<class _URNG>
		_RealType
		beta_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			gamma_distribution<result_type> g1(__p.alpha(), 1.);
			gamma_distribution<result_type> g2(__p.beta(), 1.);
			result_type a = g1(__g);
			return a / (a + g2(__g));
		}

		// kumaraswamy distribution
		template<class _RealType = double>
		class kumaraswamy_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values on the interval (0,1), similar to the beta distribution.";
			static const int nparams = 2;
			static constexpr const char * const param_alpha_desc = "First shape parameter";
			static constexpr const char * const param_beta_desc = "Second shape parameter";
			static constexpr const char * const param_desc_list[nparams] = {param_alpha_desc, param_beta_desc};
			struct x_param_validation validparams()
			{
				if(this->alpha() <= 0){
					return {false, "alpha > 0"};
				}else if(this->alpha() <= 0){
					return {false, "beta > 0"};
				}else{
					return {true, ""};
				}
			}
			
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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		kumaraswamy_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			gamma_distribution<result_type> g1(1.0, 1.0);
			gamma_distribution<result_type> g2(__p.beta(), 1.0);
			result_type a = g1(__g);
			result_type beta = a / (a + g2(__g));
			return pow(beta, 1. / __p.alpha());
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
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values on the interval (0,1) derived from the multivariate beta distribution.";
			static const int nparams = 1;
			static constexpr const char * const param_alpha_desc = "Concentrations";
			static constexpr const char * const param_desc_list[nparams] = {param_alpha_desc};
			struct x_param_validation validparams()
			{
				if(this->alpha().size() < 2){
					return {false, "alpha must contain 2 or more values"};
				}else{
					for(int i = 0; i < this->alpha().size(); i++){
						if(this->alpha()[i] <= 0){
							return {false, "alpha_i > 0"};
						}
					}
					return {true, ""};
				}
			}
			
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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
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
				gamma_distribution<_RealType> d(alpha[i], 1.0);
				
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
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values that represent the number occurences of some number of outcomes described by the list probabilities p, after n trials.";
			static const int nparams = 2;
			static constexpr const char * const param_n_desc = "Number of trials";
			static constexpr const char * const param_p_desc = "Probabilities";
			static constexpr const char * const param_desc_list[nparams] = {param_n_desc, param_p_desc};
			struct x_param_validation validparams()
			{
				if(this->n() <= 0){
					return {false, "n > 0"};
				}else if(this->p().size() < 1){
					return {false, "p must contain at least one probability"};
				}else{
					return {true, ""};
				}
			}
			
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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
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
					double pp = 1.0;
					if(i < pn - 1){
						pp = p[i] / (norm - p_sum);
					}
					binomial_distribution<_IntType> d(n - n_sum, pp);
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
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values that represent the probability of n successes in M trials given N objects of interest.";
			static const int nparams = 3;
			static constexpr const char * const param_n_desc = "Number of successes";
			static constexpr const char * const param_M_desc = "Population size";
			static constexpr const char * const param_N_desc = "Number of objects of interest";
			static constexpr const char * const param_desc_list[nparams] = {param_n_desc, param_M_desc, param_N_desc};
			struct x_param_validation validparams()
			{
				if(this->M() < 0){
					return {false, "M >= 0"};
				}else if(this->n() <= 0 || this->n() > this->M()){
					return {false, "0 < n <= M"};
				}else if(this->N() <= 0 || this->N() > this->M()){
					return {false, "0 < N <= M"};
				}else{
					return {true, ""};
				}
			}
			
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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
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
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values from the generalized hypergeometric distribution.";
			static const int nparams = 2;
			static constexpr const char * const param_n_desc = "Number of trials";
			static constexpr const char * const param_M_desc = "Population sizes";
			static constexpr const char * const param_desc_list[nparams] = {param_n_desc, param_M_desc};
			struct x_param_validation validparams()
			{
				if(this->n() <= 0){
					return {false, "n > 0"};
				}else if(this->M().size() <= 0){
					return {false, "M must contain at least one element"};
				}else{
					_IntType sum = 0;
					for(int i = 0; i < this->M().size(); i++){
						if(this->M()[i] < 0){
							return {false, "M_i >= 0"};
						}
						sum += this->M()[i];
					}
					if(this->n() > sum){
						return {false, "n <= sum(M)"};
					}else{
						return {true, ""};
					}
				}
			}
			
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
			std::vector<result_type> M() const {return __p_.M();}

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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
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
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces random values from two symmetric exponential distributions centered around mu.";
			static const int nparams = 2;
			static constexpr const char * const param_mu_desc = "Location";
			static constexpr const char * const param_sigma_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_mu_desc, param_sigma_desc};
			struct x_param_validation validparams()
			{
				if(this->sigma() <= 0){
					return {false, "sigma > 0"};
				}else{
					return {true, ""};
				}
			}
			
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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		laplace_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			exponential_distribution<result_type> ed(1.);
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
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value from the Rayleigh distribution, which is a special case of both the Chi and Weibull distributions.";
			static const int nparams = 1;
			static constexpr const char * const param_sigma_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_sigma_desc};
			struct x_param_validation validparams()
			{
				if(this->sigma() <= 0){
					return {false, "sigma > 0"};
				}else{
					return {true, ""};
				}
			}
			
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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		rayleigh_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution<result_type> d(0., 1.);
			result_type u;
			do{
				u = d(__g);
			}while(u == 0);
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
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value from the sum of k independent exponential variables with mean 1/lambda.";
			static const int nparams = 2;
			static constexpr const char * const param_k_desc = "Shape";
			static constexpr const char * const param_lambda_desc = "Rate";
			static constexpr const char * const param_desc_list[nparams] = {param_k_desc, param_lambda_desc};
			struct x_param_validation validparams()
			{
				if(this->k() < 1){
					return {false, "k > 0"};
				}else if(this->lambda() <= 0){
					return {false, "lambda > 0"};
				}else{
					return {true, ""};
				}
			}
			
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

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		erlang_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution<result_type> d(0, 1);
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

		// gaussian_tail distribution
		template<class _RealType = double>
		class gaussian_tail_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value drawn from the upper tail of the normal distribution.";
			static const int nparams = 2;
			static constexpr const char * const param_a_desc = "Lower bound";
			static constexpr const char * const param_sigma_desc = "Standard deviation";
			static constexpr const char * const param_desc_list[nparams] = {param_a_desc, param_sigma_desc};
			struct x_param_validation validparams()
			{
				if(this->sigma() <= 0){
					return {false, "sigma > 0"};
				}else{
					return {true, ""};
				}
			}
			
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __a_;
				result_type __sigma_;
			public:
				typedef gaussian_tail_distribution distribution_type;
				explicit param_type(long __a = 1, result_type __sigma = 1)
					: __a_(__a), __sigma_(__sigma) {}
				result_type a() const {return __a_;}
				result_type sigma() const {return __sigma_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__a_ == __y.__a_ && __x.__sigma_ == __y.__sigma_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit gaussian_tail_distribution(long __a = 1, result_type __sigma = 1)
				: __p_(param_type(__a, __sigma)) {}
			explicit gaussian_tail_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type a() const {return __p_.a();}
			result_type sigma() const {return __p_.sigma();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return nextafter(a(), a() + 1);}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const gaussian_tail_distribution& __x,
					       const gaussian_tail_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const gaussian_tail_distribution& __x,
					       const gaussian_tail_distribution& __y)
			{return !(__x == __y);}

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		gaussian_tail_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution<result_type> du(0, 1);
			std::normal_distribution<result_type> dn(1.0);
			result_type sigma = __p.sigma();
			result_type adivsigma = __p.a() / __p.sigma();
			if(adivsigma < 1){
				double x = dn(__g);
				while(x < adivsigma){
					x = dn(__g);
				}
				return x * sigma;
			}else{
				double u, v, x;
				do{
					u = du(__g);
					do{
						v = du(__g);
					}while(v == 0.);
					x = sqrt(adivsigma * adivsigma - 2 * log(v));
				}while(x * u > adivsigma);
				return x * sigma;
			}
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const gaussian_tail_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.a() << __sp << __x.sigma();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   gaussian_tail_distribution<_RT>& __x)
		{
			typedef gaussian_tail_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __a;
			result_type __sigma;
			__is >> __a >> __sigma;
			if (!__is.fail())
				__x.param(param_type(__a, __sigma));
			return __is;
		}

		// bivariate_normal distribution
		template<class _RealType = double>
		class bivariate_normal_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value from a two-dimensional normal distribution.";
			static const int nparams = 3;
			static constexpr const char * const param_sigmax_desc = "Standard deviation in the X direction";
			static constexpr const char * const param_sigmay_desc = "Standard deviation in the Y direction";
			static constexpr const char * const param_rho_desc = "Correlation coefficient";
			static constexpr const char * const param_desc_list[nparams] = {param_sigmax_desc, param_sigmay_desc, param_rho_desc};
			struct x_param_validation validparams()
			{
				if(this->sigmax() <= 0){
					return {false, "sigmax > 0"};
				}else if(this->sigmay() <= 0){
					return {false, "sigmay > 0"};
				}else if(this->rho() < -1 || this->rho() > 1){
					return {false, "-1 <= rho <= 1"};
				}else{
					return {true, ""};
				}
			}
			
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __sigmax_;
				result_type __sigmay_;
				result_type __rho_;
			public:
				typedef bivariate_normal_distribution distribution_type;
				explicit param_type(result_type __sigmax = 1, result_type __sigmay = 1, result_type __rho = 0)
					: __sigmax_(__sigmax), __sigmay_(__sigmay), __rho_(__rho) {}
				result_type sigmax() const {return __sigmax_;}
				result_type sigmay() const {return __sigmay_;}
				result_type rho() const {return __rho_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__sigmax_ == __y.__sigmax_ && __x.__sigmay_ == __y.__sigmay_ && __x.rho_ == __y.rho_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit bivariate_normal_distribution(long __sigmax = 1, result_type __sigmay = 1, result_type __rho = 0)
				: __p_(param_type(__sigmax, __sigmay, __rho)) {}
			explicit bivariate_normal_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			std::vector<result_type> operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> std::vector<result_type> operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type sigmax() const {return __p_.sigmax();}
			result_type sigmay() const {return __p_.sigmay();}
			result_type rho() const {return __p_.rho();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return -std::numeric_limits<result_type>::infinity();}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const bivariate_normal_distribution& __x,
					       const bivariate_normal_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const bivariate_normal_distribution& __x,
					       const bivariate_normal_distribution& __y)
			{return !(__x == __y);}

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		std::vector<_RealType>
		bivariate_normal_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution<result_type> du(0, 1);
			result_type u1, u2, r;
			do{
				u1 = -1 + 2 * du(__g);
				u2 = -1 + 2 * du(__g);
				r = u1 * u1 + u2 * u2;
			}while(r > 1.0 || r == 0);
			result_type s = sqrt(-2. * log(r) / r);
			result_type rho = __p.rho();
			result_type x = __p.sigmax() * u1 * s;
			result_type y = __p.sigmay() * (rho * u1 + sqrt(1 - rho * rho) * u2) * s;
			std::vector<_RealType> out{x, y};
			return out;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const bivariate_normal_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.sigmax() << __sp << __x.sigmay() << __sp << __x.rho();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   bivariate_normal_distribution<_RT>& __x)
		{
			typedef bivariate_normal_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __sigmax;
			result_type __sigmay;
			result_type __rho;
			__is >> __sigmax >> __sigmay >> __rho;
			if (!__is.fail())
				__x.param(param_type(__sigmax, __sigmay, __rho));
			return __is;
		}

		// exponential power distribution		
		template<class _RealType = double>
		class exponential_power_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value from a normal distribution with an added shape parameter beta.";
			static const int nparams = 2;
			static constexpr const char * const param_alpha_desc = "Scale";
			static constexpr const char * const param_beta_desc = "Shape";
			static constexpr const char * const param_desc_list[nparams] = {param_alpha_desc, param_beta_desc};
			struct x_param_validation validparams()
			{
				if(this->alpha() <= 0){
					return {false, "alpha > 0"};
				}else if(this->beta() <= 0){
					return {false, "beta > 0"};
				}else{
					return {true, ""};
				}
			}
			
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __alpha_;
				result_type __beta_;
			public:
				typedef exponential_power_distribution distribution_type;
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
			explicit exponential_power_distribution(result_type __alpha = 1, result_type __beta = 1)
				: __p_(param_type(__alpha, __beta)) {}
			explicit exponential_power_distribution(const param_type& __p)
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
			result_type min() const {return -std::numeric_limits<result_type>::infinity();}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const exponential_power_distribution& __x,
					       const exponential_power_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const exponential_power_distribution& __x,
					       const exponential_power_distribution& __y)
			{return !(__x == __y);}

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		exponential_power_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			result_type a = __p.alpha();
			result_type b = __p.beta();

			if(b < 1 || b > 4){
				std::uniform_real_distribution<result_type> du(0, 1);
				gamma_distribution<result_type> g(1. / b, 1.);
				result_type u = du(__g);
				result_type v = g(__g);
				result_type z = a * pow(v, 1 / b);
				if(u > 0.5){
					return z;
				}else{
					return -z;
				}
			}else if(b == 1){
				laplace_distribution<result_type> l(0, a);
				return l(__g);
			}else if(b < 2){
				double x, h, u, B = pow(1 / b, 1 / b);
				laplace_distribution<result_type> l(0, B);
				std::uniform_real_distribution<result_type> du(0, 1);
				do{
					x = l(__g);
					do{
						u = du(__g);
					}while(u == 0);
					h = -pow(fabs(x), b) + fabs(x) / B - 1 + (1 / b);
				}while(log(u) > h);
				return a * x;
			}else if(b == 2){
				normal_distribution<result_type> g(0., a / sqrt(2.));
				return g(__g);
			}else{
				double x, h, u, B = pow(1 / b, 1 / b);
				normal_distribution<result_type> g(0., B);
				std::uniform_real_distribution<result_type> du(0, 1);
				do{
					x = g(__g);
					do{
						u = du(__g);
					}while(u == 0);
					h = -pow(fabs(x), b) + (x * x) / (2 * B * B) * (1 / b) - 0.5;
				}while(log(u) > h);
				return a * x;
			}
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const exponential_power_distribution<_RT>& __x)
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
			   exponential_power_distribution<_RT>& __x)
		{
			typedef exponential_power_distribution<_RT> _Eng;
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

		// rayleigh_tail distribution
		template<class _RealType = double>
		class rayleigh_tail_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value from the tail of the Rayleigh distribution.";
			static const int nparams = 2;
			static constexpr const char * const param_a_desc = "Lower bound";
			static constexpr const char * const param_sigma_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_a_desc, param_sigma_desc};
			struct x_param_validation validparams()
			{
				if(this->sigma() <= 0){
					return {false, "sigma > 0"};
				}else{
					return {true, ""};
				}
			}
			
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __a_;
				result_type __sigma_;
			public:
				typedef rayleigh_tail_distribution distribution_type;
				explicit param_type(long __a = 1, result_type __sigma = 1)
					: __a_(__a), __sigma_(__sigma) {}
				result_type a() const {return __a_;}
				result_type sigma() const {return __sigma_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__a_ == __y.__a_ && __x.__sigma_ == __y.__sigma_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit rayleigh_tail_distribution(long __a = 1, result_type __sigma = 1)
				: __p_(param_type(__a, __sigma)) {}
			explicit rayleigh_tail_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type a() const {return __p_.a();}
			result_type sigma() const {return __p_.sigma();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return nextafter(a(), a() + 1);}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const rayleigh_tail_distribution& __x,
					       const rayleigh_tail_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const rayleigh_tail_distribution& __x,
					       const rayleigh_tail_distribution& __y)
			{return !(__x == __y);}

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		rayleigh_tail_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution<result_type> du(0, 1);
			result_type u, a = __p.a(), sigma = __p.sigma();
			do{
				u = du(__g);
			}while(u == 0);
			return sqrt(a * a - 2. * sigma * sigma * log(u));
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const rayleigh_tail_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.a() << __sp << __x.sigma();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   rayleigh_tail_distribution<_RT>& __x)
		{
			typedef rayleigh_tail_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __a;
			result_type __sigma;
			__is >> __a >> __sigma;
			if (!__is.fail())
				__x.param(param_type(__a, __sigma));
			return __is;
		}

		// logistic distribution
		template<class _RealType = double>
		class logistic_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value from the logistic distribution whose CDF is the logistic function.";
			static const int nparams = 2;
			static constexpr const char * const param_mu_desc = "Position";
			static constexpr const char * const param_s_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_mu_desc, param_s_desc};
			struct x_param_validation validparams()
			{
				if(this->s() <= 0){
					return {false, "s > 0"};
				}else{
					return {true, ""};
				}
			}
			
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __mu_;
				result_type __s_;
			public:
				typedef logistic_distribution distribution_type;
				explicit param_type(long __mu = 0, result_type __s = 1)
					: __mu_(__mu), __s_(__s) {}
				result_type mu() const {return __mu_;}
				result_type s() const {return __s_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__mu_ == __y.__mu_ && __x.__s_ == __y.__s_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit logistic_distribution(long __mu = 0, result_type __s = 1)
				: __p_(param_type(__mu, __s)) {}
			explicit logistic_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type mu() const {return __p_.mu();}
			result_type s() const {return __p_.s();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return -std::numeric_limits<result_type>::infinity();}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const logistic_distribution& __x,
					       const logistic_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const logistic_distribution& __x,
					       const logistic_distribution& __y)
			{return !(__x == __y);}

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		logistic_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution<result_type> du(0, 1);
			result_type u;
			do{
				u = du(__g);
			}while(u == 0 || u == 1);
			return __p.mu() + __p.s() * log(u / (1 - u));
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const logistic_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.mu() << __sp << __x.s();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   logistic_distribution<_RT>& __x)
		{
			typedef logistic_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __mu;
			result_type __s;
			__is >> __mu >> __s;
			if (!__is.fail())
				__x.param(param_type(__mu, __s));
			return __is;
		}

		// pareto distribution
		template<class _RealType = double>
		class pareto_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value that corresponds to the \"80-20\" rule, which states that 80% of outcomes are due to 20% of causes.";
			static const int nparams = 2;
			static constexpr const char * const param_a_desc = "Scale";
			static constexpr const char * const param_b_desc = "Shape";
			static constexpr const char * const param_desc_list[nparams] = {param_a_desc, param_b_desc};
			struct x_param_validation validparams()
			{
				if(this->a() <= 0){
					return {false, "a > 0"};
				}else if(this->b() <= 0){
					return {false, "b > 0"};
				}else{
					return {true, ""};
				}
			}
			
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __a_;
				result_type __b_;
			public:
				typedef pareto_distribution distribution_type;
				explicit param_type(result_type __a = 1, result_type __b = 1)
					: __a_(__a), __b_(__b) {}
				result_type a() const {return __a_;}
				result_type b() const {return __b_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__a_ == __y.__a_ && __x.__b_ == __y.__b_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit pareto_distribution(result_type __a = 1, result_type __b = 1)
				: __p_(param_type(__a, __b)) {}
			explicit pareto_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type a() const {return __p_.a();}
			result_type b() const {return __p_.b();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return __p_.a();}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const pareto_distribution& __x,
					       const pareto_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const pareto_distribution& __x,
					       const pareto_distribution& __y)
			{return !(__x == __y);}

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		pareto_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			result_type a = __p.a();
			result_type b = __p.b();

			std::uniform_real_distribution<result_type> du(0, 1);
			result_type u;
			do{
				u = du(__g);
			}while(u == 0);
			return a / pow(u, 1. / b);
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const pareto_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.a() << __sp << __x.b();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   pareto_distribution<_RT>& __x)
		{
			typedef pareto_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __a;
			result_type __b;
			__is >> __a >> __b;
			if (!__is.fail())
				__x.param(param_type(__a, __b));
			return __is;
		}

		// gumbel distribution		
		template<class _RealType = double>
		class gumbel_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value which models the maximum of a number of samples of distributions.";
			static const int nparams = 2;
			static constexpr const char * const param_mu_desc = "Location";
			static constexpr const char * const param_beta_desc = "Scale";
			static constexpr const char * const param_desc_list[nparams] = {param_mu_desc, param_beta_desc};
			struct x_param_validation validparams()
			{
				return {true, ""};
			}
			
			// types
			typedef _RealType result_type;

			class param_type
			{
				result_type __mu_;
				result_type __beta_;
			public:
				typedef gumbel_distribution distribution_type;
				explicit param_type(result_type __mu = 1, result_type __beta = 1)
					: __mu_(__mu), __beta_(__beta) {}
				result_type mu() const {return __mu_;}
				result_type beta() const {return __beta_;}
				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__mu_ == __y.__mu_ && __x.__beta_ == __y.__beta_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit gumbel_distribution(result_type __mu = 1, result_type __beta = 1)
				: __p_(param_type(__mu, __beta)) {}
			explicit gumbel_distribution(const param_type& __p)
				: __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			result_type mu() const {return __p_.mu();}
			result_type beta() const {return __p_.beta();}
			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}
			result_type min() const {return -std::numeric_limits<result_type>::infinity();}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const gumbel_distribution& __x,
					       const gumbel_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const gumbel_distribution& __x,
					       const gumbel_distribution& __y)
			{return !(__x == __y);}

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _RealType>
		template<class _URNG>
		_RealType
		gumbel_distribution<_RealType>::operator()(_URNG& __g, const param_type& __p)
		{
			std::uniform_real_distribution<result_type> du(0, 1);
			result_type m = __p.mu();
			result_type b = __p.beta();
			result_type u;
			do{
				u = du(__g);
			}while(u == 0);
			
			return m - b * log(-log(u));
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const gumbel_distribution<_RT>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			_CharT __sp = __os.widen(' ');
			__os.fill(__sp);
			__os << __x.mu() << __sp << __x.beta();
			return __os;
		}

		template <class _CharT, class _Traits, class _RT>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   gumbel_distribution<_RT>& __x)
		{
			typedef gumbel_distribution<_RT> _Eng;
			typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			result_type __mu;
			result_type __beta;
			__is >> __mu >> __beta;
			if (!__is.fail())
				__x.param(param_type(__mu, __beta));
			return __is;
		}

		// logarithmic_series_distribution

		template<class _IntType = int>
		class logarithmic_series_distribution
		{
		public:
			//static constexpr const char * const desc_long = "";
			static constexpr const char * const desc_short = "Produces a random value from a distribution with a maximum peak of 1 and a long tail controled by a shape parameter.";
			static const int nparams = 1;
			static constexpr const char * const param_p_desc = "Shape";
			static constexpr const char * const param_desc_list[nparams] = {param_p_desc};
			struct x_param_validation validparams()
			{
				if(this->p() <= 0 || this->p() >= 1){
					return {false, "0 < p < 1"};
				}else{
					return {true, ""};
				}
			}
			
			// types
			typedef _IntType result_type;

			class param_type
			{
				double __p_;
			public:
				typedef logarithmic_series_distribution distribution_type;

				explicit param_type(double __p = 0.5) : __p_(__p) {}

				double p() const {return __p_;}

				friend bool operator==(const param_type& __x, const param_type& __y)
				{return __x.__p_ == __y.__p_;}
				friend bool operator!=(const param_type& __x, const param_type& __y)
				{return !(__x == __y);}
			};

		private:
			param_type __p_;

		public:
			// constructors and reset functions
			explicit logarithmic_series_distribution(double __p = 0.5)
				: __p_(param_type(__p)) {}
			explicit logarithmic_series_distribution(const param_type& __p) : __p_(__p) {}
			void reset() {}

			// generating functions
			template<class _URNG>
			result_type operator()(_URNG& __g)
			{return (*this)(__g, __p_);}
			template<class _URNG> result_type operator()(_URNG& __g, const param_type& __p);

			// property functions
			double p() const {return __p_.p();}

			param_type param() const {return __p_;}
			void param(const param_type& __p) {__p_ = __p;}

			result_type min() const {return 1;}
			result_type max() const {return std::numeric_limits<result_type>::infinity();}

			friend bool operator==(const logarithmic_series_distribution& __x,
					       const logarithmic_series_distribution& __y)
			{return __x.__p_ == __y.__p_;}
			friend bool operator!=(const logarithmic_series_distribution& __x,
					       const logarithmic_series_distribution& __y)
			{return !(__x == __y);}

			// characterization functions
			double pdf(double x) {return 0;}
			double cdf(double x) {return 0;}
			static constexpr bool meandef = 1;
			double mean() {return 0;}
			static constexpr bool mediandef = 1;
			double median() {return 0;}
			static constexpr  bool modedef = 1;
			double mode() {return 0;}
			static constexpr bool variancedef = 1;
			double variance() {return 0;}
			static constexpr bool skewnessdef = 1;
			double skewness() {return 0;}
			static constexpr bool kurtosisdef = 1;
			double kurtosis() {return 0;}
		};

		template <class _IntType>
		template<class _URNG>
		_IntType
		logarithmic_series_distribution<_IntType>::operator()(_URNG& __g, const param_type& __p)
		{
			// http://www.nrbook.com/devroye/Devroye_files/chapter_ten.pdf
			// could implement the speed up
			std::uniform_real_distribution<double> du(0, 1);
			double u, v;
			do{
				u = du(__g);
			}while(u == 0);
			do{
				v = du(__g);
			}while(v == 0);
			return 1 + (log(v) / log(1 - pow(1. - __p.p(), u)));
		}

		template <class _CharT, class _Traits, class _IntType>
		std::basic_ostream<_CharT, _Traits>&
		operator<<(std::basic_ostream<_CharT, _Traits>& __os,
			   const logarithmic_series_distribution<_IntType>& __x)
		{
			std::__save_flags<_CharT, _Traits> __lx(__os);
			__os.flags(std::ios_base::dec | std::ios_base::left | std::ios_base::fixed |
				   std::ios_base::scientific);
			return __os << __x.p();
		}

		template <class _CharT, class _Traits, class _IntType>
		std::basic_istream<_CharT, _Traits>&
		operator>>(std::basic_istream<_CharT, _Traits>& __is,
			   logarithmic_series_distribution<_IntType>& __x)
		{
			typedef logarithmic_series_distribution<_IntType> _Eng;
			//typedef typename _Eng::result_type result_type;
			typedef typename _Eng::param_type param_type;
			std::__save_flags<_CharT, _Traits> __lx(__is);
			__is.flags(std::ios_base::dec | std::ios_base::skipws);
			double __p;
			__is >> __p;
			if (!__is.fail())
				__x.param(param_type(__p));
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
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};
		
		class uniform_real_distribution_param_type : public std::uniform_real_distribution<double>::param_type
		{
		public:
			uniform_real_distribution_param_type(void) : std::uniform_real_distribution<double>::param_type() {}
			uniform_real_distribution_param_type(double p1, double p2) : std::uniform_real_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class bernoulli_distribution_param_type : public bernoulli_distribution<long>::param_type
		{
		public:
			bernoulli_distribution_param_type(void) : bernoulli_distribution<long>::param_type() {}
			bernoulli_distribution_param_type(double p1) : bernoulli_distribution<long>::param_type(p1) {}
			double param1(void){return p();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};
		
		class binomial_distribution_param_type : public binomial_distribution<long>::param_type
		{
		public:
			binomial_distribution_param_type(void) : binomial_distribution<long>::param_type() {}
			binomial_distribution_param_type(long p1, double p2) : binomial_distribution<long>::param_type(p1, p2) {}
			long param1(void){return t();}
			double param2(void){return p();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class geometric_distribution_param_type : public geometric_distribution<long>::param_type
		{
		public:
			geometric_distribution_param_type(void) : geometric_distribution<long>::param_type() {}
			geometric_distribution_param_type(double p1) : geometric_distribution<long>::param_type(p1) {}
			double param1(void){return p();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};
		
		class negative_binomial_distribution_param_type : public negative_binomial_distribution<long>::param_type
		{
		public:
			negative_binomial_distribution_param_type(void) : negative_binomial_distribution<long>::param_type() {}
			negative_binomial_distribution_param_type(long p1, double p2) : negative_binomial_distribution<long>::param_type(p1, p2) {}
			long param1(void){return k();}
			double param2(void){return p();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class multinomial_distribution_param_type : public x::random::multinomial_distribution<long>::param_type
		{
		public:
			multinomial_distribution_param_type(void) : x::random::multinomial_distribution<long>::param_type() {}
			multinomial_distribution_param_type(long p1, std::vector<double> p2) : x::random::multinomial_distribution<long>::param_type(p1, p2.begin(), p2.end()) {}
			long param1(void){return n();}
			std::vector<double> param2(void){return p();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class hypergeometric_distribution_param_type : public x::random::hypergeometric_distribution<long>::param_type
		{
		public:
			hypergeometric_distribution_param_type(void) : x::random::hypergeometric_distribution<long>::param_type() {}
			hypergeometric_distribution_param_type(long p1, long p2, long p3) : x::random::hypergeometric_distribution<long>::param_type(p1, p2, p3) {}
			long param1(void){return n();}
			long param2(void){return M();}
			long param3(void){return N();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2()), &typeid(param3())};
		};

		class multivariate_hypergeometric_distribution_param_type : public x::random::multivariate_hypergeometric_distribution<long>::param_type
		{
		public:
			multivariate_hypergeometric_distribution_param_type(void) : x::random::multivariate_hypergeometric_distribution<long>::param_type() {}
			multivariate_hypergeometric_distribution_param_type(long p1, std::vector<long> p2) : x::random::multivariate_hypergeometric_distribution<long>::param_type(p1, p2.begin(), p2.end()) {}
			long param1(void){return n();}
			std::vector<long> param2(void){return M();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};
		
		class poisson_distribution_param_type : public poisson_distribution<long>::param_type
		{
		public:
			poisson_distribution_param_type(void) : poisson_distribution<long>::param_type() {}
			poisson_distribution_param_type(double p1) : poisson_distribution<long>::param_type(p1) {}
			double param1(void){return mean();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};

		class exponential_distribution_param_type : public exponential_distribution<double>::param_type
		{
		public:
			exponential_distribution_param_type(void) : exponential_distribution<double>::param_type() {}
			exponential_distribution_param_type(double p1) : exponential_distribution<double>::param_type(p1) {}
			double param1(void){return lambda();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};

		class gamma_distribution_param_type : public gamma_distribution<double>::param_type
		{
		public:
			gamma_distribution_param_type(void) : gamma_distribution<double>::param_type() {}
			gamma_distribution_param_type(double p1, double p2) : gamma_distribution<double>::param_type(p1, p2) {}
			double param1(void){return alpha();}
			double param2(void){return beta();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class weibull_distribution_param_type : public weibull_distribution<double>::param_type
		{
		public:
			weibull_distribution_param_type(void) : weibull_distribution<double>::param_type() {}
			weibull_distribution_param_type(double p1, double p2) : weibull_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class extreme_value_distribution_param_type : public extreme_value_distribution<double>::param_type
		{
		public:
			extreme_value_distribution_param_type(void) : extreme_value_distribution<double>::param_type() {}
			extreme_value_distribution_param_type(double p1, double p2) : extreme_value_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class dirichlet_distribution_param_type : public x::random::dirichlet_distribution<double>::param_type
		{
		public:
			dirichlet_distribution_param_type(void) : x::random::dirichlet_distribution<double>::param_type() {}
			template <class _InputIterator>
			dirichlet_distribution_param_type(_InputIterator __f, _InputIterator __l) : x::random::dirichlet_distribution<double>::param_type(__f, __l) {}
			dirichlet_distribution_param_type(std::vector<double> v) : dirichlet_distribution_param_type(v.begin(), v.end()) {}
			std::vector<double> param1(void){return alpha();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};

		class beta_distribution_param_type : public x::random::beta_distribution<double>::param_type
		{
		public:
			beta_distribution_param_type(void) : x::random::beta_distribution<double>::param_type() {}
			beta_distribution_param_type(double p1, double p2) : x::random::beta_distribution<double>::param_type(p1, p2) {}
			double param1(void){return alpha();}
			double param2(void){return beta();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class kumaraswamy_distribution_param_type : public x::random::kumaraswamy_distribution<double>::param_type
		{
		public:
			kumaraswamy_distribution_param_type(void) : x::random::kumaraswamy_distribution<double>::param_type() {}
			kumaraswamy_distribution_param_type(double p1, double p2) : x::random::kumaraswamy_distribution<double>::param_type(p1, p2) {}
			double param1(void){return alpha();}
			double param2(void){return beta();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class laplace_distribution_param_type : public x::random::laplace_distribution<double>::param_type
		{
		public:
			laplace_distribution_param_type(void) : x::random::laplace_distribution<double>::param_type() {}
			laplace_distribution_param_type(double p1, double p2) : x::random::laplace_distribution<double>::param_type(p1, p2) {}
			double param1(void){return mu();}
			double param2(void){return sigma();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class erlang_distribution_param_type : public x::random::erlang_distribution<double>::param_type
		{
		public:
			erlang_distribution_param_type(void) : x::random::erlang_distribution<double>::param_type() {}
			erlang_distribution_param_type(double p1, double p2) : x::random::erlang_distribution<double>::param_type(p1, p2) {}
			long param1(void){return k();}
			double param2(void){return lambda();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class gaussian_tail_distribution_param_type : public x::random::gaussian_tail_distribution<double>::param_type
		{
		public:
			gaussian_tail_distribution_param_type(void) : x::random::gaussian_tail_distribution<double>::param_type() {}
			gaussian_tail_distribution_param_type(double p1, double p2) : x::random::gaussian_tail_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return sigma();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class bivariate_normal_distribution_param_type : public x::random::bivariate_normal_distribution<double>::param_type
		{
		public:
			bivariate_normal_distribution_param_type(void) : x::random::bivariate_normal_distribution<double>::param_type() {}
			bivariate_normal_distribution_param_type(double p1, double p2, double p3) : x::random::bivariate_normal_distribution<double>::param_type(p1, p2, p3) {}
			double param1(void){return sigmax();}
			double param2(void){return sigmay();}
			double param3(void){return rho();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2()), &typeid(param3())};
		};

		class exponential_power_distribution_param_type : public x::random::exponential_power_distribution<double>::param_type
		{
		public:
			exponential_power_distribution_param_type(void) : x::random::exponential_power_distribution<double>::param_type() {}
			exponential_power_distribution_param_type(double p1, double p2) : x::random::exponential_power_distribution<double>::param_type(p1, p2) {}
			double param1(void){return alpha();}
			double param2(void){return beta();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class pareto_distribution_param_type : public x::random::pareto_distribution<double>::param_type
		{
		public:
			pareto_distribution_param_type(void) : x::random::pareto_distribution<double>::param_type() {}
			pareto_distribution_param_type(double p1, double p2) : x::random::pareto_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class rayleigh_tail_distribution_param_type : public x::random::rayleigh_tail_distribution<double>::param_type
		{
		public:
			rayleigh_tail_distribution_param_type(void) : x::random::rayleigh_tail_distribution<double>::param_type() {}
			rayleigh_tail_distribution_param_type(double p1, double p2) : x::random::rayleigh_tail_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return sigma();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class logistic_distribution_param_type : public x::random::logistic_distribution<double>::param_type
		{
		public:
			logistic_distribution_param_type(void) : x::random::logistic_distribution<double>::param_type() {}
			logistic_distribution_param_type(double p1, double p2) : x::random::logistic_distribution<double>::param_type(p1, p2) {}
			double param1(void){return mu();}
			double param2(void){return s();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class gumbel_distribution_param_type : public x::random::gumbel_distribution<double>::param_type
		{
		public:
			gumbel_distribution_param_type(void) : x::random::gumbel_distribution<double>::param_type() {}
			gumbel_distribution_param_type(double p1, double p2) : x::random::gumbel_distribution<double>::param_type(p1, p2) {}
			double param1(void){return mu();}
			double param2(void){return beta();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class logarithmic_series_distribution_param_type : public logarithmic_series_distribution<long>::param_type
		{
		public:
			logarithmic_series_distribution_param_type(void) : logarithmic_series_distribution<long>::param_type() {}
			logarithmic_series_distribution_param_type(double p1) : logarithmic_series_distribution<long>::param_type(p1) {}
			double param1(void){return p();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};

		class normal_distribution_param_type : public normal_distribution<double>::param_type
		{
		public:
			normal_distribution_param_type(void) : normal_distribution<double>::param_type() {}
			normal_distribution_param_type(double p1, double p2) : normal_distribution<double>::param_type(p1, p2) {}
			double param1(void){return mean();}
			double param2(void){return stddev();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class lognormal_distribution_param_type : public lognormal_distribution<double>::param_type
		{
		public:
			lognormal_distribution_param_type(void) : lognormal_distribution<double>::param_type() {}
			lognormal_distribution_param_type(double p1, double p2) : lognormal_distribution<double>::param_type(p1, p2) {}
			double param1(void){return m();}
			double param2(void){return s();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class chi_squared_distribution_param_type : public chi_squared_distribution<double>::param_type
		{
		public:
			chi_squared_distribution_param_type(void) : chi_squared_distribution<double>::param_type() {}
			chi_squared_distribution_param_type(double p1) : chi_squared_distribution<double>::param_type(p1) {}
			double param1(void){return n();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};

		class cauchy_distribution_param_type : public cauchy_distribution<double>::param_type
		{
		public:
			cauchy_distribution_param_type(void) : cauchy_distribution<double>::param_type() {}
			cauchy_distribution_param_type(double p1, double p2) : cauchy_distribution<double>::param_type(p1, p2) {}
			double param1(void){return a();}
			double param2(void){return b();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class fisher_f_distribution_param_type : public fisher_f_distribution<double>::param_type
		{
		public:
			fisher_f_distribution_param_type(void) : fisher_f_distribution<double>::param_type() {}
			fisher_f_distribution_param_type(double p1, double p2) : fisher_f_distribution<double>::param_type(p1, p2) {}
			double param1(void){return m();}
			double param2(void){return n();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class student_t_distribution_param_type : public student_t_distribution<double>::param_type
		{
		public:
			student_t_distribution_param_type(void) : student_t_distribution<double>::param_type() {}
			student_t_distribution_param_type(double p1) : student_t_distribution<double>::param_type(p1) {}
			double param1(void){return n();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};

		class rayleigh_distribution_param_type : public rayleigh_distribution<double>::param_type
		{
		public:
			rayleigh_distribution_param_type(void) : rayleigh_distribution<double>::param_type() {}
			rayleigh_distribution_param_type(double p1) : rayleigh_distribution<double>::param_type(p1) {}
			double param1(void){return sigma();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};
		
		class discrete_distribution_param_type : public discrete_distribution<long>::param_type
		{
		public:
			discrete_distribution_param_type(void) : discrete_distribution<long>::param_type() {}
			template <class _InputIterator>
			discrete_distribution_param_type(_InputIterator __f, _InputIterator __l) : discrete_distribution<long>::param_type(__f, __l) {}
			discrete_distribution_param_type(std::vector<double> v) : discrete_distribution_param_type(v.begin(), v.end()) {}
			std::vector<double> param1(void){return probabilities();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1())};
		};

		class piecewise_constant_distribution_param_type : public piecewise_constant_distribution<double>::param_type
		{
		public:
			piecewise_constant_distribution_param_type(void) : piecewise_constant_distribution<double>::param_type() {}
			template <class _InputIteratorB, class _InputIteratorW>
			piecewise_constant_distribution_param_type(_InputIteratorB __fB, _InputIteratorB __lB, _InputIteratorW __fW) : piecewise_constant_distribution<double>::param_type(__fB, __lB, __fW) {}
			piecewise_constant_distribution_param_type(std::vector<double> v1, std::vector<double> v2) : piecewise_constant_distribution_param_type(v1.begin(), v1.end(), v2.begin()) {}
			std::vector<double> param1(void){return intervals();}
			std::vector<double> param2(void){return densities();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};

		class piecewise_linear_distribution_param_type : public piecewise_linear_distribution<double>::param_type
		{
		public:
			piecewise_linear_distribution_param_type(void) : piecewise_linear_distribution<double>::param_type() {}
			template <class _InputIteratorB, class _InputIteratorW>
			piecewise_linear_distribution_param_type(_InputIteratorB __fB, _InputIteratorB __lB, _InputIteratorW __fW) : piecewise_linear_distribution<double>::param_type(__fB, __lB, __fW) {}
			piecewise_linear_distribution_param_type(std::vector<double> v1, std::vector<double> v2) : piecewise_linear_distribution_param_type(v1.begin(), v1.end(), v2.begin()) {}
			std::vector<double> param1(void){return intervals();}
			std::vector<double> param2(void){return densities();}
            std::vector<const std::type_info*> paramtypes = {&typeid(param1()), &typeid(param2())};
		};
	}
}
