;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests let scheme
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


(let ((x 1) (y 2) (z 3)) (+ x y z))
(let ((x 2) (y 3)) 
(let ((x (let ((a 1) (b 2) (c 3)) (* a b c))) (z (+ x y))) (* z x)))
