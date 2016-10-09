;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_IF=Test comment parsing"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; this is a comment

; another comment
;; with a new line char

'()
(quote ())
(quote (()))
(define x 2)
(quote (3 x))
'('1 '2 '3 )
'('1 (quote 2) '3 )
'''''''4
(quote (1 (2 3)))
;; another comment

