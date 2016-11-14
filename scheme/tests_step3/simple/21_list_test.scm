;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests list? primitive 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(pair? '())
(list? '())

(pair? (cons 1 2))
(list? (cons 1 2))

(pair? (list 1 2))
(list? (list 1 2))

(pair? (cons 1 (cons 2 '())))
(list? (cons 1 (cons 2 '())))
