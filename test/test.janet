(import ../build/jlzf)

(def text (string (slurp "README.md")))

(assert (jlzf/compress "test/test.lzf" text) "compress and write file")

(def result (string (jlzf/decompress "test/test.lzf")))

(assert (= (length text) 1070) "compressed size")
(assert (= (length result) 1089) "decompressed size")

(os/rm "test/test.lzf")
