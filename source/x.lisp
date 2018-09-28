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

(declaim (inline dist-uniform-real-generate))
#+sbcl (define-alien-routine ("dist_uniform_real_generate" alien-dist-uniform-real-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (a double)
	 (b double))
(defun dist-uniform-real-generate (rng a b)
  (alien-dist-uniform-real-generate rng (coerce a 'double-float) (coerce b 'double-float)))



(declaim (inline dist-bernoulli-generate))
#+sbcl (define-alien-routine ("dist_bernoulli_generate" alien-dist-bernoulli-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (p double))
(defun dist-bernoulli-generate (rng p)
  (alien-dist-bernoulli-generate rng (coerce p 'double-float)))

(declaim (inline dist-binomial-generate))
#+sbcl (define-alien-routine ("dist_binomial_generate" alien-dist-binomial-generate)
	   long
	 (rng (* (struct xobj_uint32)))
	 (n long)
	 (p double))
(defun dist-binomial-generate (rng n p)
  (alien-dist-binomial-generate rng n (coerce p 'double-float)))


(declaim (inline dist-gamma-generate))
#+sbcl (define-alien-routine ("dist_gamma_generate" alien-dist-gamma-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (alpha double)
	 (beta double))
(defun dist-gamma-generate (rng alpha beta)
  (alien-dist-gamma-generate rng (coerce alpha 'double-float) (coerce beta 'double-float)))

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
    
