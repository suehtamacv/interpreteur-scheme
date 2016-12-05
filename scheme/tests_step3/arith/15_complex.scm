;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests complex functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define z 3+4i)
(define pi 3.14159265359)

(real-part 0)
(real-part 1)
(real-part 0+1i)
(real-part NaN)
(real-part +inf)
(real-part -inf)
(real-part z)

(imag-part 0)
(imag-part 1)
(imag-part 0+1i)
(imag-part NaN)
(imag-part +inf)
(imag-part -inf)
(imag-part z)

(- z (make-rectangular 3 4)) 
(make-rectangular 1 -3)
(make-rectangular NaN +inf)
(make-rectangular +inf -inf)
(make-rectangular NaN 3)
(real-part (make-rectangular NaN 3))
(imag-part (make-rectangular NaN 3))

(magnitude 0)
(magnitude 1)
(magnitude +inf)
(magnitude -inf)
(magnitude z)

(angle 0)
(angle 1)
(/ (angle -1) pi)
(angle (make-rectangular +inf 2))
(/ (angle (make-rectangular -inf 2)) pi)
(/ (angle (make-rectangular 0 1)) pi)
(/ (angle (make-rectangular 0 -1)) pi)
(/ (angle (make-rectangular 123 -inf)) pi)
