;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=Tests char? with more than one argument 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(char? (if (< (+ 1 2) 3) #\a 0))
(char? +)
(char?)
(char? 1 2)
