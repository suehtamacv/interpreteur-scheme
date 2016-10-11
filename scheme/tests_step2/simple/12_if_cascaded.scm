;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=More sofisticated IF tests 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define x "tata")
(define y "toto")

(define z (if #t x y))

(if x "\"toto\" est vrai" "\"toto\" est faux")
(if (if x y z) "\"toto\" est vrai" "\"toto\" est faux") 
