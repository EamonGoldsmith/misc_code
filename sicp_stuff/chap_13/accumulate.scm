#|
Exercise 1.32:
a. Show that sum and product (Exercise 1.31) are both
	special cases of a still more general notion called accumulate
	that combines a collection of terms, using some general
	accumulation function:
	(accumulate combiner null-value term a next b)

	accumulate takes as arguments the same term and
	range specifications as sum and product, together with

	a combiner procedure (of two arguments) that specifies 
	how the current term is to be combined with the
	accumulation of the preceding terms and a null-value
	that specifies what base value to use when the terms
	run out. Write accumulate and show how sum and
	product can both be defined as simple calls to accumulate.
|#

(define (accumulate combiner null-value term a next b)
  (if (> a b)
	null-value
	(combiner (term a)
			  (accumulate combiner null-value term (next a) next b))))

;; sum implemented as a call to accumulate
(define (sum term a next b)
  (accumulate + 0 term a next b))

;; test with pi-sum
(define (pi-sum a b)
  (define (pi-term x)
	(/ 1.0 (* x (+ x 2))))
  (define (pi-next x)
	(+ x 4))
  (sum pi-term a pi-next b))

(display (* (pi-sum 1 100) 8))
(newline)

;; product implemented as a call to accumulate
(define (product term a next b)
  (accumulate * 1 term a next b))

;; test with factorial
(define (factorial n)
  (define (term a) a)
  (define (next a) (+ a 1))
  (product term 1 next n))

(display (factorial 7))
(newline)

#|
b. If your accumulate procedure generates a recursive
	process, write one that generates an iterative process.
	If it generates an iterative process,
	write one that generates a recursive process.
|#

;; my accumulate generates a recursive process so I'll make an iterative one
(define (accumulate-iter combiner null-value term a next b)
  (define (iter a result)
	(if (> a b)
	  result
	  (iter (next a) (combiner result (term a)))))
  (iter a null-value))

;; sum implemented as a call to accumulate
(define (sum-iter term a next b)
  (accumulate-iter + 0 term a next b))

;; test with pi-sum
(define (pi-sum-iter a b)
  (define (pi-term x)
	(/ 1.0 (* x (+ x 2))))
  (define (pi-next x)
	(+ x 4))
  (sum-iter pi-term a pi-next b))

(display (* (pi-sum-iter 1 100) 8))
(newline)

;; product implemented as a call to accumulate
(define (product-iter term a next b)
  (accumulate-iter * 1 term a next b))

;; test with factorial
(define (factorial-iter n)
  (define (term a) a)
  (define (next a) (+ a 1))
  (product-iter term 1 next n))

(display (factorial-iter 7))
(newline)
