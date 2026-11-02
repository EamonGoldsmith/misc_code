#|
Exercise 1.45: We saw in Section 1.3.3 that attempting to
compute square roots by naively finding a fixed point of
y → x/y does not converge, and that this can be fixed by
average damping. The same method works for finding cube
roots as fixed points of the average-damped y → x/y^2.

Unfortunately, the process does not work for fourth roots
a single average damp is not enough to make a fixed-point
search for y → x/y^3 converge. On the other hand, if we
average damp twice
(i.e., use the average damp of the average damp of y → x/y^3)
the fixed-point search does converge.

Do some experiments to determine how many average damps
are required to compute n-th roots as a fixedpoint search
based upon repeated average damping of y → x/y^(n−1).

Use this to implement a simple procedure for computing n-th
roots using fixed-point, average-damp, and the
repeated procedure of Exercise 1.43. Assume that any arithmetic 
operations you need are available as primitives.
|#

;; for the smoothing function

(define (compose f g)
  (lambda (x) (f (g x))))

(define (repeated f n)
  (if (= n 1)
	f
	(repeated (compose f f) (- n 1))))

(define (average a b)
  (/ (+ a b) 2))

(define (average-damp f)
  (lambda (x) (average x (f x))))

(define (smooth f n)
  ((repeated average-damp n) f))

;; fixed point finding function

(define tolerance 0.00001)

(define (fixed-point f first-guess)
  (define (close-enough? v1 v2)
	(< (abs (- v1 v2))
	   tolerance))
  (define (try guess)
	(let ((next (f guess)))
	  (if (close-enough? guess next)
		  next
	      (try next))))
  (try first-guess))

;; n-th root function

(define (n-th-root n x)
  (let ((damping n))		; damping is equal to n, for n-th root.
	   (fixed-point
			(smooth
				(lambda (y) (/ x (expt y (- n 1))))
				damping)
		1.0)))

;; test fixed point convergence

(display (n-th-root 2 49))
(newline)
(display (n-th-root 3 81))
(newline)

;; test non-converging roots

(display (n-th-root 4 4294967296))
(newline)
(display (n-th-root 5 36))
(newline)

