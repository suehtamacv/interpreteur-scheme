;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests Begin sheme 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define x 123)
(if (> x 120) (begin (- x 23) (- x 123) (* x 10)) (begin (+ x 3)))

