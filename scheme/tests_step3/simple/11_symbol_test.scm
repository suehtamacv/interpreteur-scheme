;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT=Tests symbol? primitive 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define num 1)
(define str "Salut")
(define boo #f)
(define cha #\space)
(define num2 num)
(define lst '(num str boo))

(symbol? num)
(symbol? str)
(symbol? boo)
(symbol? cha)
(symbol? num2)
(symbol? lst)

(symbol? 'num)
(symbol? 'str)
(symbol? 'boo)
(symbol? 'cha)
(symbol? 'num2)
(symbol? 'lst)
(symbol? (car lst))

(symbol? (if (< 3 5) str CeciNEstPasUnSymbole))

(symbol? 'quote)
(symbol? 'define)
(symbol? 'symbol?)
(symbol? 'if)

