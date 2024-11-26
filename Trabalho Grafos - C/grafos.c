#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX
#define N 5

char cidades[N] = {'A', 'B', 'C', 'D', 'E'};

int menorDistancia(int dist[], bool visitado[]) {
    int min = INF, minIndex;

    for (int v = 0; v < N; v++) {
        if (!visitado[v] && dist[v] <= min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void imprimirCaminho(int anterior[], int j) {
    if (anterior[j] == -1) {
        return;
    }
    imprimirCaminho(anterior, anterior[j]);
    printf(" -> %c", cidades[j]);
}

void dijkstra(int grafo[N][N], int origem) {
    int dist[N];
    bool visitado[N];
    int anterior[N];

    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        visitado[i] = false;
        anterior[i] = -1;
    }

    dist[origem] = 0;

    for (int count = 0; count < N - 1; count++) {
        int u = menorDistancia(dist, visitado);
        visitado[u] = true;

        for (int v = 0; v < N; v++) {
            if (!visitado[v] && grafo[u][v] && dist[u] != INF &&
                dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];
                anterior[v] = u;
            }
        }
    }

    printf("Cidade Origem: %c\n\n", cidades[origem]);
    for (int i = 0; i < N; i++) {
        if (i != origem) {
            printf("Menor tempo para chegar em %c: %d horas\n", cidades[i], dist[i]);
            printf("Caminho: %c", cidades[origem]);
            imprimirCaminho(anterior, i);
            printf("\n\n");
        }
    }
}

int main() {
  int grafo[N][N] = {
    // A  B  C  D  E
      {0, 4, 2, 7, 0}, // A
      {4, 0, 1, 0, 1}, // B
      {2, 1, 0, 3, 3}, // C
      {7, 0, 3, 0, 2}, // D
      {0, 1, 3, 2, 0}  // E
  };

    dijkstra(grafo, 0);

    return 0;
}