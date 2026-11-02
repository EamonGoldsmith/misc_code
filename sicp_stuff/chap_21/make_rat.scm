#|
Exercise 2.1: Define a better version of make-rat that handles
both positive and negative arguments. make-rat should
normalize the sign so that if the rational number is positive,
both the numerator and denominator are positive, and if
the rational number is negative, only the numerator is negative.

previously defined as:

(define (make-rat n d)
  (let ((g (gcd n d)))
		(cons (/ n g) (/ d g))))
|#

(define (numer r) (car r))
(define (denom r) (cdr r))

(define (print-rat r)
  (display (numer r))
  (display "/")
  (display (denom r))
  (newline))

(define (gdc a b)
  (if (= b 0)
	a
	(gdc b (remainder a b))))

(define (make-rat n d)
  (define (norm-numer n d)
	(if (and (> n 0) (< d 0))
		(* n -1)
		n))
  (define (norm-denom d)
	(if (< d 0)
		(* d -1)
		d))
  (let ((g (gcd n d)))
	(cons (/ (norm-numer n d) g) (/ (norm-denom d) g))))

;; test construct
(print-rat (make-rat 14 -4))

