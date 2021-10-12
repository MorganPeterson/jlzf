#include <janet.h>
#include <stdio.h>
#include <errno.h>
#include "../liblzf-3.6/lzf.h"

#define COMPRESSED_MAX(n) ((((n) * 33) >> 5) + 1)
#define MAX_CHUNK 64 * 1024 - 1

int
writeFile(const char* fileName, const void* data, size_t size)
{
    size_t numberBytesWritten = 0;
    FILE* file;
 
    if(fileName && *fileName != '\0') {
      file=fopen(fileName, "wb");
      if (file && data) {
        numberBytesWritten = fwrite(data, 1, size, file);
        fclose( file );
      }
    }
    return numberBytesWritten; 
}

static Janet
cfun_compress(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 2);
  const char* fileName = janet_getcstring(argv, 0);
  const char* in_data = janet_getcstring(argv, 1);
  
  unsigned int size = strlen(in_data);
  unsigned int out_len = size * 1.04f + 8;
  void *out_data = malloc(sizeof(char) * out_len);

  if (out_data == NULL) {
    janet_panic("ENOMEM in jlzf/compress");
  }

  unsigned int l = lzf_compress(in_data, size, out_data, out_len);

  if (l == 0) {
    free(out_data);
    janet_panic("jlzf/compress failed");
  }

  int result = writeFile(fileName, out_data, l);
  free(out_data);

  if (result) {
    return janet_wrap_true();
  }
  
  return janet_wrap_false();
}

static Janet
cfun_decompress(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  const char* fileName = janet_getcstring(argv, 0);
  
  char *buffer = NULL;
  FILE* file;
  unsigned int out_len;
  size_t size;
  void* out_data;
  unsigned int l;

  if (fileName && *fileName != '\0') {
    file = fopen(fileName, "rb");
    if (file) {
      fseek(file, 0L, SEEK_END);
      long s = ftell(file);
      size = s;
      rewind(file);
      buffer = malloc(s);
      if (buffer) {
        fread(buffer, s, 1, file);
        fclose(file);
      }
    }
  }

  if (buffer != NULL) {
    out_len = COMPRESSED_MAX(MAX_CHUNK);
    out_data = malloc(out_len);
    if (out_data == NULL) {
      janet_panic("ENOMEM in jlzf/decompress");
    }
    l = lzf_decompress(buffer, size, out_data, out_len);
    if (l == 0) {
      free(out_data);
      free(buffer);
      janet_panic("E2BIG jlzf/decompress");
    }
    Janet out = janet_wrap_string(strdup(out_data));
    free(out_data);
    free(buffer);
    return out;
  }
  return janet_wrap_string("");
}

static JanetReg cfuns[] = {
  {"compress", cfun_compress, "compress data and write to file"},
  {"decompress", cfun_decompress, "Read in file and decompress data"},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY (JanetTable *env) {
  janet_cfuns(env, "jlzf", cfuns);
}
