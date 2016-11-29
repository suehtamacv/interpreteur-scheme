(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))
(define (caaar x) (car (caar x)))
(define (cdaar x) (cdr (caar x)))
(define (caadr x) (car (cadr x)))
(define (cdadr x) (cdr (cadr x)))
(define (cadar x) (car (cdar x)))
(define (cddar x) (cdr (cdar x)))
(define (caddr x) (car (cddr x)))
(define (cdddr x) (cdr (cddr x)))
(define (map proc items) 
  (if (null? items)
	  '()
	  (cons
	   (proc (car items))
	   (map proc (cdr items)))))
(define sum
  (lambda (l)
	(if (null? l)
		0
		(+ (car l) (sum (cdr l))))))
(define product
  (lambda (l)
	(if (null? l)
		1
		(* (car l) (product (cdr l))))))
(define length
  (lambda (l)
	(if (null? l)
		0
		(+ 1 (length (cdr l))))))
(define reverse
  (lambda (l)
	(if (null? l)
		'()
		(append (reverse (cdr l)) (list (car l))))))
