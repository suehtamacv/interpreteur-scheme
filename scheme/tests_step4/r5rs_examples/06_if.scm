;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=IF test
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(if (> 3 2) 'yes 'no)
(if (> 2 3) 'yes 'no)
(if (> 3 2)
	(- 3 2)
	(+ 3 2))

