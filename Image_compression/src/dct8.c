/*
 * dct8.c
 *
 */

#include "dct8.h"
#include <math.h>
#define PI 3.14159265358979323846

static float C[8][8];
static int inited = 0;

void dct8x8_init(void)
{
    const float N = 8.0;
    for(int u = 0; u < 8; u++)
    {
        float a = (u == 0) ? sqrtf(1.0/N) : sqrtf(2.0/N);
        for(int x = 0; x < 8; x++)
        {
            C[u][x] = a * cosf((PI/N) * ((float)x + 0.5) * (float)u);
        }
    }
    inited = 1;
}

/* F(u,v) = sum_y sum_x C(u,x) * f(y,x) * C(v,y)  => out[u*8+v] */
void dct8x8(const s16 in[64], float out[64])
{
    if(!inited) dct8x8_init();

    float tmp[8][8];  // tmp[u][y] = sum_x C[u][x] * f[y][x]

    for(int u = 0; u < 8; u++){
        for(int y = 0; y < 8; y++){
            float s = 0.0f;
            for(int x = 0; x < 8; x++){
                float fx = (float)in[y*8 + x];
                s += C[u][x] * fx;
            }
            tmp[u][y] = s;
        }
    }

    for(int u = 0; u < 8; u++){
        for(int v = 0; v < 8; v++){
            float s = 0.0f;
            for(int y = 0; y < 8; y++){
                s += tmp[u][y] * C[v][y];
            }
            out[u*8 + v] = s;
        }
    }
}
