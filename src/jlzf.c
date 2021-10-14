#include <janet.h>
#include <stdio.h>
#include <errno.h>
#include "../liblzf-3.6/lzf.h"

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
  const void *const in_data = janet_getcstring(argv, 1);
  unsigned int in_len = strlen(in_data);
  
  unsigned int out_len = (in_len * 1.04) - 1;
  void *out_data = janet_smalloc(out_len);

  if (out_data == NULL) {
    janet_sfree(out_data);
    janet_panic("ENOMEM");
  }

  unsigned int l = lzf_compress(in_data, in_len, out_data, out_len);

  if (l == 0) {
    janet_sfree(out_data);
    janet_panic("unable to compress data");
  }

  int result = writeFile(fileName, out_data, l);
  janet_sfree(out_data);

  if (result) {
    return janet_wrap_true();
  }
  
  return janet_wrap_false();
}

static Janet
cfun_decompress(int32_t argc, Janet *argv) {
  janet_fixarity(argc, 1);
  const char* fileName = janet_getcstring(argv, 0);
  
  FILE* file;
  unsigned int size;
  unsigned int out_len;
  void* out_data;
  unsigned int l;

  if (!fileName || *fileName == '\0') {
    janet_panic("no file name provided");
  }

  file = fopen(fileName, "rb");
  if (!file) {
    janet_panic("unable to open file for decompression");
  }

  fseek(file, 0L, SEEK_END);
  size = ftell(file);
  rewind(file);
  void *const in_data = janet_smalloc(size);
  if (in_data) {
    fread(in_data, size, 1, file);
    fclose(file);
  }

  if (in_data == NULL) {
    janet_sfree(in_data);
    janet_panic("no data read in from file");
  }

  out_len = (size * 5.0f) * 2.0f;
  out_data = janet_smalloc(sizeof(char) * out_len);

  if (out_data == NULL) {
    janet_sfree(in_data);
    janet_panic("ENOMEM");
  }

  l = lzf_decompress((const void *const)in_data, size, out_data, out_len);

  if (l == 0) {
    janet_sfree(out_data);
    janet_sfree(in_data);
    janet_panic("E2BIG");
  }

  Janet out = janet_wrap_string(strdup((char *)out_data));

  janet_sfree(out_data);
  janet_sfree(in_data);

  return out;
}

static JanetReg cfuns[] = {
  {"compress", cfun_compress, "compress data and write to file"},
  {"decompress", cfun_decompress, "Read in file and decompress data"},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY (JanetTable *env) {
  janet_cfuns(env, "jlzf", cfuns);
}
