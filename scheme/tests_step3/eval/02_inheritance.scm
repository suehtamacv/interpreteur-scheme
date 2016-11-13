;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests if inheritance works 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define Env0 (interaction-environment))
(eval '(define Env1 (interaction-environment)) Env0)
(eval '(define Env2 (interaction-environment)) (eval 'Env1 Env0))

(define x 123)
(eval '(define x (exp 0)) Env0)

x
(eval 'x Env0)
(eval 'x (eval 'Env1 Env0))
(eval 'x (eval 'Env2 (eval 'Env1 Env0)))

(eval '(define y 42) (eval 'Env1 Env0))
(eval 'y (eval 'Env2 (eval 'Env1 Env0)))
(eval 'y (eval 'Env1 Env0))

(eval '(define y "No Longer 42") (eval 'Env2 (eval 'Env1 Env0)))
(eval 'y (eval 'Env2 (eval 'Env1 Env0)))
(eval 'y (eval 'Env1 Env0))

