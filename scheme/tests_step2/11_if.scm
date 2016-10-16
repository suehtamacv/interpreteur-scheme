;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Basic IF tests 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define x 3)

(if #t "TRUE" "FALSE")
(if (and #t 1 x) x "PAS DE X")

(set! x #f)
(if (and #t 1 x) x "X EST FAUX")

(if (and #t 1 x) CaNExistePas 'x)
(if #f CaNExistePas x)
