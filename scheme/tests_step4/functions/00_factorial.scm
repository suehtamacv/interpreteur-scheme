;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Factorial function 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define n 30)

(define factorial
  (lambda (n)
	(if (= n 0)
		1
		(* n (factorial (- n 1))))))

(factorial 0)
(factorial 1)
(factorial 5)
(factorial 10)
(factorial 12)

n
