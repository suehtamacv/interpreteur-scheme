;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Let et Let*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(let ((x 2) (y 3))
  (* x y))
(let ((x 2) (y 3))
  (let ((x 7) (z (+ x y)))
	(* z x)))

(let ((x 2) (y 3))
  (let* ((x 7) (z (+ x y)))
	(* z x)))
