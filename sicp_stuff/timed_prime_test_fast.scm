;; Ingore this
(define (runtime)
  (exact->inexact (/ (get-internal-run-time) 
					 internal-time-units-per-second)))

(define true #t)
(define false #f)

(define (square n) (* n n))

#|
Exercise 1.23: 

The smallest-divisor procedure shown at the start of this section does 
lots of needless testing:
Afer it checks to see if the number is divisible by 2 there is no point in
checking to see if it is divisible by any larger even numbers. 

This suggests that the values used for test-divisor should not be 2, 3, 4, 5,
6, ..., but rather 2, 3, 5, 7, 9, ...
To implement this change, define a procedure next that returns 3 if its input 
is equal to 2 and otherwise returns its input plus 2. Modify the smallest-divisor
procedure to use (next test-divisor) instead of (+ test-divisor 1). With
timed-prime-test incorporating this modified version of smallest-divisor, run 
the test for each of the 12 primes found in Exercise 1.22. Since this 
modification halves the number of test steps, you should expect it to run about
twice as fast. Is this expectation confirmed? If not, what is the observed

ratio of the speeds of the two algorithms, and how do you explain the fact that
it is different from 2?
|#

(define (smallest-divisor n)
  (find-divisor n 2))

(define (find-divisor n test-divisor)
  (define (next n)
	(cond ((= n 2) 3)
		  (else (+ n 2))))
  (cond ((> (square test-divisor) n) n)
		((divides? test-divisor n) test-divisor)
		(else (find-divisor n (next test-divisor)))))

(define (divides? a b)
  (= (remainder b a) 0))

(define (prime? n)
  (= n (smallest-divisor n)))

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

(search-for-primes 1000 1020)
(search-for-primes 10000 10040)
(search-for-primes 100000 100140)
(search-for-primes 1000000 1000040)

#|
1009 *** 1.333299999999038e-5
1013 *** 2.0869999999978406e-6
1019 *** 2.1560000000031554e-6

10007 *** 2.1810000000038743e-6
10009 *** 2.509000000011641e-6
10037 *** 1.97399999998793e-6

100003 *** 6.1489999999969625e-6
100019 *** 5.014999999997105e-6
100043 *** 5.05600000000328e-6

1000003 *** 1.4611999999997183e-5
1000033 *** 1.4233999999987978e-5
1000037 *** 1.5213000000000032e-5

For larger numbers this algorithm is generally faster!
|#
