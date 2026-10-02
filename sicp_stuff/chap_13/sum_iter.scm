#|
Exercise 1.30: The sum procedure above generates a linear
recursion. The procedure can be rewritten so that the sum
is performed iteratively. Show how to do this by filling in
the missing expressions in the following definition:

(define (sum term a next b)
  (define (iter a result)
    (if ⟨??⟩
      ⟨??⟩
	  (iter ⟨??⟩ ⟨??⟩)))
  (iter ⟨??⟩ ⟨??⟩))

|#

(define (sum term a next b)
  (define (iter a result)
	(if (> a b)
	  result
	  (iter (next a) (+ result (term a)))))
  (iter a 0))

;; lets test with simpsons rule

(define (simpson f a b n)
  (define (h a b n)
	(/ (- b a) n))

  (define (term k)
	(cond ((or (= k 0) (= k n))
		   (f (+ a (* k (h a b n)))))
		  ((even? k)
		   (* 2 (f (+ a (* k (h a b n))))))
		  ((odd? k)
		   (* 4 (f (+ a (* k (h a b n))))))))

  (define (next k)
	(+ k 1))

  (* (/ (h a b n) 3)
	 (sum term 0 next n)))

(define (cube n) (* n n n))

(display (simpson cube 0 1 100))
(newline)
