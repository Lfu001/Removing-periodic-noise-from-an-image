#pragma warning(disable : 4996)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "dft_idft_2d.h"
#include "filtering.h"

#define FILTER_SIZE 1


int main() {
    int x, y, u, v;
    Complex f[SIZE][SIZE];        /* input image for image processing */
    Complex g[SIZE][SIZE];        /* output image for image processing */
    unsigned char f1[SIZE][SIZE]; /* input image */
    unsigned char g1[SIZE][SIZE]; /* output image */
    FILE *fp1, *fp2;


    fp1 = fopen("noiseimage.raw", "rb");
    fread(f1, 1, SIZE * SIZE, fp1);
    fclose(fp1);

    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE; y++) {
            f[x][y].r = (double)f1[x][y];
            f[x][y].i = 0.0;
            g[x][y] = (Complex){0, 0};
        }
    }

    dft_idft_2d(g, f, DFT);

    // power spectrum image
    double p[SIZE][SIZE];
    for (u = 0; u < SIZE; u++) {
        for (v = 0; v < SIZE; v++) {
            g1[(u + 128) % SIZE][(v + 128) % SIZE] = (p[u][v] = 18.2 * log10(1 + (g[u][v].r)*(g[u][v].r) + (g[u][v].i)*(g[u][v].i))) > 255 ? 255 : (unsigned char)p[u][v];
        }
    }
    fp2 = fopen("power_spectrum.raw", "wb");
    fwrite(g1, sizeof(unsigned char), SIZE * SIZE, fp2);
    fclose(fp2);

    // real part of the Fourier transformed image
    for (u = 0; u < SIZE; u++) {
        for (v = 0; v < SIZE; v++) {
            g1[(u + 128) % SIZE][(v + 128) % SIZE] = (p[u][v] = 36.4 * log10(1 + abs(g[u][v].r))) > 255 ? 255 : (unsigned char)p[u][v];
        }
    }
    fp2 = fopen("Re(F(w)).raw", "wb");
    fwrite(g1, sizeof(unsigned char), SIZE * SIZE, fp2);
    fclose(fp2);

    // imaginary part of the Fourier transformed image
    for (u = 0; u < SIZE; u++) {
        for (v = 0; v < SIZE; v++) {
            g1[(u + 128) % SIZE][(v + 128) % SIZE] = (p[u][v] = 36.4 * log10(1 + abs(g[u][v].i))) > 255 ? 255 : (unsigned char)p[u][v];
        }
    }
    fp2 = fopen("Im(F(w)).raw", "wb");
    fwrite(g1, sizeof(unsigned char), SIZE * SIZE, fp2);
    fclose(fp2);
    

    //////////////////////////////////////////////////////////////////////////////////

    // coordinates of the noises
    Coordinate noise[] = {{128, 0}, {128, 64}, {128, 192}};

    // sorting Fourier transformed images
    for (u = 0; u < SIZE; u++) {
        for (v = 0; v < SIZE; v++) {
            f[(u + 128) % SIZE][(v + 128) % SIZE] = g[u][v];
        }
    }

    int n = sizeof(noise) / sizeof(Coordinate);

    filtering(g, f, noise, n, FILTER_SIZE);

    // power spectrum image after filtering the image
    for (u = 0; u < SIZE; u++) {
        for (v = 0; v < SIZE; v++) {
            g1[u][v] = (p[u][v] = 18.2 * log10(1 + (g[u][v].r) * (g[u][v].r) + (g[u][v].i) * (g[u][v].i))) > 255 ? 255 : (unsigned char)p[u][v];
        }
    }
    fp2 = fopen("power_spectrum_filtered_1.raw", "wb");
    fwrite(g1, sizeof(unsigned char), SIZE * SIZE, fp2);
    fclose(fp2);
    

    //////////////////////////////////////////////////////////////////////////////////

    // restore the order of the frequency image
    for (u = 0; u < SIZE; u++) {
        for (v = 0; v < SIZE; v++) {
            f[(u + 128) % SIZE][(v + 128) % SIZE] = g[u][v];
        }
    }

    dft_idft_2d(g, f, IDFT);

    double t;

    // image with noise removed
    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE; y++) {
            g1[x][y] = (unsigned char)((t = g[x][y].r) > 255 ? 255 : (t < 0 ? 0 : t));
        }
    }
    fp2 = fopen("noise_removed_1.raw", "wb");
    fwrite(g1, sizeof(unsigned char), SIZE * SIZE, fp2);
    fclose(fp2);
}
