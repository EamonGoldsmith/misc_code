#|
Exercise 1.33: You can obtain an even more general version of
accumulate (Exercise 1.32) by introducing the notion of a filter
on the terms to be combined. That is, combine only those terms
derived from values in the range that satisfy a specified
condition. The resulting filtered-accumulate abstraction takes
the same arguments as accumulate, together with an additional
predicate of one argument that specifies the filter.

Write filtered-accumulate as a procedure. Show how to express
the following using filtered-accumulate:

a. the sum of the squares of the prime numbers in the
	interval a to b (assuming that you have a prime? predicate already written)
|#

(define (square n) (* n n))

;; prime? predicate
(define (smallest-divisor n)
  (find-divisor n 2))

(define (find-divisor n test-divisor)
  (cond ((> (square test-divisor) n) n)
		((divides? test-divisor n) test-divisor)
		(else (find-divisor n (+ test-divisor 1)))))

(define (divides? a b)
  (= (remainder b a) 0))

(define (prime? n)
  (if (= n 1)
	#f
	(= n (smallest-divisor n))))

;; filter-accumulate function
(define (filtered-accumulate predicate combiner null-value term a next b)
  (if (> a b)
	null-value
	(combiner 
	  (if (not (predicate a))
		  null-value
		  (term a))
	  (filtered-accumulate predicate combiner null-value term (next a) next b))))

;; sum of all squares of primes in the interval a to b
(define (sum-square-primes a b)
  (define (term n) (square n))
  (define (next n) (+ n 1))
  (filtered-accumulate prime? + 0 term a next b))

;; 87
(display (sum-square-primes 1 10))
(newline)

#|
b. the product of all the positive integers less than n that are
	relatively prime to n (i.e., all positive integers i < n
	such that GCD(i, n) = 1).
|#

(define (gdc a b)
  (if (= b 0)
	a
	(gdc b (remainder a b))))

(define (product-relatively-prime n)
  (define (term i) i)
  (define (next i) (+ i 1))
  (define (predicate i) (= (gdc i n) 1))
  (filtered-accumulate predicate * 1 term 1 next n))

;; RAAAA yeah first try 189!!! I'm da GOAT
(display (product-relatively-prime 10))
(newline)
