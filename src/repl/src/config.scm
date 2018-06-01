(define (* a b)
  (if (< b 1)
      0
      (+ a (* a (- b 1)))))

(define (max a b)
  (if (< a b)
      b
      a))
