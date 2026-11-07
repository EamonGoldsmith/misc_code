#|
Exercise 2.4: Here is an alternative procedural representation of pairs.
For this representation, verify that (car (cons x y)) yields x
for any objects x and y.

(define (cons x y)
  (lambda (m) (m x y)))

(define (car z)
  (z (lambda (p q) p)))

What is the corresponding definition of cdr? 
(Hint: To verify that this works, make use of the substitution model of
Section 1.1.5.)

I got a little bit carried away with this question.
|#

;; true and false in the lambda calculus

(define (construct x y)
  (lambda (i) (cond ((= i 0) x)
					((= i 1) y))))
(define (first z)
  (z 0))
(define (second z)
  (z 1))

;; make a vector construction

(define (append-list l e)
  (construct l e))

(define (index-list l i)
  (cond ((= i 0) (first l))
		(else (index-list (second l) (- i 1)))))

;; test it

(define (test)
  (let ((numbers (append-list 2 
				 (append-list 9
				 (append-list 3
				 (append-list 1
				 (append-list 7 0)))))))
	(display (index-list numbers 2))
	(newline)))

(test)

