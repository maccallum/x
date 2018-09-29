;;(cl:defpackage "TEST-C-CALL" (:use "CL" "SB-ALIEN" "SB-C-CALL"))
;;(cl:in-package "TEST-C-CALL")

(load-shared-object "libx.dylib")

(declaim (inline random-device-new))
(declaim (inline random-device-delete))
(declaim (inline random-device-generate))
(declaim (inline random-device-min))
(declaim (inline random-device-max))

#+sbcl (define-alien-routine random-device-new
	   (* (struct xobj_uint32)))
#+sbcl (define-alien-routine random-device-delete
	   void
	 (obj (* (struct xobj_uint32))))
#+sbcl (define-alien-routine random-device-generate
	   unsigned-long
	 (obj (* (struct xobj_uint32))))
#+sbcl (define-alien-routine random-device-min
	   unsigned-long
	 (obj (* (struct xobj_uint32))))
#+sbcl (define-alien-routine random-device-max
	   unsigned-long
	 (obj (* (struct xobj_uint32))))

;;
;; seed_seq_from
;;
(declaim (inline seed-seq-from-new))
(declaim (inline seed-seq-from-delete))

#+sbcl (define-alien-routine seed-seq-from-new
	   (* (struct xobj_uint32))
	 (random_device (* (struct xobj_uint32))))
#+sbcl (define-alien-routine seed-seq-from-delete
	   void
	 (obj (* (struct xobj_uint32))))

;;
;; seed_seq_from_delegate
;;
(declaim (inline seed-seq-from-delegate-new))
(declaim (inline seed-seq-from-delegate-delete))

#+sbcl (define-alien-routine seed-seq-from-delegate-new
	   (* (struct xobj_uint32))
	 (seed_seq_from (* (struct xobj_uint32))))
#+sbcl (define-alien-routine seed-seq-from-delegate-delete
	   void
	 (obj (* (struct xobj_uint32))))

;;
;; rngs
;;
(declaim (inline rng-pcg32-new))
(declaim (inline rng-pcg32-delete))
(declaim (inline rng-pcg32-generate))
(declaim (inline rng-pcg32-min))
(declaim (inline rng-pcg32-max))

#+sbcl (define-alien-routine rng-pcg32-new
	   (* (struct xobj_uint32))
	 (seed-seq-from-delegate (* (struct xobj_uint32))))
#+sbcl (define-alien-routine rng-pcg32-delete
	   void
	 (obj (* (struct xobj_uint32))))
#+sbcl (define-alien-routine rng-pcg32-generate
	   unsigned-long
	 (obj (* (struct xobj_uint32))))
#+sbcl (define-alien-routine rng-pcg32-min
	   unsigned-long)
#+sbcl (define-alien-routine rng-pcg32-max
	   unsigned-long)
#+sbcl (define-alien-routine rng-min
	   unsigned-long-long)
#+sbcl (define-alien-routine rng-max
	   unsigned-long-long)

;;
;; distributions
;;
(declaim (inline dist-uniform-int-generate))
#+sbcl (define-alien-routine dist-uniform-int-generate
	   long
	 (rng (* (struct xobj_uint32)))
	 (a long)
	 (b long))

(declaim (inline alien-dist-uniform-real-generate))
#+sbcl (define-alien-routine ("dist_uniform_real_generate" alien-dist-uniform-real-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (a double)
	 (b double))
(defun dist-uniform-real-generate (rng a b)
  (alien-dist-uniform-real-generate rng (coerce a 'double-float) (coerce b 'double-float)))



