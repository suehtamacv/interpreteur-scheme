;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests conversion function between symbol and string
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;(symbol->string _)
(symbol->string 'test)
(string? (symbol->string 'check))
(define x (symbol->string 'test))
(string? x)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;(string->symbol _)
(string->symbol "test")
(symbol? (string->symbol "test"))
