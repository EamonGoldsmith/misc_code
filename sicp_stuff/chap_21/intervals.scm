#|
Extended Exercise 2.1.4:
Alyssa P. Hacker's resistor calculator.
|#

(define (add-interval x y)
  (make-interval (+ (lower-bound x) (lower-bound y))
				 (+ (upper-bound x) (upper-bound y))))

(define (mul-interval x y)
  (let ((p1 (* (lower-bound x) (lower-bound y)))
		(p2 (* (lower-bound x) (upper-bound y)))
		(p3 (* (upper-bound x) (lower-bound y)))
		(p4 (* (upper-bound x) (upper-bound y))))
	(make-interval (min p1 p2 p3 p4)
				   (max p1 p2 p3 p4))))

(define (div-interval x y)
  (mul-interval
	x
	(make-interval (/ 1.0 (lower-bound y))
				   (/ 1.0 (upper-bound y)))))

#|
Exercise 2.7: Alyssa’s program is incomplete because she
has not specified the implementation of the interval abstraction. 
Here is a definition of the interval constructor:

(define (make-interval a b) (cons a b))

Define selectors upper-bound and lower-bound to complete
the implementation.
|#

(define (make-interval a b) (cons a b))
(define (lower-bound x) (car x))
(define (upper-bound x) (cdr x))

#|
Exercise 2.8: Using reasoning analogous to Alyssa’s, 
describe how the difference of two intervals may be computed. 
Define a corresponding subtraction procedure, called sub-interval.

The sum of two intervals is simply the sum of their upper and lower bounds,
so the difference could simply be the difference of the upper and lower bounds.
|#

(define (sub-interval x y)
  (make-interval (- (lower-bound x) (lower-bound y))
				 (- (upper-bound x) (upper-bound y))))

#|
Exercise 2.9: The width of an interval is half of the difference 
between its upper and lower bounds. The width is a measure of the 
uncertainty of the number specified by the interval. 

For some arithmetic operations the width of the
result of combining two intervals is a function only of the
widths of the argument intervals, whereas for others the
width of the combination is not a function of the widths of
the argument intervals.

Show that the width of the sum (or difference) of two intervals
is a function only of the widths of the intervals being added
(or subtracted). Give examples to show that this is not true 
for multiplication or division.

for intervals of width 1:
1,2 + 2,2 -> 3,4	the width is preserved.
1,2 - 2,2 -> -1,0	agian, width is preserved.

for intervals of width 3 and 5:
1,4 + 2,7 -> 3,11	width is 8, the sum of the widths of the two intervals
1,4 - 2,7 -> -1,-3	width is 2, the difference of the widths

for multiplication:
1,4 * 2,7 -> [2, 7, 8, 28] -> 2,28	width is separate from the components
division follows
|#

#|
Exercise 2.10: Ben Bitdiddle, an expert systems programmer, 
looks over Alyssa’s shoulder and comments that it is
not clear what it means to divide by an interval that spans
zero.

Modify Alyssa’s code to check for this condition and
to signal an error if it occurs.
|#

