#|
Exercise 2.5: Show that we can represent pairs of nonnegative integers 
using only numbers and arithmetic operations if we represent
the pair a and b as the integer that is the product 2^a*3^b.
Give the corresponding definitions of the procedures cons, car, and cdr.

since 2 and 3 are primes we can get the parts by remainder when divided
repeatedly by the base.
|#

(define (cons a b)
  (* (expt 2 a) (expt 3 b)))

(define (clean-divisions n f)
  (define (iter n i)
    (if (= (remainder n f) 0)
		(iter (/ n f) (+ i 1))
		i))
  (iter n 0))

(define (car i)
  (clean-divisions i 2))

(define (cdr i)
  (clean-divisions i 3))

;; test

(display (car (cons 1 2)))
(newline)

(display (cdr (cons 1 2)))
(newline)

(display (car (cons 12 32)))
(newline)

(display (cdr (cons 89 73)))
(newline)
