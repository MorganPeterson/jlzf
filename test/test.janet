(import ../build/jlzf)

(def text "The quick brown fox jumps over the lazy dog.")

(assert (jlzf/compress "test/test.lzf" text) "compress and write file")

(def result (jlzf/decompress "test/test.lzf"))

(assert (= result text) "decompressed text matches original text")
