;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=AND et OR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(and (= 2 2) (> 2 1))
(and (= 2 2) (< 2 1))
(and 1 2 'c '(f g))
(and)

(or (= 2 2) (> 2 1))
(or (= 2 2) (< 2 1))
(or #f #f #f)
