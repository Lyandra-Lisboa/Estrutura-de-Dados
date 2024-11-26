#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO 10

// Estrutura para armazenar um voto
typedef struct Voto {
    char id[50];
    char candidato[50];
    struct Voto* proximo; // Para lidar com colisões
} Voto;

// Tabela hash e votos
typedef struct SistemaDeVotacao {
    Voto* tabela[TAMANHO]; // Tabela hash
    int votos_candidatos[TAMANHO];
    char candidatos[TAMANHO][50];
    int num_candidatos;
} SistemaDeVotacao;

// Função hash
int funcao_hash(const char* chave) {
    int hash = 0;
    while (*chave) {
        hash = (hash + *chave) % TAMANHO;
        chave++;
    }
    return hash;
}

// Inicializa o sistema de votação
void inicializar(SistemaDeVotacao* sistema) {
    for (int i = 0; i < TAMANHO; i++) {
        sistema->tabela[i] = NULL;
    }
    sistema->num_candidatos = 0;
}

// Busca um usuário na tabela hash
int buscar_usuario(SistemaDeVotacao* sistema, const char* id) {
    int indice = funcao_hash(id);
    Voto* atual = sistema->tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->id, id) == 0) {
            return 1; // Encontrado
        }
        atual = atual->proximo;
    }
    return 0; // Não encontrado
}

// Registra um voto
void votar(SistemaDeVotacao* sistema, const char* id, const char* candidato) {
    if (buscar_usuario(sistema, id)) {
        printf("Erro: Eleitor ja votou.\n");
        return;
    }

    // Adiciona o voto para o candidato
    int encontrado = 0;
    for (int i = 0; i < sistema->num_candidatos; i++) {
        if (strcmp(sistema->candidatos[i], candidato) == 0) {
            sistema->votos_candidatos[i]++;
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        strcpy(sistema->candidatos[sistema->num_candidatos], candidato);
        sistema->votos_candidatos[sistema->num_candidatos] = 1;
        sistema->num_candidatos++;
    }

    // Adiciona o usuário à tabela hash
    int indice = funcao_hash(id);
    Voto* novo_voto = (Voto*)malloc(sizeof(Voto));
    strcpy(novo_voto->id, id);
    strcpy(novo_voto->candidato, candidato);
    novo_voto->proximo = sistema->tabela[indice];
    sistema->tabela[indice] = novo_voto;

    printf("Voto registrado para %s!\n", candidato);
}

// Exibe os resultados
void obter_resultado(SistemaDeVotacao* sistema) {
    if (sistema->num_candidatos == 0) {
        printf("Nenhum voto registrado ainda.\n");
        return;
    }

    printf("Resultados da eleicao:\n");
    for (int i = 0; i < sistema->num_candidatos; i++) {
        printf("%s: %d voto(s)\n", sistema->candidatos[i], sistema->votos_candidatos[i]);
    }
}

// Libera a memória alocada
void liberar(SistemaDeVotacao* sistema) {
    for (int i = 0; i < TAMANHO; i++) {
        Voto* atual = sistema->tabela[i];
        while (atual != NULL) {
            Voto* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

// Menu principal
void menu() {
    SistemaDeVotacao sistema;
    inicializar(&sistema);

    while (1) {
        printf("\n--- Sistema de Votacao ---\n");
        printf("1 - Registrar voto\n");
        printf("2 - Mostrar resultados\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");

        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1) {
            char id[50], candidato[50];
            printf("Digite o ID do eleitor: ");
            scanf("%s", id);
            printf("Digite o nome do candidato: ");
            scanf("%s", candidato);
            votar(&sistema, id, candidato);
        } else if (opcao == 2) {
            obter_resultado(&sistema);
        } else if (opcao == 0) {
            printf("Encerrando o programa. Ate logo!\n");
            break;
        } else {
            printf("Opcao invalida! Tente novamente.\n");
        }
    }

    liberar(&sistema);
}

int main() {
    menu();
    return 0;
}
