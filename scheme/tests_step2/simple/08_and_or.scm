;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Basic tests of AND and OR functionalities 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(and #f #f)
(and #t #f)
(and #f #t)
(and #t #t #t)
(or #f #f)
(or #t #f)
(or #f #t)
(or #t #t #t)

;; Court circuit
(and #t #f CaNExistePas)
(or #f #t CaNExistePas)
(and #t #f (or #f CaNExistePas))

(and (or #t #f) (or #f #f #f) (and #t #f) )

;;Nombre incorrecte d'arguments
(and)
(or)
