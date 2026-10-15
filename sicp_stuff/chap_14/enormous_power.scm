#|
Exercise 1.40: Define a procedure cubic that can be used
together with the newtons-method procedure in expressions
of the form

(newtons-method (cubic a b c) 1)

to approximate zeros of the cubic x^3 + ax^2 + bx + c.
|#

(define dx 0.00001)

(define (deriv g)
  (lambda (x) (/ (- (g (+ x dx)) (g x)) dx)))

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

(define (newton-transform g)
  (lambda (x) (- x (/ (g x) ((deriv g) x)))))

(define (newtons-method g guess)
  (fixed-point (newton-transform g) guess))

;; my procedure here
(define (cubic a b c)
  (lambda (x) (+ (* x x x) (* a x x) (* b x) c)))

;; test, works with desmos
(display (newtons-method (cubic 2 3 -3) 1))
(newline)

;; test lecture question
(define (func d)
  (- (+ (* 2.1353 (cos d)) d) 2.2955))

(display "newtons: ")
(display (newtons-method func 10))
(newline)
