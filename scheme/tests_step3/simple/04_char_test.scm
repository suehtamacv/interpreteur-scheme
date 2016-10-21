;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests char? procedure  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(char? #\a)
(char? #\z)
(char? #\space)
(char? #\newline)

(char? 1)
(char? 'a)
(char? '(1 2))
(char? "a")
(char? ())
(char? #f)

(define x #f)
(char? x)
(set! x #\x)
(char? x)
(char? 'x)
