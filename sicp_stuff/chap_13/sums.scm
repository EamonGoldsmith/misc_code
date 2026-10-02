(define (sum-integers a b)
  (if (> a b)
	0
	(+ a (sum-integers (+ a 1) b))))

;; 55
(display (sum-integers 0 10))
(newline)

(define (cube n) (* n n n))

(define (sum-cubes a b)
  (if (> a b)
	0
	(+ (cube a)
	   (sum-cubes (+ 1 a) b))))

;; 3025
(display (sum-cubes 0 10))
(newline)

(define (pi-sum a b)
  (if (> a b)
	0
	(+ (/ 1.0 (* a (+ a 2)))
	   (pi-sum (+ a 4) b))))

;; 3.1415
(display (* (pi-sum 1 1000) 8))
(newline)

(define (sum term a next b)
  (if (> a b)
	0
	(+ (term a)
	   (sum term (next a) next b))))

(define (pi-sum2 a b)
  (define (pi-term x)
	(/ 1.0 (* x (+ x 2))))
  (define (pi-next x)
	(+ x 4))
  (sum pi-term a pi-next b))

;; 3.1415
(display (* (pi-sum 1 1000) 8))
(newline)

