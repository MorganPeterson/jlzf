(import jlzf)

(jlzf/compress "test.lzf" (string (slurp "test.txt")))
(def result (jlzf/decompress "test.lzf"))
(print (length result))
(print result)
(os/rm "test.lzf")
