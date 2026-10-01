;; Ingore this
(define (runtime)
  (exact->inexact (/ (get-internal-run-time) 
					 internal-time-units-per-second)))

(define true #t)
(define false #f)

(define (square n) (* n n))

#|
Exercise 1.24: Modify the timed-prime-test procedure of
Exercise 1.22 to use fast-prime? (the Fermat method), and
test each of the 12 primes you found in that exercise. Since
the Fermat test has Θ(logn) growth, how would you expect
the time to test primes near 1,000,000 to compare with the
time needed to test primes near 1000? Do your data bear
this out? Can you explain any discrepancy you find?
|#

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

(define (timed-prime-test n)
  (newline)
  (display n)
  (start-prime-test n (runtime)))
(define (start-prime-test n start-time)
  (if (prime? n)
      (report-prime (- (runtime) start-time))))
(define (report-prime elapsed-time)
  (display " *** ")
  (display elapsed-time))

;; find all primes less than range
(define (search-for-primes lower upper)
  (define (search-iter i l u)
	(cond ((> i u)
		   false)
	(else (timed-prime-test i)
		  (search-iter (+ i 1) l u))))
  (search-iter lower lower upper))
 
#|
Last 12 primes found:

1009 *** 1.9170000000040543e-6
1013 *** 1.6389999999977256e-6
1019 *** 1.5020000000004474e-6

10007 *** 3.835000000007582e-6
10009 *** 3.3930000000043092e-6
10037 *** 7.460000000000799e-6

100003 *** 9.159000000008577e-6
100019 *** 9.118000000002402e-6
100043 *** 9.034000000004982e-6

1000003 *** 2.9949000000001336e-5
1000033 *** 4.007899999999842e-5
1000037 *** 2.921099999998733e-5
|#

(search-for-primes 100 1000)

#|

10007 *** 3.526000000003693e-6
10009 *** 3.382000000023977e-6
10037 *** 3.1220000000220516e-6

100003 *** 3.575000000005657e-6
100019 *** 3.5730000000067097e-6
100043 *** 3.5059999999864644e-6

1009 *** 3.083000000014824e-6
1013 *** 2.946000000003668e-6
1019 *** 2.923999999987492e-6

1000003 *** 3.999000000004527e-6
1000033 *** 3.951000000002036e-6
1000037 *** 4.1010000000063496e-6

Yes, fermat theorum is much faster, the complexity is constant.
|#
