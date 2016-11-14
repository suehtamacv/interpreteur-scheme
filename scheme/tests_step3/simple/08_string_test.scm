;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests string? primitive 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(string? #t)
(string? "I'm a string!")
(string? 'IAmNot)

(string? '(1 2 3))
(if (string? "Does it work?") ":)" ":(")
(and #t (string? "I Work") #t)
(or #f #t (string? CourtCircuit))
