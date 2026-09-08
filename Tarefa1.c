#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

typedef struct ponto {
    int x;
    int y;
} ponto;

void merge_x(ponto* v, int c, int m, int f) {
    ponto* temp = malloc((f - c + 1) * sizeof(ponto));

    int i = c;
    int j = m + 1; 
    for (int k = 0; k < (f - c + 1); k++) {
        if (i > m) {
            temp[k] = v[j++];
        }
        else if (j > f) {
            temp[k] = v[i++];
        }
        else if (v[i].x <= v[j].x) {
            temp[k] = v[i++];
        }
        else {
            temp[k] = v[j++];
        }
    }

    for (int k = 0; k < (f - c + 1); k++) {
        v[c + k] = temp[k];
    }

    free(temp);
}

void merge_y(ponto* v, int c, int m, int f) {
    ponto* temp = malloc((f - c + 1) * sizeof(ponto));

    int i = c;
    int j = m + 1; 
    for (int k = 0; k < (f - c + 1); k++) {
        if (i > m) {
            temp[k] = v[j++];
        }
        else if (j > f) {
            temp[k] = v[i++];
        }
        else if (v[i].y <= v[j].y) {
            temp[k] = v[i++];
        }
        else {
            temp[k] = v[j++];
        }
    }

    for (int k = 0; k < (f - c + 1); k++) {
        v[c + k] = temp[k];
    }

    free(temp);
}

void mergesort_x(ponto* v, int c, int f) {
    if (c >= f) return;
    mergesort_x(v, c, c + (f - c)/2);
    mergesort_x(v, c + (f - c)/2 + 1, f);
    merge_x(v, c, c + (f - c)/2, f);
}

void mergesort_y(ponto* v, int c, int f) {
    if (c >= f) return;
    mergesort_y(v, c, c + (f - c)/2);
    mergesort_y(v, c + (f - c)/2 + 1, f);
    merge_y(v, c, c + (f - c)/2, f);
}

double distance(ponto v1, ponto v2) {
    return sqrt((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
}

double minDist(ponto* v, int c, int f, ponto* res1, ponto* res2) {
    if (c >= f) return DBL_MAX;
    if (c + 1 == f) {
        *res1 = v[c];
        *res2 = v[f];
        return distance(v[c], v[f]);
    }

    int m = c + (f - c) / 2;
    ponto pil, pjl, pir, pjr;

    double dl = minDist(v, c, m, &pil, &pjl);
    double dr = minDist(v, m + 1, f, &pir, &pjr);

    double d;
    if (dl < dr) {
        d = dl;
        *res1 = pil;
        *res2 = pjl;
    } else {
        d = dr;
        *res1 = pir;
        *res2 = pjr;
    }

    ponto* faixa = malloc((f - c + 1) * sizeof(ponto));
    int tamFaixa = 0;

    for (int i = c; i <= f; i++) {
        if (fabs((double)v[i].x - v[m].x) < d) {
            faixa[tamFaixa++] = v[i];
        }
    }

    if (tamFaixa > 0) {
        // CORREÇÃO: passar tamFaixa - 1 para o limite superior
        mergesort_y(faixa, 0, tamFaixa - 1);

        for (int i = 0; i < tamFaixa; i++) {
            for (int j = i + 1; j < tamFaixa && (faixa[j].y - faixa[i].y) < d; j++) {
                double dist = distance(faixa[i], faixa[j]);
                if (dist < d) {
                    d = dist;
                    *res1 = faixa[i];
                    *res2 = faixa[j];
                }
            }
        }
    }

    free(faixa);
    return d;
}

int main() {
    int n;
    scanf("%d", &n);

    ponto* coor = malloc(n*sizeof(ponto));
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &coor[i].x, &coor[i].y);
    }
    
    mergesort_x(coor, 0, n-1);
    ponto pi, pj;
    printf("%.6f\n", minDist(coor, 0, n-1, &pi, &pj));
    printf("(%d,%d), (%d,%d)\n", pi.x, pi.y, pj.x, pj.y);
    free(coor);
    return 0;
}