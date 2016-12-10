;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=eq?
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(eq? 'a 'a)
(eq? (list 'a) (list 'a))
(eq? '() '())
(eq? car car)
(let ((x '(a)))
  (eq? x x))
(let ((p (lambda (x) x)))
  (eq? p p))
