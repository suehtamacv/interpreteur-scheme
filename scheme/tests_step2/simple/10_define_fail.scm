;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=Define in a unvalid location 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(or #t (define x 3))
(and #t (define x 3))
(if #t (define x 3) CaNExistePas)
x
