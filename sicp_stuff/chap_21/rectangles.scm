#|
Exercise 2.3: Implement a representation for rectangles in
a plane. (Hint: You may want to make use of Exercise 2.2.)

In terms of your constructors and selectors, create procedures
that compute the perimeter and the area of a given rectangle.
Now implement a different representation for rectangles. 

Can you design your system with suitable abstraction
barriers, so that the same perimeter and area procedures
will work using either representation?
|#

;; point functions

(define (x-point p) (car p))
(define (y-point p) (cdr p))
(define (make-point x y) (cons x y))

(define (print-point p)
	(display "(")
	(display (x-point p))
	(display ",")
	(display (y-point p))
	(display ")\n"))

;; segment functions

(define (start-segment s) (car s))
(define (end-segment s) (cdr s))
(define (make-segment s e) (cons s e))

(define (midpoint-segment s)
  (make-point
	(/ (- 
		 (x-point (end-segment s))
		 (x-point (start-segment s)))
	   2.0)
	(/ (- 
		 (y-point (end-segment s))
		 (y-point (start-segment s)))
	   2.0)))

(define (length-segment s)
  (sqrt (+ (expt (- (x-point (end-segment s))
					(x-point (start-segment s))) 2)
		   (expt (- (y-point (end-segment s))
					(y-point (start-segment s))) 2))))

;; rectangle functions

(define (make-rect top-left bottom-right)
  (make-segment top-left bottom-right))

(define (width-rect r)
  (length-segment
	(make-segment
	  (start-segment r)
	  (make-point 
		(x-point (end-segment r))
		(y-point (start-segment r))))))

(define (height-rect r)
  (length-segment
	(make-segment
	  (start-segment r)
	  (make-point
		(x-point (start-segment r))
		(y-point (end-segment r))))))

(define (perimeter r)
  (* 2 (+ (width-rect r) (height-rect r))))

(define (area r)
  (* (width-rect r) (height-rect r)))

;; test

; 60
(display (perimeter (make-rect (make-point 10 0) (make-point 0 20))))
(newline)

; 200
(display (area (make-rect (make-point 10 0) (make-point 0 20))))
(newline)

