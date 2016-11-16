(ns clojure-noob.core
  (:gen-class))


;function will accept a variable and then add then ammount to it.
(defn adding
  [add]
  #(+ % add))
  
;function that will decrease a starting value by the number you give it.
(defn dec-maker
  [dec-by]
  #(- % dec-by))
  
;function that for every body part that starts with first will add a second - fifth body part
(defn matching-part
  [part]
  [{:name (clojure.string/replace (:name part)  #"^first-" "Second-") :size (:size part)}
  {:name (clojure.string/replace (:name part)  #"^first-" "Third-") :size (:size part)}
  {:name (clojure.string/replace (:name part)  #"^first-" "Fourth-") :size (:size part)}
  {:name (clojure.string/replace (:name part)  #"^first-" "Fifth-") :size (:size part)}]
  )

;function that carries the map of body parts and calls the function that will add more parts
(defn symmetrize-body-parts
  "Expects a seq of maps that have a :name and :size"
  [asym-body-parts]
  (loop [remaining-asym-parts asym-body-parts
         final-body-parts []]
    (if (empty? remaining-asym-parts)
      final-body-parts
      (let [[part & remaining] remaining-asym-parts]
        (recur remaining
               (into final-body-parts
                     (set (conj (matching-part part) part))))))))

;main function that will print the exercise you are on and call the appropriate functions
(defn -main
  [& args]
  ;start of exercise 2
  (println "This is exercise 2")
  (def number 5)
  (println "the starting number is " number)
  (def x (adding 5))
  (println "the new number is " (x 100))
  (println "")
  
  ;start of exercise 3
  (println "this is the start of exervise 3")
  (println "This is exercise 3")
  (def decrease (dec-maker 1))
  (println "the starting number is 10")
  (println "now the number is "(decrease 10))
  (println "")
  
  ;start of exercise5
  (println "this is the start of exercise 5")
  (def asym-alien-body-parts [{:name "head" :size 3}
                             {:name "first-eye" :size 1}
                             {:name "first-ear" :size 1}
                             {:name "mouth" :size 1}
                             {:name "nose" :size 1}
                             {:name "neck" :size 2}
                             {:name "first-shoulder" :size 3}
                             {:name "first-upper-arm" :size 3}
                             {:name "chest" :size 10}
                             {:name "back" :size 10}
                             {:name "first-forearm" :size 3}
                             {:name "abdomen" :size 6}
                             {:name "first-kidney" :size 1}
                             {:name "first-hand" :size 2}
                             {:name "first-knee" :size 2}
                             {:name "first-thigh" :size 4}
                             {:name "first-lower-leg" :size 3}
                             {:name "first-achilles" :size 1}
                             {:name "first-foot" :size 2}])
    (def alien (symmetrize-body-parts asym-alien-body-parts))
    (println alien))
                             
 
    
  


