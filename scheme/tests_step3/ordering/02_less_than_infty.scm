;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests short circuit when comparing with infinity 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(< +inf 0)
(< 1 +inf)
(< 1 +inf 0 1000)
(< 1 2 +inf 0 1 2)

(< -inf 1)
(< -inf 1 2)
(< -inf 1 2 3)
(< -inf 1 2 3 1)
