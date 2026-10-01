#|
we'll define some function 'f' that takes an arg 'g'
all f does is apply g to 2.
|#

(define (f g) (g 2))

#|
now we'll become evil and pass f itself
|#

(display (f f))
(newline)

#|
the callstack will look something like:

(f f)
(f 2)
(2 2)

error: cannot apply 2, we're trying to use an integer as a function

if however we defined f as:

(define (f g) (g g))

then the program would run indefinitely

|#

