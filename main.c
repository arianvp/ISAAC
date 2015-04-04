#include "rand.h"
int main()
{
  struct randctx ctx;

  int seed[4];
  seed[0] = 5;
  seed[1] = 4;
  seed[2] = 3;
  seed[3] = 2;
  isaac_init(&ctx,seed,4);

  printf("%d", isaac_next_int(&ctx));
}
