;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Lambda does not create variable in global environment
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define y 3)
y
((lambda (x) (begin (define y 10) y)) 1)
y
