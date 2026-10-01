;; Ingore this
(define (runtime)
  (exact->inexact (/ (get-internal-run-time) 
					 internal-time-units-per-second)))

(define true #t)
(define false #f)

(define (square n) (* n n))

#|
Exercise 1.28: One variant of the Fermat test that cannot
be fooled is called the Miller-Rabin test (Miller 1976; Rabin
1980). This starts from an alternate form of Fermat’s Little
theorem, which states that if n is a prime number and a is
any positive integer less than n, then a raised to the (n−1)-st
power is congruent to 1 modulo n.

To test the primality of a number n by the Miller-Rabin test,
we pick a random number a < n
and raise a to the (n − 1)-st power modulo n using
the expmod procedure. However, whenever we perform the
squaring step in expmod, we check to see if we have discovered a
“nontrivial square root of 1 modulo n,” that is, a number not
equal to 1 or n−1 whose square is equal to 1 modulo n.
It is possible to prove that if such a nontrivial square root
of 1 exists, then n is not prime.

It is also possible to prove that if n is an odd number that
is not prime, then, for at least half the numbers a < n,
computing a n−1 in this way will reveal a nontrivial square
root of 1 modulo n.
(This is why the Miller-Rabin test cannot be fooled.)
Modify the expmod procedure to signal if it discovers a
nontrivial square root of 1, and use this to implement the
Miller-Rabin test with a procedure analogous to fermat-test.

Check your procedure by testing various known primes and non-primes.
Hint: One convenient way to make expmod signal is to have it return 0.
|#

(define (expmod base exp m)		; (base ^ exp) % m, (a ^ (n - 1)) % n
  (define (nontrivial r n)
	;; true for r != 1, r != n - 1, r^2 == 1 % n
	(and (not (= r 1)) (not (= r (- n 1)))
		 (= (remainder (square r) n) 1)))

  (define (nontrivial-check r)
	(if (nontrivial r m)
	  0	;; no recursion in case of nontrivial square-root found
	  (remainder (square r) m)))

  (cond ((= exp 0) 1)
		((nontrivial base m) 0)
		((even? exp)
		  (nontrivial-check
			(expmod base (/ exp 2) m)))
		(else
		  (remainder
			(* base (expmod base (- exp 1) m))
			m))))

(define (miller-rabin n)
  (define (try-it a n)
	(cond ((< a 2) true)			; miller-rabin passed for all a < n
		  ((= (expmod a n n) a)		; raise a to (n - 1)-st power modulo n
		      (try-it (- a 1) n))	; try next a < n
		  (else false)))			; failed miller-rabin 
  (try-it (- n 1) n))

;; true
(display (miller-rabin 29))
(newline)

;; false
(display (miller-rabin 561))
(newline)

;; false
(display (miller-rabin 1105))
(newline)

;; false
(display (miller-rabin 1729))
(newline)

;; false
(display (miller-rabin 2465))
(newline)

;; false
(display (miller-rabin 2821))
(newline)

;; false
(display (miller-rabin 6601))
(newline)
