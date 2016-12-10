;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Factorial as a lambda 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; The FACT procedure computes the factorial
;;; of a non-negative integer.
(define fact
  (lambda (n)
	(if (= n 0)
		1
		;Base case: return 1
		(* n (fact (- n 1))))))
