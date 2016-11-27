;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Greatest common divisor (Euclid algorithm)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define gcd
  (lambda (a b)
	(if (= a b)
		a
		(if (> a b)
			(gcd (- a b) b)
			(gcd a (- b a))))))

(gcd 0 0)
(gcd 1 1)
(gcd 2 1024)
(gcd 1023 2)
(gcd 441 21)
(gcd 120 1)
