;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests eq? primitive 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(eq? 'a 'a)
(eq? (list 'a) (list 'a))
(eq? '() '())
(eq? #\A #\A)
(eq? car car)
