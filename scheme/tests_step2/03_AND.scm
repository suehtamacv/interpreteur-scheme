;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_IF=Test comment parsing"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; this is a comment

; another comment
;; with a new line char

(and (< 2 8) (number? "yes") (+ 1 (/ 2 0)))

;; another comment
