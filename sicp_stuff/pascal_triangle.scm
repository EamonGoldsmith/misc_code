#|
Exercise 1.12.  The following pattern of numbers is called Pascal's triangle.

	1
       1 1
      1 2 1
     1 3 3 1
    1 4 6 4 1

The numbers at the edge of the triangle are all 1,
and each number inside the triangle is the sum of the two numbers above it.
Write a procedure that computes elements of Pascal's triangle by means of a recursive process. 
|#

(define (pascal n k)
  (cond ((= k 1) 1)
	((= k n) 1)
	(else (+ (pascal (- n 1) (- k 1))
	      	 (pascal (- n 1) k)))))

(display (pascal 4 2)) ; gives 3
(newline)

(display (pascal 5 3)) ; gives 6
(newline)
