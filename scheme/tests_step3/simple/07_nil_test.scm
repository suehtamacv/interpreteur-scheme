;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests nil? procedure  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(nil? #\a)
(nil? 3)
(nil? '())
(nil? '(1 2 3))
(nil?)

(define x '())
(nil? 'x)
(nil? x)
