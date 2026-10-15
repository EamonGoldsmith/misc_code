#|
Exercise 1.42: Let f and д be two one-argument functions.
The composition f after д is defined to be the function 

	x 7→ f(д(x)).

Define a procedure compose that implements composition.
For example, if inc is a procedure that adds 1 to its argument,

((compose square inc) 6)
49
|#

(define (inc x)
  (+ x 1))

(define (square x)
  (* x x))

(define (compose f g)
  (lambda (x) (f (g x))))

;; (6 + 1)^2 = 49
(display ((compose square inc) 6))
(newline)
