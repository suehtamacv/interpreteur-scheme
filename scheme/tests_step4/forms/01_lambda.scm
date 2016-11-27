;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests lambda scheme 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Simple test lambda
((lambda (temp) (* temp temp)) 10)
((lambda (x y) (+ x y))
   1 2)
;; Calculation square
 (define square (lambda (n)(* n n)))
(square 5)
;; Calcualation the value absolute
(define absolute (lambda (n) (if (< n 0)(- 0 n)n)))
(absolute -4)
;; Copy list
(define list-copy
  (lambda (ls)
    (if  (null? ls)
       '()
       (cons (car ls)
            (list-copy (cdr ls))))))
(list-copy '(4 5 '(6 7 '(8 9))))
(list-copy '())
