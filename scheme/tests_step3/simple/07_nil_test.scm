;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests null? procedure  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(null? #\a)
(null? 3)
(null? '())
(null? '(1 2 3))

(define x '())
(null? 'x)
(null? x)
