;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests boolean? primitive 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(boolean? #t)
(boolean? #f)

(boolean? 0)
(boolean? ())

(boolean? (and 1 2 3))
(boolean? (and #t #f #t))
(boolean? (if (boolean? #t) #t "not boolean"))

(boolean? (boolean? 'x))
(define x #t)
(boolean? 'x)
(boolean? x)