(define (div-interval-safe x y)
  (if (<= (lower-bound y) 0 (upper-bound y))
	  (error 'div-interval "cannot divide interval spanning zero" y)
	  (mul-interval
		x
		(make-interval (/ (lower-bound y))
					   (/ (upper-bound y))))))

#|
Exercise 2.11: In passing, Ben also cryptically comments:
“By testing the signs of the endpoints of the intervals, it is
possible to break mul-interval into nine cases, only one
of which requires more than two multiplications.” Rewrite
this procedure using Ben’s suggestion.

our original implementation uses 16 cases hidden in min and max.
we can check each interval for being entirely negative, entirely positive
or spanning zero. Giving 9 possible cases. 8 of which we can predetermine
what combinations result in lowest and highest values. In case 9 both
intervals span zero so we must check all for combinations of bounds.
|#

(define (mul-interval-fast x y)
  (let ((x1 (lower-bound x))
        (x2 (upper-bound x))
        (y1 (lower-bound y))
        (y2 (upper-bound y)))
    (cond ((> x1 0)
           (cond ((> y1 0) (make-interval (* x1 y1) (* x2 y2)))
                 ((< y2 0) (make-interval (* x2 y1) (* x1 y2)))
                 (else (make-interval (* x2 y1) (* x2 y2)))))
          ((< x2 0)
           (cond ((> y1 0) (make-interval (* x1 y2) (* x2 y1)))
                 ((< y2 0) (make-interval (* x2 y2) (* x1 y1)))
                 (else (make-interval (* x1 y2) (* x1 y1)))))
          (else
           (cond ((> y1 0) (make-interval (* x1 y2) (* x2 y2)))
                 ((< y2 0) (make-interval (* x2 y1) (* x1 y1)))
                 (else (make-interval (min (* x1 y2) (* x2 y1))
                                      (max (* x1 y1) (* x2 y2)))))))))

#|
Exercise 2.12: Define a constructor make-center-percent
that takes a center and a percentage tolerance and produces
the desired interval. You must also define a selector
percent that produces the percentage tolerance for a given
interval. The center selector is the same as the one shown
above.
|#

(define (make-center-width c w)
  (make-interval (- c w) (+ c w)))
(define (center i)
  (/ (+ (lower-bound i) (upper-bound i)) 2))
(define (width i)
  (/ (- (upper-bound i) (lower-bound i)) 2))

(define (make-center-percent c p)
  (let ((w (* (/ p 100) c)))
	   (make-center-width c w)))

(define (percent x)
  (* (/ (width x) (center x)) 100))

#|
Exercise 2.13: Show that under the assumption of small
percentage tolerances there is a simple formula for the 
approximate percentage tolerance of the product of two intervals
in terms of the tolerances of the factors. You may
simplify the problem by assuming that all numbers are positive.

the product of two intervals x and y with tolerances p1 and p2 is:
(x1 - x1p1),(x2 + x2p1) * (y1 - y1p2),(y2 + y2p2)
(x1 - x1p1) * (y1 - y1p2),(x2 + x2p1) * (y2 + y2p2)
x1y1 - x1y1p2 - x1y1p1 + x1y1p1p2 , x2y2 + x2y2p2 + x2y2p1 + x2y2p1p2

; given both p1 and p2 are very small we can consider their product to be zero
x1y1 - (p2 + p1)(x1y1) + 0 , x2y2 + (p1 + p2)(x2y2) - 0
x1y1 - (p1 + p2)(x1y1),x2y2 + (p1 + p2)(x2y2)

from the form we can see the tolerance of the product is p1 + p2.
|#

#|
Exercise 2.14: Demonstrate that Lem is right. Investigate
the behavior of the system on a variety of arithmetic expressions.
Make some intervals A and B, and use them in computing the 
expressions A/A and A/B. You will get the most insight by using 
intervals whose width is a small percentage of the center value.
Examine the results of the computation in center-percent form
(see Exercise 2.12).
|#

(define (print-center-percent x)
  (display "center: ")
  (display (center x))
  (display ", percent: ")
  (display (percent x))
  (newline))

; Lem's method
(define (par1 r1 r2)
  (div-interval (mul-interval r1 r2)
				(add-interval r1 r2)))

; Alyssa's method
(define (par2 r1 r2)
  (let ((one (make-interval 1 1)))
	(div-interval
	  one (add-interval (div-interval one r1)
						(div-interval one r2)))))

(define (test-lem)
  (let ((A (make-center-percent 10000 5))
	    (B (make-center-percent 330 10)))
	(display "using par1: ")
	(print-center-percent (par1 A B))
	(display "using par2: ")
	(print-center-percent (par2 A B))))
(test-lem)

#|
using par1: center: 324.39127376040165, percent: 19.931607019958708
using par2: center: 319.4331254203496, percent: 9.841433938087881

Lem is right, Alyssa's function has smaller percentages than Lems.
|#

(define (test-div)
  (let ((A (make-center-percent 5000 2))
	    (B (make-center-percent 2500 1)))
	(display "A/A: ")
	(print-center-percent (div-interval A A))
	(display "A/B: ")
	(print-center-percent (div-interval A B))))
(test-div)

#|
Exercise 2.15: Eva Lu Ator, another user, has also noticed
the different intervals computed by different but algebraically
equivalent expressions. She says that a formula to compute
with intervals using Alyssa’s system will produce tighter
error bounds if it can be written in such a form that no variable 
that represents an uncertain number is repeated. Thus,
she says, par2 is a “better” program for parallel resistances
than par1. Is she right? Why?

Yes, Eva is right because the in par1 the interval appears
twice and its uncertainty is applied twice to the function.
par2 has the intervals written once and produces the correct amount
of uncertainty.
|#

#|
Exercise 2.16: Explain, in general, why equivalent algebraic
expressions may lead to different answers. Can you
devise an interval-arithmetic package that does not have
this shortcoming, or is this task impossible? (Warning: This
problem is very difficult.)

The answers are different because the uncertainty of the interval
is present more than once in the expression. It is not possible to 
create an interval package that completely removes the dependacy problem.
|#
