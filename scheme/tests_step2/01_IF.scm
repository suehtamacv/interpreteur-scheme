;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_IF=Test comment parsing"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; this is a comment

; another comment
;; with a new line char

(define x -4)
(if (< x 0) (- x) x )

;; another comment
