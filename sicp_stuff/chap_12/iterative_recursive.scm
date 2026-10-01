; 1. the recursive proceedure is a recursive process
(define (rfactorial n)
  (if (= n 1)
    1
    (* n (rfactorial (- n 1)))))

; 2. the recursive proceedure is an iterative process
(define (ifactorial n)
  (fact-iter 1 1 n))

(define (fact-iter product counter max-count)
  (if (> counter max-count)
    product
    (fact-iter (* counter product)
	       (+ counter 1)
	       max-count)))

(display (rfactorial 999)) ; takes almost 100 ms
(display (ifactorial 999)) ; takes almost no time at all
(newline)

#|

(A 1 10)
(A 0 (A 1 9))
(* 2 (A 0 (A 1 8)))
(* 2 (* 2 (A 0 (A 1 7))))
1^10
1

(A 2 4)
(A 1 (A 2 3))
(A 0 (A 1 (A 2 2)))
(* 2 (A 0 (A 1 (A 2 1))))
(* 2 (* 2 (A 0 (2))))
(* 2 (* 2 (* 2 (2))))
2^4
16

|#
