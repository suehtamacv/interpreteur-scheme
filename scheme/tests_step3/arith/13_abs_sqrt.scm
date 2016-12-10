;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests sqrt and abs functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(abs 1)
(abs +1)
(abs 1.1)
(abs -1)
(abs -3.312)
(abs NaN)
(abs +inf)
(abs -inf)
(abs 3+4i)
(abs (string->number "3+4i"))

(sqrt 1)
(sqrt 4)
(sqrt 9)
(sqrt 9.0)
(imag-part (sqrt -9))
(sqrt NaN)
(sqrt +inf)
