;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=Tests string? with more than one argument 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(string? "YAY")
(string? (boolean? (char? #\a)))
(string? (+ 1 2))
(string? "I" "SHOULD" "NOT" "WORK")
