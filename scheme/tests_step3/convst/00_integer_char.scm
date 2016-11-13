;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests conversion function between char and integer
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(integer->char 97)
(integer->char 98)
(integer->char 99)
(char->integer #\a)
(char->integer #\b)
(char->integer #\c)
(number? (char->integer #\a))
(char? (integer->char 97))
(+ (char->integer #\D) (char->integer #\d))
(define x (char->integer #\E))
(define y x)
(- (* 2 x) y y)
(- x y)
