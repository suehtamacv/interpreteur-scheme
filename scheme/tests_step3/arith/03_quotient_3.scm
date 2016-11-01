;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=Tests "quotient" in these case wrong procedure
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(quotient)
(quotient 1)
(quotient (+ 5 6))
(quotient (- 5 6))
(quotient 1 2 3)
(quotient (+ 5 6) (+ 2 5) (- 5 6))
(quotient 5 6 7 8)
(quotient (+ 5 6) (- 2 5) (- 5 6) (- 9 8))
