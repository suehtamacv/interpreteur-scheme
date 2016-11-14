;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests if can define in an environment 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define x 5)
(define myenv (interaction-environment))

x
(eval '(define x "Not 5") myenv)
x
(eval 'x myenv)
