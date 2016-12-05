(define gcd
  (lambda (a b)
	(set! a (abs a))
	(set! b (abs b))
	(if (= a b)
		a
		(if (> a b)
			(gcd (- a b) b)
			(gcd a (- b a))))))
(define tan
  (lambda (x)
	(/ (sin x) (cos x))))
(define sinh
  (lambda (x)
	(* 0-1i (sin (* x 0+1i))))) ;; sinh(x) = -i sin(ix)
(define cosh
  (lambda (x)
	(cos (* x 0+1i)))) ;; cosh(x) = cos(ix)
(define tanh
  (lambda (x)
	(/ (sinh x) (cosh x))))
(define expt
  (lambda (z1 z2)
	(if (= z1 0)
		(if (= z2 0)
			1
			0)
		(exp (* z2 (log z1))))))
(define sqrt
  (lambda (x)
	(expt x (/ 2))))
(define asin
  (lambda (z)
	(* 0-1i (log (+ (* 0+1i z) (sqrt (- 1 (* z z))))))))
(define acos
  (lambda (z)
	(- 1.57079632679 (asin z))))
(define atan
  (lambda (z)
	(/ (- (log (+ 1 (* 0+1i z))) (log (- 1 (* 0+1i z)))) 0+2i)))
