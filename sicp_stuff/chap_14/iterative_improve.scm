#|
Exercise 1.46: Several of the numerical methods described
in this chapter are instances of an extremely general
computational strategy known as iterative improvement.
Iterative improvement says that, to compute something, we start
with an initial guess for the answer, test if the guess is good
enough, and otherwise improve the guess and continue the
process using the improved guess as the new guess. 

Write a procedure iterative-improve that takes two procedures
as arguments:

	a method for telling whether a guess is good enough
	and a method for improving a guess.

iterative-improve should return as its value a procedure that
takes a guess as argument and keeps improving the guess until it is
good enough.

Rewrite the sqrt procedure of Section 1.1.7
and the fixed-point procedure of Section 1.3.3 in terms of
iterative-improve.
|#

;; iterative improve

(define (iterative-improve close-enough? improve)
  (define (iter guess)
	(if (close-enough? guess)
		guess
		(iter (improve guess))))
  (lambda (guess) (iter guess)))

;; rewrite of sqrt using iterative-improve

(define (sqrt x)
  (define (improve guess)
	(/ (+ guess (/ x guess)) 2))
  (define (close-enough? guess)
	(< (abs (- (* guess guess) x)) 0.001))
  ((iterative-improve close-enough? improve) 1.0))

(display (sqrt 2))
(newline)
(display (sqrt 9))
(newline)

;; rewrite of fixed-point using iterative-improve

(define (fixed-point f first-guess)
  (define tolerance 0.00001)
  (define (improve guess)
	(f guess))
  (define (close-enough? guess)
	(< (abs (- guess (f guess)))
		tolerance))
  ((iterative-improve close-enough? improve) first-guess))

(define (n-th-root n x)
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
  (let ((damping n))
	(fixed-point
	  (smooth
		(lambda (y)(/ x (expt y (- n 1))))
		  damping)
	  1.0)))

(display (n-th-root 2 49))
(newline)
(display (n-th-root 3 729))
(newline)

;; Its TOO EASY RAAAA!!!!
