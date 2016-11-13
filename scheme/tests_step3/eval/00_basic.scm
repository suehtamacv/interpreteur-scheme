;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests if the environment related primitives are implemented 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(interaction-environment)
(define myenv (interaction-environment))
(eval '"toto" myenv)
