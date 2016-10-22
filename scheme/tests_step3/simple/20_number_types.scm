;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests number types 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define zero 0)
(number? zero)
(integer? zero)
(real? zero)
(complex? zero)
(positive? zero)
(negative? zero)
(zero? zero)

(define un 1)
(number? un)
(integer? un)
(real? un)
(complex? un)
(positive? un)
(negative? un)
(zero? un)

(define real 1.234)
(number? real)
(integer? real)
(real? real)
(complex? real)
(positive? real)
(negative? real)
(negative? (- real))
(zero? real)
(zero? (+ real (- real)))

(define compl 1-2i)
(number? compl)
(integer? compl)
(real? compl)
(complex? compl)
(zero? compl)

