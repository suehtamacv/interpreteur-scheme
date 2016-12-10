;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests Procedure scheme 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;Checking the condition of trigular
(define (is_trigular x y z) 
(if (or (> x (+ y z)) (> y (+ x z)) (> z (+ x y))) "is not Trigular" "is Trigular"))
(is_trigular 3 4 5)
(is_trigular 3 4 9)
;;Calculation of n!
(define (recursive x) 
(if (< x 1) 1 (* (recursive (- x 1)) x)))
(recursive 1)
(recursive 6)

