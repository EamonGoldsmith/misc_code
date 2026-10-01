#|
Exercise 1.20
How many remainder operations are actually performed for (gcd 206 40)

In the normal-order evaluation?
In the applicative-order evaluation?

“fully expand and then reduce” evaluation method
is known as normal-order evaluation, in contrast to the “evaluate the
arguments and then apply” method that the interpreter actually uses,
which is called applicative-order evaluation.

normal-order:

(gdc 206 40)
(gdc 40 (r 206 40)))
(gdc (r 206 40) (r 40 (r 206 40)))
(gdc (r 206 40) (r (r 206 40) (r 40 (r 206 40))))
(gdc (r (r 206 40) (r 40 (r 206 40))) (r (r 206 40) (r (r 206 40) (r 40 (r 206 40)))))
(gdc (r (r 206 40) (r (r 206 40) (r 40 (r 206 40)))) (r (r (r 206 40) (r 40 (r 206 40))) (r (r 206 40) (r (r 206 40) (r 40 (r 206 40))))))

remainder is evaluated 18 times 

applicative-order:

(gdc 206 40)
(gdc 40 (r 206 40))
(gdc 6 (r 40 6))
(gdc 4 (r 4 2))
(gdc 2 (r 2 0))

remainder is evaluated 4 times

The interpreter uses applicative-order because it is more efficient.

|#

(define (gdc a b)
  (if (= b 0)
	a
	(gdc b (remainder a b))))

(display (gdc 206 40))
(newline)

