#|
Exercise 1.39: A continued fraction representation of the
tangent function was published in 1770 by the German mathematician J.H. Lambert:

tan x = x / (1 − (x^2 / (3 − x^2 (5 − ... ))))

where x is in radians. Define a procedure (tan-cf x k) that
computes an approximation to the tangent function based
on Lambert’s formula. k specifies the number of terms to
compute, as in Exercise 1.37.
|#

(define (cont-frac n d k)
  (define (recur i)
	  (if (> i k)
		  0.0
		  (/ (n i)
			 (- (d i) (recur (+ i 1))))))
  (recur 1))

(define (square n) (* n n))

(define (tan-cf x k)
  (cont-frac
	(lambda (k) 
	  (if (= k 1)
		    x
			(square x)))
	(lambda (k)
	  (- (* k 2) 1))
	k))

#|
from python

tan(1)		1.5574077246549023
tan(0.1)	0.10033467208545055
tan(3)		-0.1425465430742778
tan(3.14)	-0.001592654936407223
tan(0.5)	0.5463024898437905
|#

(display (tan-cf 0.5 10))
(newline)
