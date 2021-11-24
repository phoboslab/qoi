#define QOI_IMPLEMENTATION
#define MAX_ALLOC (1 * 1024 * 1024)
#include "qoi.h"
#include <stddef.h>
#include <stdint.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  int w, h;
  if (size < 4) {
    return 0;
  }

  void* decoded = qoi_decode((void*)(data + 4), (int)(size - 4), &w, &h, *((int *)data));
  if (decoded != NULL) {
	  QOI_FREE(decoded);
  }
  return 0;
}