(declaim (inline alien-dist-bernoulli-generate))
#+sbcl (define-alien-routine ("dist_bernoulli_generate" alien-dist-bernoulli-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (p double))
(defun dist-bernoulli-generate (rng p)
  (alien-dist-bernoulli-generate rng (coerce p 'double-float)))

(declaim (inline alien-dist-binomial-generate))
#+sbcl (define-alien-routine ("dist_binomial_generate" alien-dist-binomial-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (n long)
	 (p double))
(defun dist-binomial-generate (rng n p)
  (alien-dist-binomial-generate rng n (coerce p 'double-float)))

(declaim (inline alien-dist-geometric-generate))
#+sbcl (define-alien-routine ("dist_geometric_generate" alien-dist-geometric-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (p double))
(defun dist-geometric-generate (rng p)
  (alien-dist-geometric-generate rng (coerce p 'double-float)))

(declaim (inline alien-dist-negative-binomial-generate))
#+sbcl (define-alien-routine ("dist_negative_binomial_generate" alien-dist-negative-binomial-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (n long)
	 (p double))
(defun dist-negative-binomial-generate (rng n p)
  (alien-dist-negative-binomial-generate rng n (coerce p 'double-float)))

;; multinomial

(declaim (inline dist-hypergeometric-generate))
#+sbcl (define-alien-routine dist-hypergeometric-generate
	   long
	 (rng (* (struct xobj_uint32)))
	 (ntrials long)
	 (M long)
	 (N long))

;; multivariate hypergeometric


(declaim (inline alien-dist-poisson-generate))
#+sbcl (define-alien-routine ("dist_poisson_generate" alien-dist-poisson-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (mean double))
(defun dist-poisson-generate (rng mean)
  (alien-dist-poisson-generate rng (coerce mean 'double-float)))

(declaim (inline alien-dist-exponential-generate))
#+sbcl (define-alien-routine ("dist_exponential_generate" alien-dist-exponential-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (lambda double))
(defun dist-exponential-generate (rng lambda)
  (alien-dist-exponential-generate rng (coerce lambda 'double-float)))

(declaim (inline alien-dist-gamma-generate))
#+sbcl (define-alien-routine ("dist_gamma_generate" alien-dist-gamma-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-gamma-generate (rng alpha beta)
  (alien-dist-gamma-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-weibull-generate))
#+sbcl (define-alien-routine ("dist_weibull_generate" alien-dist-weibull-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-weibull-generate (rng alpha beta)
  (alien-dist-weibull-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-extreme-value-generate))
#+sbcl (define-alien-routine ("dist_extreme_value_generate" alien-dist-extreme-value-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-extreme-value-generate (rng alpha beta)
  (alien-dist-extreme-value-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

;; dirichlet

(declaim (inline alien-dist-beta-generate))
#+sbcl (define-alien-routine ("dist_beta_generate" alien-dist-beta-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-beta-generate (rng alpha beta)
  (alien-dist-beta-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-kumaraswamy-generate))
#+sbcl (define-alien-routine ("dist_kumaraswamy_generate" alien-dist-kumaraswamy-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-kumaraswamy-generate (rng alpha beta)
  (alien-dist-kumaraswamy-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-laplace-generate))
#+sbcl (define-alien-routine ("dist_laplace_generate" alien-dist-laplace-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (mu double)
	 (sigma double))
(defun dist-laplace-generate (rng mu sigma)
  (alien-dist-laplace-generate rng (coerce mu 'double-float) (coerce sigma 'double-float)))

(declaim (inline alien-dist-erlang-generate))
#+sbcl (define-alien-routine ("dist_erlang_generate" alien-dist-erlang-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (k double)
	 (lambda double))
(defun dist-erlang-generate (rng k lambda)
  (alien-dist-erlang-generate rng (coerce k 'double-float) (coerce lambda 'double-float)))


(declaim (inline alien-dist-normal-generate))
#+sbcl (define-alien-routine ("dist_normal_generate" alien-dist-normal-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (mean double)
	 (stddev double))
(defun dist-normal-generate (rng mean stddev)
  (alien-dist-normal-generate rng (coerce mean 'double-float) (coerce stddev 'double-float)))

(declaim (inline alien-dist-lognormal-generate))
#+sbcl (define-alien-routine ("dist_lognormal_generate" alien-dist-lognormal-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (m double)
	 (s double))
(defun dist-lognormal-generate (rng m s)
  (alien-dist-lognormal-generate rng (coerce m 'double-float) (coerce s 'double-float)))

(declaim (inline alien-dist-chi-squared-generate))
#+sbcl (define-alien-routine ("dist_chi_squared_generate" alien-dist-chi-squared-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (n double))
(defun dist-chi-squared-generate (rng n)
  (alien-dist-chi-squared-generate rng (coerce n 'double-float)))

(declaim (inline alien-dist-cauchy-generate))
#+sbcl (define-alien-routine ("dist_cauchy_generate" alien-dist-cauchy-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (a double)
	 (b double))
(defun dist-cauchy-generate (rng a b)
  (alien-dist-cauchy-generate rng (coerce a 'double-float) (coerce b 'double-float)))

(declaim (inline alien-dist-fisher-f-generate))
#+sbcl (define-alien-routine ("dist_fisher_f_generate" alien-dist-fisher-f-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (m double)
	 (n double))
(defun dist-fisher-f-generate (rng m n)
  (alien-dist-fisher-f-generate rng (coerce m 'double-float) (coerce n 'double-float)))

(declaim (inline alien-dist-student-t-generate))
#+sbcl (define-alien-routine ("dist_student_t_generate" alien-dist-student-t-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (n double))
(defun dist-student-t-generate (rng n)
  (alien-dist-student-t-generate rng (coerce n 'double-float)))

(declaim (inline alien-dist-rayleigh-generate))
#+sbcl (define-alien-routine ("dist_rayleigh_generate" alien-dist-rayleigh-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (sigma double))
(defun dist-rayleigh-generate (rng sigma)
  (alien-dist-rayleigh-generate rng (coerce sigma 'double-float)))

(defvar *defrng* 
  (with-alien ((rd (* (struct xobj_uint32)) (random-device-new)))
    (with-alien ((ssf (* (struct xobj_uint32)) (seed-seq-from-new rd)))
      (with-alien ((ssfd (* (struct xobj_uint32)) (seed-seq-from-delegate-new ssf)))
	(with-alien ((rng (* (struct xobj_uint32)) (rng-pcg32-new ssfd)))
	  (seed-seq-from-delegate-delete ssfd)
	  (seed-seq-from-delete ssf)
	  (random-device-delete rd)
	  rng)))))

(defun test ()
    (dist-uniform-int-generate *defrng*
				0
				100))
    
