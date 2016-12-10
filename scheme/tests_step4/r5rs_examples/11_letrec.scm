;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Letrec
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(letrec
	((even?
	  (lambda (n)
		(if (zero? n)
			#t
			(odd? (- n 1)))))
	 (odd?
	  (lambda (n)
		(if (zero? n)
			#f
			(even? (- n 1))))))
  (even? 88))
