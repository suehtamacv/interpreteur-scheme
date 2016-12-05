;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Cond
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(cond
 ((> 3 2) 'greater)
 ((< 3 2) 'less))
(cond
 ((> 3 3) 'greater)
 ((< 3 3) 'less)
 (else 'equal))
