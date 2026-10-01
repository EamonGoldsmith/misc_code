;; Ingore this
(define (runtime)
  (exact->inexact (/ (get-internal-run-time) 
					 internal-time-units-per-second)))

(define true #t)
(define false #f)

(define (square n) (* n n))

#|
Exercise 1.22: Most Lisp implementations include a primitive called runtime that
returns an integer that specifies the amount of time the system has been running
(measured, for example, in microseconds). The following timedprime-test
procedure, when called with an integer n, prints n and checks to see if n is
prime. If n is prime, the procedure prints three asterisks followed by the
amount of time used in performing the test.

Using this procedure, write a procedure search-for-primes that checks the 
primality of consecutive odd integers in a specified range. Use your procedure 
to find the three smallest primes larger than 1000; larger than 10,000; larger 
than 100,000; larger than 1,000,000. Note the time needed to test each prime. 

Since the testing algorithm has order of growth of Θ(√n), you should expect that
testing for primes around 10,000 should take about √10 times as long as testing 
for primes around 1000. Do your timing data bear this out? 

How well do the data for 100,000 and 1,000,000 support the Θ(√n) prediction? 

Is your result compatible with the notion that programs on your machine run in 
time proportional to the number of steps required for the computation?
|#

(define (smallest-divisor n)
  (find-divisor n 2))

(define (find-divisor n test-divisor)
  (cond ((> (square test-divisor) n) n)
		((divides? test-divisor n) test-divisor)
		(else (find-divisor n (+ test-divisor 1)))))

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
		  (search-iter (+ i 2) l u))))
  (search-iter lower lower upper))

(newline)
(search-for-primes 1001 1020)

#|
1009 *** 1.9170000000040543e-6
1013 *** 1.6389999999977256e-6
1019 *** 1.5020000000004474e-6
avg = 1.68
|#

(newline)
(search-for-primes 10001 10040)

#|
sqrt(10) = 3.16
3.16 * 1.68 = 5.3088

10007 *** 3.835000000007582e-6
10009 *** 3.3930000000043092e-6
10037 *** 7.460000000000799e-6

avg = 4.89

the average time is slightly lower than the predited O(sqrt(10)) time
|#

(newline)
(search-for-primes 100001 100100)

#|
100003 *** 9.159000000008577e-6
100019 *** 9.118000000002402e-6
100043 *** 9.034000000004982e-6

avg = 9.105
predicted = 15.35

actual time is much lower than predicted by complexity
|#

(newline)
(search-for-primes 1000001 1000100)

#|
1000003 *** 2.9949000000001336e-5
1000033 *** 4.007899999999842e-5
1000037 *** 2.921099999998733e-5

avg = 33
predicted = 28.587

actual time is larger than the predicted value. resolutions this small cannot
be acurately measured. We'd need to run the test many times to average out OS
interference.
|#

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

(define (expmod base exp m)
  (cond ((= exp 0) 1)
		((even? exp) (remainder (square (expmod base (/ exp 2) m)) m))
		(else		 (remainder (* base (expmod base (- exp 1) m)) m))))

(define (fermat-test n)
  (define (try-it a)
    (= (expmod a n n) a))
  (try-it (+ 1 (random (- n 1)))))

(define (fast-prime? n times)
  (cond ((= times 0) true))
		((fermat-test n) (fast-prime? n (- times 1)))
		(else false)))

|#
