;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests pair? primitive 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(pair? '())
(pair? '(1 2))
(pair? (cons 1 2))
(pair? (list 1 2))

(pair? 1)
(pair? (+ 1 2 3))
(pair? '(+ 1 2 3))

(if (pair? (cons 1 2)) "Ça marche" "Ça marche pas")
