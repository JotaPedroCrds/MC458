/*
    Nome: João Pedro Cardoso de Paula 
    RA: 246527 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

typedef struct ponto {
    int x;
    int y;
    int id;
} ponto;

typedef struct {
    ponto p1;
    ponto p2;
} par;

double distance(ponto v1, ponto v2) {
    double dx = (double)v1.x - v2.x;
    double dy = (double)v1.y - v2.y;
    return sqrt(dx * dx + dy * dy);
}

void adicionar_ou_atualizar_par(ponto a, ponto b, par* pares_minimos, double* d, int* qtd_pares) {
    double dist = distance(a, b);

    if (a.id > b.id) {
        ponto temp = a;
        a = b;
        b = temp;
    }

    // Achou uma distância ESTRITAMENTE menor: limpa a lista e reinicia
    if (dist < *d - 1e-9) {
        *d = dist;
        *qtd_pares = 0;
        pares_minimos[*qtd_pares].p1 = a;
        pares_minimos[*qtd_pares].p2 = b;
        (*qtd_pares)++;
        return;
    } 
    // Empate na distância mínima: adiciona à lista existente
    else if ((dist - *d > -1e-9) && (dist - *d < 1e-9)) {
        for (int i = 0; i < *qtd_pares; i++) {
            if (pares_minimos[i].p1.id == a.id && pares_minimos[i].p2.id == b.id) {
                return;
            }
        }
        pares_minimos[*qtd_pares].p1 = a;
        pares_minimos[*qtd_pares].p2 = b;
        (*qtd_pares)++;
    }
}

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

void minDist(ponto* v, int c, int f, par* pares_minimos, double* d, int* qtd_pares) {
    if (c >= f) return;
    if (f - c <= 2) {
        for (int i = c; i <= f; i++) {
            for (int j = i + 1; j <= f; j++) {
                adicionar_ou_atualizar_par(v[i], v[j], pares_minimos, d, qtd_pares);
            }
        }
        return;
    }

    int m = c + (f - c) / 2;

    minDist(v, c, m, pares_minimos, d, qtd_pares);
    minDist(v, m + 1, f, pares_minimos, d, qtd_pares);

    ponto* faixa = malloc((f - c + 1) * sizeof(ponto));
    int tamFaixa = 0;

    for (int i = c; i <= f; i++) {
        if (fabs((double)v[i].x - v[m].x) < *d) {
            faixa[tamFaixa++] = v[i];
        }
    }

    if (tamFaixa > 0) {
        // CORREÇÃO: passar tamFaixa - 1 para o limite superior
        mergesort_y(faixa, 0, tamFaixa - 1);

        for (int i = 0; i < tamFaixa; i++) {
            for (int j = i + 1; j < tamFaixa && (faixa[j].y - faixa[i].y) < *d; j++) {
                adicionar_ou_atualizar_par(faixa[i], faixa[j], pares_minimos, d, qtd_pares);
            }
        }
    }

    free(faixa);
    return;
}

int main() {
    int n;
    scanf("%d", &n);

    int max = (n*(n-1))/2;
    par* pares_minimos = malloc(max*sizeof(par));
    int qtd_pares = 0;
    double d = DBL_MAX;

    ponto* coor = malloc(n*sizeof(ponto));
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &coor[i].x, &coor[i].y);
        coor[i].id = i;
    }
    
    mergesort_x(coor, 0, n-1);
    minDist(coor, 0, n - 1, pares_minimos, &d, &qtd_pares);
    for (int j = 0; j < qtd_pares; j++)
    {
        printf("(%d,%d)\n", pares_minimos[j].p1.id, pares_minimos[j].p2.id);
    }
    
    free(coor);
    return 0;
}