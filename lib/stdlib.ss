(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))
(define (caaar x) (car (car (car x))))
(define (caadr x) (car (car (cdr x))))
(define (cadar x) (car (cdr (car x))))
(define (caddr x) (car (cdr (cdr x))))
(define (cdaar x) (cdr (car (car x))))
(define (cdadr x) (cdr (car (cdr x))))
(define (cddar x) (cdr (cdr (car x))))
(define (cdddr x) (cdr (cdr (cdr x))))
(define (caaaar x) (car (car (car (car x)))))
(define (caaadr x) (car (car (car (cdr x)))))
(define (caadar x) (car (car (cdr (car x)))))
(define (caaddr x) (car (car (cdr (cdr x)))))
(define (cadaar x) (car (cdr (car (car x)))))
(define (cadadr x) (car (cdr (car (cdr x)))))
(define (caddar x) (car (cdr (cdr (car x)))))
(define (cadddr x) (car (cdr (cdr (cdr x)))))
(define (cdaaar x) (cdr (car (car (car x)))))
(define (cdaadr x) (cdr (car (car (cdr x)))))
(define (cdadar x) (cdr (car (cdr (car x)))))
(define (cdaddr x) (cdr (car (cdr (cdr x)))))
(define (cddaar x) (cdr (cdr (car (car x)))))
(define (cddadr x) (cdr (cdr (car (cdr x)))))
(define (cdddar x) (cdr (cdr (cdr (car x)))))
(define (cddddr x) (cdr (cdr (cdr (cdr x)))))

(define first car)
(define rest cdr)
(define empty? null?)
(define list? pair?)
(define cons? pair?)
(define empty '())
(define equal? eq?)

(define (not b) (if b #f #t))
(define true #t)
(define false #f)

(define (add1 n) (+ n 1))
(define (sub1 n) (- n 1))
(define (sqr n) (* n n))

(define (positive? n) (> n 0))
(define (negative? n) (< n 0))
(define (zero? n) (= n 0))
(define (even? n) (= (% n 2) 0))
(define (odd? n) (not (even? n)))

(define (foldr f init l)
	(if (empty? l)
		init
		(f (first l) (foldr f init (rest l)))))
		
(define (foldl f init l)
	(if (empty? l)
		init
		(foldl f (f init (first l)) (rest l))))

(define fold foldl)
(define reduce fold)

(define (map f l)
  (foldr (lambda (v l) (cons (f v) l)) empty l))
  
(define (filter f l)
  (foldr (lambda (v l)
           (cond
             ((not (f v)) l)
             (else (cons v l)))) empty l))
             
(define (flip f)
	(lambda (x y) (f y x)))
             
(define append 
	(lambda (x y)
		(foldr cons y x)))
             
(define (flatten l)
  (filter (lambda (x) (not (empty? x)))
           (cond
             ((empty? l) empty)
             ((cons? (first l)) (append (flatten (first l)) (flatten (rest l))))
             (else (cons (first l) (flatten (rest l)))))))

(define (reverse l)
	(foldl (flip cons) empty l))

(define (count l)
	(cond
		((empty? l) 0)
		(else (add1 (count (rest l))))))
		
(define (member m l)
	(cond 
		((empty? l) false)
		((eq? m (first l)) true)
		(else (member m (rest l)))))
		
(define (take l n)
	(cond
		((zero? n) empty)
		(else  (cond
					((empty? l) empty)
					(else (cons (first l) (take (rest l) (sub1 n))))))))
					
(define (drop l n) 
	(take (reverse l) n))
	
(define (build-list n)
	(if (> n 0) (append (build-list (sub1 n)) (list n))
				empty))

(define bl build-list)

(define test (list 1 2 3 4 5 6 7 8 9
				   10 11 12 13 14 15
				   16 17 18 19 20 21
				   22 23 24 25 26 27
				   28 29 30 31 32 33
				   34 35 36 37 38 39
				   40 41 42 43 44 45
				   46 47 48 49 50 51
				   52 53 54 55 56 57
				   58 59 60 61 62 63
				   64 65 66 67 68 69))
				   
(define sl string->list)
(define ls list->string)
(define clist (list #\a #\b #\c))
				   
	
'stdlib-loaded
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		