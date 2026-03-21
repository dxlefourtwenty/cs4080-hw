### memory_hardcore.h
```
#ifndef clox_memory_hardcore_h
#define clox_memory_hardcore_h

#include "common.h"

#define HARDCORE_GROW_CAPACITY(capacity) \
  ((capacity) < 8 ? 8 : (capacity) * 2)

#define HARDCORE_GROW_ARRAY(type, pointer, oldCount, newCount) \
  (type*)hardcore_reallocate((pointer), sizeof(type) * (oldCount), \
      sizeof(type) * (newCount))

#define HARDCORE_FREE_ARRAY(type, pointer, oldCount) \
  hardcore_reallocate((pointer), sizeof(type) * (oldCount), 0)

// Call once at interpreter startup. This is the single malloc() for the allocator.
bool hardcore_allocator_init(size_t heapBytes);

// Optional cleanup for programs/tests that want to release the single heap block.
void hardcore_allocator_shutdown(void);

// Drop-in reallocate() equivalent using only the custom heap.
void* hardcore_reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif
```

### memory_hardcore.c
```
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "memory_hardcore.h"

static uint8_t* gHeap = NULL;
static size_t gHeapSize = 0;
static size_t gOffset = 0;

// the return for this is a little confusing
// basically, it just rounds 'n' to the nearest 
// multiple of 8
// it ensures every allocated block in the heap 
// starts on an 8-byte aligned address
static size_t align8(size_t n) {
  return (n + 7u) & ~7u;
}

bool hardcore_allocator_init(size_t heapBytes) {
  if (gHeap != NULL || heapBytes == 0) return false;

  gHeap = (uint8_t*)malloc(heapBytes); // called malloc here
  if (gHeap == NULL) return false;

  gHeapSize = heapBytes;
  gOffset = 0;
  return true;
}

void hardcore_allocator_shutdown(void) {
  // In strict hardcore mode, no free() calls are allowed.
  // We keep the global reference alive so leak checkers treat it as reachable.
  // this function is defined for free() calls if constraints are
  // ever to be changed
}

void* hardcore_reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (gHeap == NULL || gHeapSize == 0 || newSize == 0) {
    return NULL;
  }

  size_t wanted = align8(newSize);
  if (gOffset + wanted > gHeapSize) {
    return NULL;
  }

  void* out = gHeap + gOffset;
  gOffset += wanted;

  if (pointer != NULL) {
    size_t copyBytes = oldSize < newSize ? oldSize : newSize;
    memcpy(out, pointer, copyBytes);
  }

  return out;
}
```

### main_hardcore.c
```
#include <assert.h>
#include <stdio.h>

#include "memory_hardcore.h"

int main(void) {
  assert(hardcore_allocator_init(256));
  printf("init ok (256 bytes)\n");

  int* a = (int*)hardcore_reallocate(NULL, 0, sizeof(int) * 4);
  assert(a != NULL);
  for (int i = 0; i < 4; i++) a[i] = i + 1;
  printf("alloc 4 ints at %p\n", (void*)a);

  int* b = (int*)hardcore_reallocate(a, sizeof(int) * 4, sizeof(int) * 8);
  assert(b != NULL);
  assert(b[0] == 1 && b[1] == 2 && b[2] == 3 && b[3] == 4);
  printf("grow to 8 ints at %p (copied old values)\n", (void*)b);

  void* freed = hardcore_reallocate(b, sizeof(int) * 8, 0);
  assert(freed == NULL);
  printf("free request returned NULL (no reclaim in bump allocator)\n");

  void* tooBig = hardcore_reallocate(NULL, 0, 10000);
  assert(tooBig == NULL);
  printf("oversized alloc returned NULL\n");

  hardcore_allocator_shutdown();
  printf("hardcore bump allocator demo passed\n");
  return 0;
}
```
