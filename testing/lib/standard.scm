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
