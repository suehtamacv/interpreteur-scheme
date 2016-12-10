;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=Lambda with a define inside
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

((lambda (x) x) 3)
((lambda (x) x (+ x x)) 3)
(lambda (x) x (+ x x) (define x 3))
((lambda (x) x) (define x 3))
