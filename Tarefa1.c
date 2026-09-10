/*
    Nome: João Pedro Cardoso de Paula 
    RA: 246527 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

// Estrutura para conter um ponto cartesiano e seu índice inicial
typedef struct ponto {
    int x;
    int y;
    int id;
} ponto;

// Estrutura para conter um par de pontos
typedef struct {
    ponto p1;
    ponto p2;
} par;

/*
Função que calcula a distância entre dois pontos
*/
long long distance(ponto v1, ponto v2) {
    long long dx = (long long) (v1.x - v2.x);
    long long dy = (long long) (v1.y - v2.y);
    return (long long) (dx * dx) + (dy * dy);
}

/* 
Como é preciso imprimir todos os pontos que tenham a distância mínima, esta função compara uma nova distância
com a menor distância encontrada até o momento, se a nova for menor, esta será a menor distância encontrada até o momento,
se forem iguais, adiciona o novo par aos pares que tenham esse mesmo valor.
Caso a nova distancia seja maior, não executa nada.
*/
void adicionar_ou_atualizar_par(ponto a, ponto b, par** pares_minimos, int* cap, long long* d, int* qtd_pares) {
    long long dist = distance(a, b); // nova distância a ser avaliada

    if (a.id > b.id) { // inverte os indices para obedecerem a ordem crescente
        ponto temp = a;
        a = b;
        b = temp;
    }

    
    if (dist < *d) {
        *d = dist; // atualiza a menor distancia
        *qtd_pares = 0; // reinicia a lista
        (*pares_minimos)[*qtd_pares].p1 = a;
        (*pares_minimos)[*qtd_pares].p2 = b;
        (*qtd_pares)++;
        return;
    } 

    else if (dist > *d) { // ignoramos se a nova distância for maior
        return;
    }

    // realocamos a lista se necessário
    if (*qtd_pares >= *cap) {
        *cap *= 2;
        *pares_minimos = realloc(*pares_minimos, (*cap) * sizeof(par));
    }

    // adicionamos o par e aumentamos a lista
    (*pares_minimos)[*qtd_pares].p1 = a;
    (*pares_minimos)[*qtd_pares].p2 = b;
    (*qtd_pares)++;
}

/*
mergesort para os vetores de pontos, onde eixo 0 corresponde a ordenação dos pontos pelas coordenadas x e eixo 1 pelas coordenadas y
*/
void merge(ponto* v, int c, int m, int f, int eixo, ponto* temp) {
    int i = c;
    int j = m + 1;
    if (eixo == 0) { 
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
    }
    else {
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
    }

    for (int k = 0; k < (f - c + 1); k++) {
        v[c + k] = temp[k];
    }
}
void mergesort(ponto* v, int c, int f, int eixo, ponto* temp) {
    if (c >= f) return;
    mergesort(v, c, c + (f - c)/2, eixo, temp);
    mergesort(v, c + (f - c)/2 + 1, f, eixo, temp);
    merge(v, c, c + (f - c)/2, f, eixo, temp);
}

/*
mergesort para os vetores de pares
*/
void merge_pares(par* v, int c, int m, int f, par* temp) {
    int i = c;
    int j = m + 1;

    for (int k = 0; k < (f - c + 1); k++) {
        if (i > m) {
            temp[k] = v[j++];
        }
        else if (j > f) {
            temp[k] = v[i++];
        }
        else if (v[i].p1.id < v[j].p1.id) {
            temp[k] = v[i++];
        }
        else if (v[i].p1.id == v[j].p1.id) {
            if (v[i].p2.id < v[j].p2.id)
                temp[k] = v[i++];
            else
                temp[k] = v[j++];
        }
        else {
            temp[k] = v[j++];
        }
    }
    for (int k = 0; k < (f - c + 1); k++) {
        v[c + k] = temp[k];
    }
}
void mergesort_pares(par* v, int c, int f, par* temp) {
    if (c >= f) return;
    int m = c + (f - c) / 2;
    mergesort_pares(v, c, m, temp);
    mergesort_pares(v, m + 1, f, temp);
    merge_pares(v, c, m, f, temp);
}

