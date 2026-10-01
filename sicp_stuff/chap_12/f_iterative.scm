#|
Exercise 1.11.  A function f is defined by the rule that
f(n) = n if n<3
f(n) = f(n - 1) + 2f(n - 2) + 3f(n - 3) if n> 3.
|#

; Write a procedure that computes f by means of a recursive process.
(define (fr n)
  (cond ((< n 3) n)
	((> n 3) (+ 
		 (fr (- n 1))
		 (* 2 (fr (- n 2)))
		 (* 3 (fr (- n 3))))

; Write a procedure that computes f by means of an iterative process.
(define (fi n)
  (if (< n 3)
    n
    (fi-iter n)

(define (fi-iter a b c i n)
  (if (> i n)
    a
    (fi-iter (+ a (* 2 b) (* 3 c))
	     a
	     b
	     (+ i 1)
	     n)))
