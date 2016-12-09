;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Lambda that generates lambdas
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define gen-counter
  (lambda ()
	(let ((n 0))
	  (lambda () (set! n (+ n 1)) n))))

(gen-counter)
((gen-counter))

(define count1 (gen-counter))
(define count2 (gen-counter))
(count1)
(count1)
(count2)
(count1)
(define count3 (gen-counter))
(count2)
(count2)
(count3)
(count1)
(count2)
