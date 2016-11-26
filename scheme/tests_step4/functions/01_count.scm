;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Count function 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define total -10)
(define increment 123)

(define count
  ((lambda (total)
	 (lambda (increment)
	   (set! total (+ total increment))
	   total)) 0))

(count 1)
(count 10)
(count 1.1)
(count -1.1)
(count 3)
(count 1+2j)
(count (make-rectangular 0 (- (imag-part (count 0)))))

total
increment
