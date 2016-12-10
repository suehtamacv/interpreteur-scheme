;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


(define reverse-subtract 
(lambda (x y) (- y x))) 

(reverse-subtract 10 20)

(define mul_10
(let ((x 10))
(lambda (y) (* x y))))

(mul_10 10)


(define count
((lambda (total)
(lambda (increment)
(set! total (+ total increment)) total)) 0))
(count 12)
(count 24)
(count 2)
