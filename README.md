# JLZF

A Janet native module that allows for use of [liblzf](http://software.schmorp.de/pkg/liblzf.html)

Liblzf is a very small data compression library.

## liblzf

All the files need are already bundled inside this repository. No need to download.

Liblzf has it's own very permissive license that can be found in the liblzf-3.6 directory.

There are no known patents involved.

Original files can be found here: [dist.schmorp.de](http://dist.schmorp.de/liblzf/)

## install
```sh
sudo jpm install https://github.com/MorganPeterson/jlzf
```

## example

The test.janet file in the test directory should give you some idea of the workings.

```janet
(import jlzf)

(def text (string/slice (slurp "test.txt")))

# compress returns true for success; otherwise returns false.
(cond (jlzf/compress "test.lzf" text)
  (print "success")
  (print "failure"))

# decompress returns a string
(print (jlzf/decompress "test.lzf"))
```

## Caveat

Compression fails on files that are 17 bytes or less. If you have to compress something that small... well, may the gods help you.