/*
Algoritmo que encontra a menor distância e guarda os pares que possuem essa distância:
Recursivamente, divide o conjunto de pontos ordenados pela metade e calcula a menor distancia em cada uma das metades,
depois compara com as distancias entre os pontos destes dois conjuntos que estejam distantes do meio, pelo eixo x, no maximo
pelo menor valor encontrado até o momento.
Caso os conjuntos tenham 3 ou menos pontos, a função compara as distâncias por força bruta (compara todas) 
*/
void minDist(ponto* v, int c, int f, par** pares_minimos, long long* d, int* qtd_pares, int* cap, ponto* temp, ponto* faixa) {
    if (c >= f) return;
    if (f - c <= 2) { // caso em que o conjunto possui 3 ou menos pontos
        for (int i = c; i <= f; i++) {
            for (int j = i + 1; j <= f; j++) {
                adicionar_ou_atualizar_par(v[i], v[j], pares_minimos, cap, d, qtd_pares);
            }
        }
        return;
    }

    int m = c + (f - c) / 2; // meio do conjunto

    // atua recursivamente
    minDist(v, c, m, pares_minimos, d, qtd_pares, cap, temp, faixa);
    minDist(v, m + 1, f, pares_minimos, d, qtd_pares, cap, temp, faixa);

    // coleta os pontos entre as duas metades que estejam distantes do meio, pelo eixo x, no maximo pelo menor valor encontrado até o momento
    // ponto* faixa = malloc((f - c + 1) * sizeof(ponto));
    int tamFaixa = 0;
    for (int i = c; i <= f; i++) {
        long long dx = (long long)v[i].x - v[m].x;
        if (dx * dx <= *d) {
            faixa[tamFaixa++] = v[i];
        }
    }

    // compara as distancias dos pontos na faixa com a menor distância encontrada até o momento
    if (tamFaixa > 0) {
        mergesort(faixa, 0, tamFaixa - 1, 1, temp);
        for (int i = 0; i < tamFaixa; i++) {
            for (int j = i + 1; j < tamFaixa; j++) {
                long long dy = (long long)faixa[j].y - faixa[i].y;
                if (dy * dy > *d) break;
                adicionar_ou_atualizar_par(faixa[i], faixa[j], pares_minimos, cap, d, qtd_pares);
            }
        }
    }
    return;
}

int main() {
    int n; // número de pontos
    scanf("%d", &n);

    int max = 1000; // máximo de comparações entre dois pontos possíveis
    par* pares_minimos = malloc(max*sizeof(par)); // lista que vai armazenar os pares que possuam a distância minimal
    int qtd_pares = 0;
    long long d = LLONG_MAX; // variável que vai armazenar a menor distância encontrada, sendo atualizada a cada nova distância menor encontrada

    // inicializando os pontos que serão analisados
    ponto* coor = malloc(n*sizeof(ponto));
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &coor[i].x, &coor[i].y);
        coor[i].id = i; // armazenamos o indice inicial pois o 'perdemos' quando ordenamos a lista
    }
    
    // ordena os pontos pela coordenada x
    ponto* temp = malloc(n*sizeof(ponto));
    mergesort(coor, 0, n - 1, 0, temp);

    // encontra os pontos com a menor distância
    ponto* faixa = malloc(n*sizeof(ponto));
    minDist(coor, 0, n - 1, &pares_minimos, &d, &qtd_pares, &max, temp, faixa);

    // ordenamos os pontos e excluímos as duplicatas
    par* temp_pares = malloc(qtd_pares*sizeof(par));
    mergesort_pares(pares_minimos, 0, qtd_pares - 1, temp_pares);
    int unique_qtd = 1;
    for (int i = 1; i < qtd_pares; i++) {
        if (pares_minimos[i].p1.id != pares_minimos[unique_qtd - 1].p1.id ||
            pares_minimos[i].p2.id != pares_minimos[unique_qtd - 1].p2.id) { // se os dois pontos próximos são diferentes não existe duplicata
            pares_minimos[unique_qtd++] = pares_minimos[i];
        }
    }
    qtd_pares = unique_qtd;

    // imprimi os pontos na lista
    for (int j = 0; j < qtd_pares; j++)
    {
        printf("(%d,%d)\n", pares_minimos[j].p1.id, pares_minimos[j].p2.id);
    }
    
    free(pares_minimos);
    free(coor);
    free(temp);
    free(faixa);
    free(temp_pares);
    return 0;
}