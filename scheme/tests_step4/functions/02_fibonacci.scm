;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Fibonacci function 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define fib
  (lambda (n)
	(if (= n 0)
		0
		(if (= n 1)
			1
			(+ (fib (- n 1)) (fib (- n 2)))))))

(fib 0)
(fib 1)
(fib 2)
(fib 3)
(fib 4)
(fib 5)
(fib 10)
(fib 20)

