// chunk refers to sequences of bytecode

#ifndef clox_chunk_h

#define clox_chunk_h

#include "common.h"

// in our bytecode format, each instruction has a one-byte
// operation code (opcode) <-- this number controls what kind
// of instruction we're dealing with (e.g. add, subtract, look up variables, etc.)
typedef enum {
  OP_RETURN,
} OpCode;

// at the moment, this is simply a wrapper around an array of bytes
// this is dynamic because we don't know how big the array needs to be before we start compiling a chunk
typedef struct {
  uint8_t* code;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);

#endif

