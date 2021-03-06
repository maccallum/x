;;(cl:defpackage "TEST-C-CALL" (:use "CL" "SB-ALIEN" "SB-C-CALL"))
;;(cl:in-package "TEST-C-CALL")

(load-shared-object "lib/libx.dylib")

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

(defvar *defrng* 
  (with-alien ((rd (* (struct xobj_uint32)) (random-device-new)))
    (with-alien ((ssf (* (struct xobj_uint32)) (seed-seq-from-new rd)))
      (with-alien ((ssfd (* (struct xobj_uint32)) (seed-seq-from-delegate-new ssf)))
	(with-alien ((rng (* (struct xobj_uint32)) (rng-pcg32-new ssfd)))
	  (seed-seq-from-delegate-delete ssfd)
	  (seed-seq-from-delete ssf)
	  (random-device-delete rd)
	  rng)))))

;;
;; distributions
;;
(declaim (inline alien-dist-uniform-int-generate))
#+sbcl (define-alien-routine ("dist_uniform_int_generate" alien-dist-uniform-int-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (a long)
	 (b long))
(defun dist-uniform-int-generate (a b &optional (rng *defrng*))
  (alien-dist-uniform-int-generate rng a b))

(declaim (inline alien-dist-uniform-real-generate))
#+sbcl (define-alien-routine ("dist_uniform_real_generate" alien-dist-uniform-real-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (a double)
	 (b double))
(defun dist-uniform-real-generate (a b &optional (rng *defrng*))
  (alien-dist-uniform-real-generate rng (coerce a 'double-float) (coerce b 'double-float)))



