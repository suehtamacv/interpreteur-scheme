;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests trig functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define pi 3.14159265359)
(define eps 0.000000001)
(define ang 123.4)

(sin 0)
(cos 0)
(tan 0)
(< (abs (sin pi)) eps)
(cos pi)
(< (abs (tan pi)) eps)

(+ (* (cos ang) (cos ang)) (* (sin ang) (sin ang)))
(< (abs (- (* 2 (cos ang) (sin ang)) (sin (* 2 ang)))) eps)

(sin +inf)
(sin -inf)
(sin NaN)
(cos +inf)
(cos -inf)
(cos NaN)
(tan +inf)
(tan -inf)
(tan NaN)

(define arcsinpi 1.57079632679-1.81152627246i)
(< (abs (- (sin arcsinpi) pi)) eps)
