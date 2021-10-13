(import ../build/jlzf)

(def text (string/slice (slurp "test/test.txt")))
(jlzf/compress "test/test.lzf" text)
(print (jlzf/decompress "test/test.lzf"))
