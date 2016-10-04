;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_IF=Test comment parsing"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; this is a comment

; another comment
;; with a new line char
(define min1 2)
(define min2 3)
(define min3 4)
 (if (and (< min1 min2)(< min2 min3) (or (< min2 min3) (> min1 min3)) ) "true" "false")

;; another comment

