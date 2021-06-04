#define SIZE 256

typedef struct Coordinate { unsigned char x, y; } Coordinate;

void filtering(Complex filtered[SIZE][SIZE], Complex unfiltered[SIZE][SIZE], Coordinate c[], int numOfNoisePoint, unsigned char range) {
    int i, j, k, u, v;

    for (u = 0; u < SIZE; u++) {
        for (v = 0; v < SIZE; v++) {
            filtered[u][v] = unfiltered[u][v];
        }
    }

    for (i = 0; i < numOfNoisePoint; i++) {
        for (j = -1 * range + 1; j < range; j++) {
            for (k = -1 * range + 1; k < range; k++) {
                u = (c[i].x + j);
                v = (c[i].y + k);
                if (!((0 <= u) && (u < SIZE) && (0 <= v) && (v < SIZE))) continue;
                
                filtered[v][u] = (Complex){0, 0};
            }
        }
    }
}