
;; for our fixed point function
(define tolerance 0.00001)

(define (fixed-point f first-guess)
  (define (close-enough? v1 v2)
	(< (abs (- v1 v2))
	   tolerance))
  (define (try guess)
	(display guess) (newline)
	(let ((next (f guess)))
	  (if (close-enough? guess next)
		  next
	      (try next))))
  (try first-guess))

(define (transform x)
  (+ 1.0 (/ 1.0 x)))

;; fixed point of golden ratio tranform 
;; 1.61803398875
(display (fixed-point transform 1))
(newline)

;; we can also find the fixed point of cos
;; 0.7390822985224024
;; that is cos(0.739...) = 0.739...
(display (fixed-point cos 1))
(newline)

;; what about a function with no fixed points like f(x) = x + 1
(define (linear x) (+ x 1))
;; this will run forever as we keep getting next guess

;; what about the quadratic? f(x) = x^2, the only fixed point should be 1
(define (quad x) (* x x))
;; 1, what we expected, unless initial guess is greater than 1, then we explode
(display (fixed-point quad 1))
(newline)

;; what about collatz conjecture? f(x) = { odd: (3 * x) + 1, even: x / 2 }
(define (collatz x)
  (if (odd? x)
	(+ (* 3 x) 1)
	(/ x 2)))
;; (fixed-point collatz 1)) bounces around between 4 and 1
;; (fixed-point collatz 7)) reaches 1 and bounces around again
