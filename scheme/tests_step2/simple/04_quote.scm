;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Test mixing quotes and apostrophes 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

'(quote 4)
(quote '4)
'('1 '2 '('3 '4))
'(x y z)
'(/ 1 0)
'(if #t (/ 1 0) CaNExistePas)
