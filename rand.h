#ifndef ISAAC_RAND
#define ISAAC_RAND

#include <stdint.h>
#include <stdlib.h>

struct randctx
{
  uint32_t results[256];
  uint32_t memory[256];
  uint32_t aa;
  uint32_t bb;
  uint32_t cc;
  size_t position;
};


int isaac_init(struct randctx *, int32_t *, size_t);
int isaac_next_int(struct randctx *);

#endif
