;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests procedure? primitive 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(procedure? procedure?)
(procedure? (procedure? procedure?))

(procedure? if)
(procedure? and)
(procedure? or)
(procedure? define)

(procedure? quote)
(define quote +)
(procedure? quote)
(quote 1 2 3)

(procedure? boolean?)
(define boolean? "No longer a procedure")
(procedure? boolean?)

(procedure? '(1 2))
(procedure? cons)
(procedure? (cons 1 2))

