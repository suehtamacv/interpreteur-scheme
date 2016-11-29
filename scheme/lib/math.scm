(define gcd
  (lambda (a b)
	(define a (abs a))
	(define b (abs b))
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
	(* 0-1j (sin (* x 0+1j))))) ;; sinh(x) = -j sin(jx)
(define cosh
  (lambda (x)
	(cos (* x 0+1j)))) ;; cosh(x) = cos(jx)
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
	(* 0-1j (log (+ (* 0+1j z) (sqrt (- 1 (* z z))))))))
(define acos
  (lambda (z)
	(- 1.57079632679 (asin z))))
(define atan
  (lambda (z)
	(/ (- (log (+ 1 (* 0+1j z))) (log (- 1 (* 0+1j z)))) 0+2j)))
