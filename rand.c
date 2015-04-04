#include "rand.h"
#include <string.h>

#define MIX(a,b,c,d,e,f,g,h) \
{ \
   a ^= b << 11; d += a; b += c; \
   b ^= c >> 2;  e += b; c += d; \
   c ^= d << 8;  f += c; d += e; \
   d ^= e >> 16; g += d; e += f; \
   e ^= f << 10; h += e; f += g; \
   f ^= g >> 4;  a += f; g += h; \
   g ^= h << 8;  b += g; h += a; \
   h ^= a >> 9;  c += h; a += b; \
}


static void isaac(struct randctx *ctx)
{
  register uint32_t i,x,y;
  ctx->cc++;
  ctx->bb += ctx->cc;

  for (i = 0; i < 256; i++)
  {
    x = ctx->memory[i];
    switch (i%4)
    {
      case 0:
        ctx->aa = ctx->aa ^ (ctx->aa << 13);
        break;
      case 1:
        ctx->aa = ctx->aa ^ (ctx->aa >> 6);
        break;
      case 2:
        ctx->aa = ctx->aa ^ (ctx->aa << 2);
        break;
      case 3:
        ctx->aa = ctx->aa ^ (ctx->aa >> 16);
        break;
    }
    ctx->aa = ctx->memory[(i+128) % 256] + ctx->aa;
    ctx->memory[i] = y = ctx->memory[(x>>2)%256] + ctx->aa + ctx->bb;
    ctx->results[i] = ctx->bb = ctx->memory[(y>>10)%256]+x;
  }
}


extern int
isaac_init(struct randctx *ctx, int32_t *seed, size_t cnt)
{
  int i;
  register uint32_t a,b,c,d,e,f,g,h;

  memset(ctx, 0, sizeof(struct randctx));
  if (cnt > 255)
  {
    return 0;
  }

  for (i = 0; i < cnt; i++)
  {
    ctx->results[i] = seed[i];
  }

  a=b=c=d=e=f=g=h=0x9e3779b9;

  for (i = 0; i < 4; i++)
  {
    MIX(a,b,c,d,e,f,g,h);
  }

  for (i = 0; i < 256; i+= 8)
  {
    a += ctx->results[i + 0];
    b += ctx->results[i + 1];
    c += ctx->results[i + 2];
    d += ctx->results[i + 3];
    e += ctx->results[i + 4];
    f += ctx->results[i + 5];
    g += ctx->results[i + 6];
    h += ctx->results[i + 7];

    MIX(a,b,c,d,e,f,g,h);

    ctx->memory[i + 0] = a;
    ctx->memory[i + 1] = b;
    ctx->memory[i + 2] = c;
    ctx->memory[i + 3] = d;
    ctx->memory[i + 4] = e;
    ctx->memory[i + 5] = f;
    ctx->memory[i + 6] = g;
    ctx->memory[i + 7] = h;
  }

  for (i = 0; i < 256; i+= 8)
  {
    a += ctx->memory[i + 0];
    b += ctx->memory[i + 1];
    c += ctx->memory[i + 2];
    d += ctx->memory[i + 3];
    e += ctx->memory[i + 4];
    f += ctx->memory[i + 5];
    g += ctx->memory[i + 6];
    h += ctx->memory[i + 7];

    MIX(a,b,c,d,e,f,g,h);

    ctx->memory[i + 0] = a;
    ctx->memory[i + 1] = b;
    ctx->memory[i + 2] = c;
    ctx->memory[i + 3] = d;
    ctx->memory[i + 4] = e;
    ctx->memory[i + 5] = f;
    ctx->memory[i + 6] = g;
    ctx->memory[i + 7] = h;
  }

  isaac(ctx);
  ctx->position = 256;
  return 1;
}

extern int32_t
isaac_next_int(struct randctx *ctx)
{
  if (ctx->position-- == 0)
  {
    isaac(ctx);
    ctx->position = 255;
  }
  return ctx->results[ctx->position];
}