(declaim (inline alien-dist-bernoulli-generate))
#+sbcl (define-alien-routine ("dist_bernoulli_generate" alien-dist-bernoulli-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (p double))
(defun dist-bernoulli-generate (p &optional (rng *defrng*))
  (alien-dist-bernoulli-generate rng (coerce p 'double-float)))

(declaim (inline alien-dist-binomial-generate))
#+sbcl (define-alien-routine ("dist_binomial_generate" alien-dist-binomial-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (n long)
	 (p double))
(defun dist-binomial-generate (n p &optional (rng *defrng*))
  (alien-dist-binomial-generate rng n (coerce p 'double-float)))

(declaim (inline alien-dist-geometric-generate))
#+sbcl (define-alien-routine ("dist_geometric_generate" alien-dist-geometric-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (p double))
(defun dist-geometric-generate (p &optional (rng *defrng*))
  (alien-dist-geometric-generate rng (coerce p 'double-float)))

(declaim (inline alien-dist-negative-binomial-generate))
#+sbcl (define-alien-routine ("dist_negative_binomial_generate" alien-dist-negative-binomial-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (n long)
	 (p double))
(defun dist-negative-binomial-generate (n p &optional (rng *defrng*))
  (alien-dist-negative-binomial-generate rng n (coerce p 'double-float)))

(declaim (inline alien-dist-multinomial-generate))
#+sbcl (define-alien-routine ("dist_multinomial_generate" alien-dist-multinomial-generate)
	   (* long)
	 (rng (* (struct xobj_uint32)))
	 (n long)
	 (buflen long)
	 (p (* double))
	 (result (* long)))
(defun dist-multinomial-generate (n p &optional (rng *defrng*))
  (let ((ar (make-alien double (length p)))
	(out (make-alien long (length p))))
    (dotimes (i (length p))
      (setf (deref ar i) (coerce (nth i p) 'double-float)))
    (alien-dist-multinomial-generate rng n (length p) ar out)
    (let ((ret (make-list (length p))))
      (dotimes (i (length p))
	(setf (nth i ret) (deref out i)))
      (free-alien ar)
      (free-alien out)
      ret)))

(declaim (inline alien-dist-hypergeometric-generate))
#+sbcl (define-alien-routine ("dist_hypergeometric_generate" alien-dist-hypergeometric-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (ntrials long)
	 (M long)
	 (N long))
(defun dist-hypergeometric-generate (ntrials M N &optional (rng *defrng*))
  (alien-dist-hypergeometric-generate rng ntrials M N))

(declaim (inline alien-dist-multivariate-hypergeometric-generate))
#+sbcl (define-alien-routine ("dist_multivariate_hypergeometric_generate" alien-dist-multivariate-hypergeometric-generate)
	   (* long)
	 (rng (* (struct xobj_uint32)))
	 (n long)
	 (buflen long)
	 (M (* long))
	 (result (* long)))
(defun dist-multivariate-hypergeometric-generate (n M &optional (rng *defrng*))
  (let ((ar (make-alien long (length M)))
	(out (make-alien long (length M))))
    (dotimes (i (length M))
      (setf (deref ar i) (nth i M)))
    (alien-dist-multivariate-hypergeometric-generate rng n (length M) ar out)
    (let ((ret (make-list (length M))))
      (dotimes (i (length M))
	(setf (nth i ret) (deref out i)))
      (free-alien ar)
      (free-alien out)
      ret)))

(declaim (inline alien-dist-poisson-generate))
#+sbcl (define-alien-routine ("dist_poisson_generate" alien-dist-poisson-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (mean double))
(defun dist-poisson-generate (mean &optional (rng *defrng*))
  (alien-dist-poisson-generate rng (coerce mean 'double-float)))

(declaim (inline alien-dist-exponential-generate))
#+sbcl (define-alien-routine ("dist_exponential_generate" alien-dist-exponential-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (lambda double))
(defun dist-exponential-generate (lambda &optional (rng *defrng*))
  (alien-dist-exponential-generate rng (coerce lambda 'double-float)))

(declaim (inline alien-dist-gamma-generate))
#+sbcl (define-alien-routine ("dist_gamma_generate" alien-dist-gamma-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-gamma-generate (alpha beta &optional (rng *defrng*))
  (alien-dist-gamma-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-weibull-generate))
#+sbcl (define-alien-routine ("dist_weibull_generate" alien-dist-weibull-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-weibull-generate (alpha beta &optional (rng *defrng*))
  (alien-dist-weibull-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-extreme-value-generate))
#+sbcl (define-alien-routine ("dist_extreme_value_generate" alien-dist-extreme-value-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-extreme-value-generate (alpha beta &optional (rng *defrng*))
  (alien-dist-extreme-value-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-dirichlet-generate))
#+sbcl (define-alien-routine ("dist_dirichlet_generate" alien-dist-dirichlet-generate)
	   (* double)
	 (rng (* (struct xobj_uint32)))
	 (buflen long)
	 (alpha (* double))
	 (result (* double)))
(defun dist-dirichlet-generate (alpha &optional (rng *defrng*))
  (let ((ar (make-alien double (length alpha)))
	(out (make-alien double (length alpha))))
    (dotimes (i (length alpha))
      (setf (deref ar i) (coerce (nth i alpha) 'double-float)))
    (alien-dist-dirichlet-generate rng (length alpha) ar out)
    (let ((ret (make-list (length alpha))))
      (dotimes (i (length alpha))
	(setf (nth i ret) (deref out i)))
      (free-alien ar)
      (free-alien out)
      ret)))

(declaim (inline alien-dist-beta-generate))
#+sbcl (define-alien-routine ("dist_beta_generate" alien-dist-beta-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-beta-generate (alpha beta &optional (rng *defrng*))
  (alien-dist-beta-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-kumaraswamy-generate))
#+sbcl (define-alien-routine ("dist_kumaraswamy_generate" alien-dist-kumaraswamy-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-kumaraswamy-generate (alpha beta &optional (rng *defrng*))
  (alien-dist-kumaraswamy-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

(declaim (inline alien-dist-laplace-generate))
#+sbcl (define-alien-routine ("dist_laplace_generate" alien-dist-laplace-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (mu double)
	 (sigma double))
(defun dist-laplace-generate (mu sigma &optional (rng *defrng*))
  (alien-dist-laplace-generate rng (coerce mu 'double-float) (coerce sigma 'double-float)))

(declaim (inline alien-dist-erlang-generate))
#+sbcl (define-alien-routine ("dist_erlang_generate" alien-dist-erlang-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (k double)
	 (lambda double))
(defun dist-erlang-generate (k lambda &optional (rng *defrng*))
  (alien-dist-erlang-generate rng (coerce k 'double-float) (coerce lambda 'double-float)))


(declaim (inline alien-dist-normal-generate))
#+sbcl (define-alien-routine ("dist_normal_generate" alien-dist-normal-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (mean double)
	 (stddev double))
(defun dist-normal-generate (mean stddev &optional (rng *defrng*))
  (alien-dist-normal-generate rng (coerce mean 'double-float) (coerce stddev 'double-float)))

(declaim (inline alien-dist-lognormal-generate))
#+sbcl (define-alien-routine ("dist_lognormal_generate" alien-dist-lognormal-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (m double)
	 (s double))
(defun dist-lognormal-generate (m s &optional (rng *defrng*))
  (alien-dist-lognormal-generate rng (coerce m 'double-float) (coerce s 'double-float)))

(declaim (inline alien-dist-chi-squared-generate))
#+sbcl (define-alien-routine ("dist_chi_squared_generate" alien-dist-chi-squared-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (n double))
(defun dist-chi-squared-generate (n &optional (rng *defrng*))
  (alien-dist-chi-squared-generate rng (coerce n 'double-float)))

(declaim (inline alien-dist-cauchy-generate))
#+sbcl (define-alien-routine ("dist_cauchy_generate" alien-dist-cauchy-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (a double)
	 (b double))
(defun dist-cauchy-generate (a b &optional (rng *defrng*))
  (alien-dist-cauchy-generate rng (coerce a 'double-float) (coerce b 'double-float)))

(declaim (inline alien-dist-fisher-f-generate))
#+sbcl (define-alien-routine ("dist_fisher_f_generate" alien-dist-fisher-f-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (m double)
	 (n double))
(defun dist-fisher-f-generate (m n &optional (rng *defrng*))
  (alien-dist-fisher-f-generate rng (coerce m 'double-float) (coerce n 'double-float)))

(declaim (inline alien-dist-student-t-generate))
#+sbcl (define-alien-routine ("dist_student_t_generate" alien-dist-student-t-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (n double))
(defun dist-student-t-generate (n &optional (rng *defrng*))
  (alien-dist-student-t-generate rng (coerce n 'double-float)))

(declaim (inline alien-dist-rayleigh-generate))
#+sbcl (define-alien-routine ("dist_rayleigh_generate" alien-dist-rayleigh-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (sigma double))
(defun dist-rayleigh-generate (sigma &optional (rng *defrng*))
  (alien-dist-rayleigh-generate rng (coerce sigma 'double-float)))

(declaim (inline alien-dist-discrete-generate))
#+sbcl (define-alien-routine ("dist_discrete_generate" alien-dist-discrete-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (buflen long)
	 (probabilities (* double)))
(defun dist-discrete-generate (probabilities &optional (rng *defrng*))
  (let ((ar (make-alien double (length probabilities))))
    (dotimes (i (length probabilities))
      (setf (deref ar i) (coerce (nth i probabilities) 'double-float)))
    
    (let ((ret (alien-dist-discrete-generate rng (length probabilities) ar)))
      (free-alien ar)
      ret)))

(declaim (inline alien-dist-piecewise-constant-generate))
#+sbcl (define-alien-routine ("dist_piecewise_constant_generate" alien-dist-piecewise-constant-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (intervals-len long)
	 (intervals (* double))
	 (densities (* double)))
(defun dist-piecewise-constant-generate (intervals densities &optional (rng *defrng*))
  (let ((intervals-ar (make-alien double (length intervals)))
	(densities-ar (make-alien double (length densities))))
    (setf (deref intervals-ar 0) (coerce (nth 0 intervals) 'double-float))
    (dotimes (i (- (length intervals) 1))
      (setf (deref intervals-ar (+ 1 i)) (coerce (nth (+ 1 i) intervals) 'double-float))
      (setf (deref densities-ar i) (coerce (nth i densities) 'double-float)))
    (let ((ret (alien-dist-piecewise-constant-generate rng (length intervals) intervals-ar densities-ar)))
      (free-alien intervals-ar)
      (free-alien densities-ar)
      ret)))

(declaim (inline alien-dist-piecewise-linear-generate))
#+sbcl (define-alien-routine ("dist_piecewise_linear_generate" alien-dist-piecewise-linear-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (intervals-len long)
	 (intervals (* double))
	 (densities (* double)))
(defun dist-piecewise-linear-generate (intervals densities &optional (rng *defrng*))
  (let ((intervals-ar (make-alien double (length intervals)))
	(densities-ar (make-alien double (length densities))))
    (setf (deref intervals-ar 0) (coerce (nth 0 intervals) 'double-float))
    (dotimes (i (- (length intervals) 1))
      (setf (deref intervals-ar (+ 1 i)) (coerce (nth (+ 1 i) intervals) 'double-float))
      (setf (deref densities-ar i) (coerce (nth i densities) 'double-float)))
    (let ((ret (alien-dist-piecewise-linear-generate rng (length intervals) intervals-ar densities-ar)))
      (free-alien intervals-ar)
      (free-alien densities-ar)
      ret)))
