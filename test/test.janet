(import ../build/jlzf)

(def text (string/slice (slurp "test.txt")))
(jlzf/compress "test.lzf" text)
(print (jlzf/decompress "test.lzf"))
