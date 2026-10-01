#|
Exercise 1.25: Alyssa P. Hacker complains that we went to
a lot of extra work in writing expmod. Aer all, she says,
since we already know how to compute exponentials, we could have
simply written

(define (expmod base exp m)
  (remainder (fast-expt base exp) m))

Is she correct? Would this procedure serve as well for our
fast prime tester? Explain.

Answer:
fast-expr is only optimized for even numbers, fast-prime using the fermat
test would also produce odd numbers randomly, so it would be faster 
sometimes

Exercise 1.26: Louis Reasoner is having great difficulty doing
Exercise 1.24. His fast-prime? test seems to run more
slowly than his prime? test. Louis calls his friend Eva Lu
Ator over to help. When they examine Louis’s code, they
find that he has rewriten the expmod procedure to use an
explicit multiplication, rather than calling square:

(define (expmod base exp m)
  (cond ((= exp 0) 1)
		((even? exp)
		(remainder (* (expmod base (/ exp 2) m)
				      (expmod base (/ exp 2) m))
                   m))
		(else
			(remainder (* base
						  (expmod base (- exp 1) m))
						m))))

“I don’t see what difference that could make,” says Louis.
“I do.” says Eva. “By writing the procedure like that, you
have transformed the Θ(logn) process into a Θ(n) process.”
Explain

Answer:
The results of expmod must be calculated twice before squaring them.
an iterative process has been turned into a recursive process with 2
branches.

|#

