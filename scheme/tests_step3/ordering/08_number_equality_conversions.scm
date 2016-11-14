;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests number equality between different types
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(= 1 +1)
(= +1 -1)

(= 1 1.0)
(= 1.0 1)
(= +1 1.0)
(= 1.0 +1)
(= 1 -1.0)
(= -1.0 1)
(= +1 -1.0)
(= -1.0 +1)

(= 1 1+0j)
(= +1 1+0j)
(= 1 1-0j)
(= +1 1-0j)
(= 1.0 1+0j)
(= 1.0 1-0j)

(= 1 1+2j)
(= +1 1+2j)
(= 1 1-2j)
(= +1 1-2j)
(= 1.0 1+2j)
(= 1.0 1-2j)
