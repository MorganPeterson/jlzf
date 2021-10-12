(declare-project
  :name "jlzf"
  :author "Morgan Peterson <lastyearsmodel@gmail.com"
  :description "Compression binding for liblzf"
  :license "MIT"
  :url "https://github.com/MorganPeterson/jlzf"
  :repo "https://github.com/MorganPeterson/jlzf")

(def cflags
  '["-std=c99"
    "-Wall"
    "-Werror"
    "-D_POSIX_C_SOURCE=200809L"
    "-D_XOPEN_SOURCE"])

(def lib-string "liblzf-3.6")

(defn configure []
  (if (not (=
       (os/stat
         (string lib-string "/config.h")
         :mode)
       :file))
    (do
      (os/cd lib-string)
      (os/execute ["./configure"] :p)
      (os/cd ".."))))

(defn check-liblzf []
  (if (= (os/stat "liblzf-3.6" :mode) :directory)
    (configure)))

(check-liblzf)

(declare-native
  :name "jlzf"
  :cflags cflags
  :source @["src/jlzf.c"
           "liblzf-3.6/lzf.c"
           "liblzf-3.6/lzf_c.c"
           "liblzf-3.6/lzf_d.c"])

