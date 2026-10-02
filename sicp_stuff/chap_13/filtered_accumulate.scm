
;; prime? predicate

(define true #t)
(define false #f)

(define (square n) (* n n))

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
  (define (try-it a)
	(= (expmod a n n) a))
  (try-it (+ 1 (random (- n 1)))))

(define (fast-prime? n times)
  (cond ((= times 0) true)
		((fermat-test n) (fast-prime? n (- times 1)))
		(else false)))

(define (prime? n)
  (fast-prime? n 3))

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

(define (filtered-accumulate predicate combiner null-value term a next b)
  (if (not (predicate (term a)))
	  (filtered-accumulate predicate combiner null-value term (next a) b))
  (if (> a b)
	null-value
	(combiner (term a)
			  (filtered-accumulate predicate combiner null-value term (next a) b))))


;; sum of all squares of primes in the interval a to b

(define (sum-square-primes a b)
  (define (square n) (* n n))
  (define (term n) n)
  (define (next n) (+ n 1))
  (filtered-accumulate prime? + 0 term a next b)

;; 3025
(display (sum-cubes 0 10))
(newline)

#|
b. the product of all the positive integers less than n that are
	relatively prime to n (i.e., all positive integers i < n
	such that GCD(i, n) = 1).
|#

