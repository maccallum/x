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
	 (* (struct xobj_uint32)))
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
	 (* (struct xobj_uint32)))
#+sbcl (define-alien-routine rng-pcg32-generate
	   unsigned-long
	 (obj (* (struct xobj_uint32))))
#+sbcl (define-alien-routine rng-pcg32-min
	   unsigned-long)
#+sbcl (define-alien-routine rng-pcg32-max
	   unsigned-long)

;;
;; distributions
;;
(declaim (inline dist-gamma-generate))

#+sbcl (define-alien-routine ("dist_gamma_generate" alien-dist-gamma-generate)
	   double
	 (rng (* (struct xobj_uint32)))
	 (rng-min unsigned-long)
	 (rng-max unsigned-long)
	 (alpha double)
	 (beta double))

(defun dist-gamma-generate (rng rng-min rng-max alpha beta)
  (alien-dist-gamma-generate rng rng-min rng-max (coerce alpha 'double-float) (coerce beta 'double-float)))

(defun test ()
  (let ((rng (rng-pcg32-new (seed-seq-from-delegate-new (seed-seq-from-new (random-device-new))))))
    (print (rng-pcg32-generate rng))
    (dist-gamma-generate rng
				(rng-pcg32-min)
				(rng-pcg32-max)
				2.0
				2.0)))
    
