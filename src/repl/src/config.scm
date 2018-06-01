(define * (lambda (a b)
            (if (< b 1)
                0
                (+ a (* a (- b 1))))))

(define max (lambda (a b)
              (if (< a b)
                  b
                  a)))

(define myplus (lambda (x y)
                 (+ x y)))
