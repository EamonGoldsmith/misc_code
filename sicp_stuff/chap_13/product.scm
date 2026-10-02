#|
Exercise 1.31:

a. The sum procedure is only the simplest of a vast number of similar
	abstractions that can be captured as higher order procedures.
	Write an analogous procedure called product that returns the product
	of the values of a function at points over a given range.

	Show how to define factorial in terms of product. Also use product
	to compute approximations to π using the formula:

	   π	  2 · 4 · 4 · 6 · 6 · 8 · · ·
	   -  =	 ------------------------------
	   4	  3 · 3 · 5 · 5 · 7 · 7 · · ·
|#

(define (product term a next b)
  (if (> a b)
	1
	(* (term a)
	   (product term (next a) next b))))

(define (factorial n)
  (define (term a) a)
  (define (next a) (+ a 1))
  (product term 1 next n))

(display "(factorial 7): ")
(display (factorial 7))
(newline)

(define (prod-pi n)
  (define (term a) 
	(cond 
	  ((= a 1)
	   (/ 2.0 3.0))
	  ((even? a)
	   (/ (+ a 2.0) (+ a 1.0)))
	  ((odd? a)
	   (/ (+ a 1.0) (+ a 2.0)))))
  (define (next a) (+ a 1))
  (product term 1 next n))

(display "(prod-pi * 4): ")
(display (* 4 (prod-pi 100000)))
(newline)

#|
b. If your product procedure generates a recursive process,
	write one that generates an iterative process. If
	it generates an iterative process, write one that generates
	a recursive process.
|#

;; my product produced a recursive process, so I will make it iterative
(define (product-it term a next b)
  (define (iter a result)
    (if (> a b)
	  result
	  (iter (next a) (* result (term a)))))
  (iter a 1))

;; we can test it with the factorial program
(define (factorial-iter n)
  (define (term a) a)
  (define (next a) (+ a 1))
  (product-it term 1 next n))

(display "(factorial-iter 7): ")
(display (factorial-iter 7))
(newline)
