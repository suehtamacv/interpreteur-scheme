;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests number? primitive 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(number? #t)
(number? #f)
(number? "0")
(number? #\0)
(number? '0)

(number? 0)
(number? 1)
(number? -1)
(number? 1.1)
(number? 1.111111)
(number? 3.14)
(number? +inf)
(number? -inf)
(number? 1-2j)
(number? 1+2j)
(number? 1-2i)
(number? 1+2i)
