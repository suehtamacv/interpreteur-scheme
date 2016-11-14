;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=FAIL
; TEST_COMMENT=Tests "remainder" in these case wrong type
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(remainder 2 1.1)
(remainder 2 3+4i)
(remainder 2 +inf)
(remainder 2 -inf)
(remainder 2 NaN)
(remainder 1.2 3)
(remainder 10+2i 3)
(remainder +inf 2)
(remainder -inf 2)
(remainder NaN 1)
(remainder +2 1.1)
(remainder +2 1+2i)
(remainder +2 +inf)
(remainder +2 -inf)
(remainder +2 NaN)
(remainder 1.1 +2)
(remainder 1+2i +2)
(remainder +inf +2)
(remainder -inf +2)
(remainder NaN +2)
(remainder -2 1.1)
(remainder -2 1+2i)
(remainder -2 +inf)
(remainder -2 -inf)
(remainder -2 NaN
(remainder 1.1 -2) 
(remainder 1+2i -2)
(remainder +inf -2)
(remainder -inf -2)
(remainder NaN -2)
(remainder 1.2 3.4) 
(remainder 1.2 3+4i)
(remainder 1.2 +inf)
(remainder 1.2 -inf)
(remainder 1.2 NaN)
(remainder 3+4i 1.2)
(remainder +inf 1.2)
(remainder -inf 1.2)
(remainder NaN 1.2)
(remainder 1+2i 3+4i)
(remainder 1+2i +inf)
(remainder 1+2i -inf)
(remainder 1+2i NaN)
(remainder +inf 1+2i)
(remainder -inf 1+2i)
(remainder NaN 1+2i)
(remainder +inf -inf)
(remainder +inf NaN)
(remainder -inf +inf)
(remainder NaN +inf)
(remainder -inf NaN)
(remainder NaN -inf)
(remainder +inf +inf)
(remainder -inf -inf)
(remainder NaN NaN) 
