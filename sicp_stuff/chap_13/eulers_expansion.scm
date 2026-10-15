#|
Exercise 1.38: In 1737, the Swiss mathematician Leonhard
Euler published a memoir De Fractionibus Continuis, which
included a continued fraction expansion for e − 2, where
e is the base of the natural logarithms.

In this fraction, the Ni are all 1,
and the Di are successively 1, 2, 1, 1, 4, 1, 1, 6, 1, 1, 8, ...

Write a program that uses your cont-frac procedure
from Exercise 1.37 to approximate e, based on Euler’s expansion.
|#

(define (cont-frac n d k)
  (if (< k 1)
	  0
	  (/ (n k)
		 (+ (d k) (cont-frac n d (- k 1))))))

(define (euler k)
  (cont-frac
	(lambda (k) 1.0)
	(lambda (k)
	  (cond ((< k 3) k)
			((= (modulo k 3) 0) 1)
			((= (modulo (- k 1) 3) 0) 1)
			(else (- k (quotient (- k 2) 3)))))
	k))

(display (euler 100))
(newline)

(define (thing k) 
  (cond ((< k 3) k)
		((= (modulo k 3) 0) 1)
		((= (modulo (- k 1) 3) 0) 1)
		(else (- k (quotient (- k 2) 3)))))

(define (repeat f n)
  (define (iter i)
	(display (f i))
	(newline)
	(if (< i n) (iter (+ i 1))))
  (iter 1))

