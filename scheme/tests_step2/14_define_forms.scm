;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Replacing forms by symbols 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define x 10)
x

(define if "toto")
if

(define and #t)
(or #f #f #f #f and)

(define define "no longer a define")
define

