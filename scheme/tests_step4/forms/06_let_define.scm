;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=Let with define
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(let ((x 3)) x)
(let ((x 3) (y 5)) x (+ x y) (define x 2))
(let ((x (define z 3))) x)
