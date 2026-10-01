
;; Ingore this
(define (runtime)
  (exact->inexact (/ (get-internal-run-time) 
					 internal-time-units-per-second)))

(define true #t)
(define false #f)

(define (square n) (* n n))

#|
Exercise 1.27: Demonstrate that the Carmichael numbers
listed in Footnote 1.47 really do fool the Fermat test. that is,
write a procedure that takes an integer n and tests whether a^n
is congruent to a modulo n for every a < n, and try your
procedure on the given Carmichael numbers.
|#

(define (expmod base exp m)
  (cond ((= exp 0) 1)
		((even? exp)
		  (remainder
			(square (expmod base (/ exp 2) m))
			m))
		(else
		  (remainder
			(* base (expmod base (- exp 1) m))
			m))))

(define (fermat-test n)
  (define (try-it a n)
	(cond ((= a 1) true)
		  ((= (expmod a n n) a) 
		      (try-it (- a 1) n))
		  (else false)))
  (try-it (- n 1) n))

(display (fermat-test 29))
(newline)

#|
There are 255 Carmichael numbers below 100,000,000. The smallest few are
561, 1105, 1729, 2465, 2821, and 6601.
|#

(display (fermat-test 561))
(newline)

(display (fermat-test 1105))
(newline)

(display (fermat-test 1729))
(newline)

(display (fermat-test 2465))
(newline)

(display (fermat-test 2821))
(newline)

(display (fermat-test 6601))
(newline)
