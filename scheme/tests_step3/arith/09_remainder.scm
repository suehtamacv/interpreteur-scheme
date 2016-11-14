;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=Tests "remainder" in these case error procedure
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(remainder)
(remainder 1)
(remainder (+ 5 6))
(remainder (- 5 6))
(remainder 1 2 3)
(remainder (+ 5 6) (+ 2 5) (- 5 6))
(remainder 5 6 7 8)
(remainder (+ 5 6) (- 2 5) (- 5 6) (- 9 8))
