#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TAMANHO 10

typedef struct Contato {
    char nome[100];
    char telefone[20];
    struct Contato *prox;
} Contato;

typedef struct {
    Contato *buckets[TAMANHO];
} TabelaHash;

unsigned int funcao_hash(const char *chave) {
    unsigned int hash = 0;
    while (*chave) {
        hash = (hash * 31) + *chave++;
    }
    return hash % TAMANHO;
}

void inicializar_tabela(TabelaHash *tabela) {
    for (int i = 0; i < TAMANHO; i++) {
        tabela->buckets[i] = NULL;
    }
}

char *formatar_telefone(const char *telefone) {
    static char telefone_formatado[20];
    int len = 0;

    for (int i = 0; telefone[i] != '\0'; i++) {
        if (isdigit(telefone[i])) {
            telefone_formatado[len++] = telefone[i];
        }
    }
    telefone_formatado[len] = '\0';

    if (len == 11) {
        sprintf(telefone_formatado, "(%c%c) %c%c%c%c%c-%c%c%c%c",
                telefone_formatado[0], telefone_formatado[1],
                telefone_formatado[2], telefone_formatado[3], telefone_formatado[4],
                telefone_formatado[5], telefone_formatado[6],
                telefone_formatado[7], telefone_formatado[8], telefone_formatado[9], telefone_formatado[10]);
    } else if (len == 10) {
        sprintf(telefone_formatado, "(%c%c) %c%c%c%c-%c%c%c%c",
                telefone_formatado[0], telefone_formatado[1],
                telefone_formatado[2], telefone_formatado[3], telefone_formatado[4],
                telefone_formatado[5],
                telefone_formatado[6], telefone_formatado[7], telefone_formatado[8], telefone_formatado[9]);
    } else {
        return NULL;
    }
    return telefone_formatado;
}

void inserir(TabelaHash *tabela, const char *nome, const char *telefone) {
    char *telefone_formatado = formatar_telefone(telefone);
    if (!telefone_formatado) {
        printf("Numero de telefone invalido.\n");
        return;
    }

    unsigned int indice = funcao_hash(nome);
    Contato *atual = tabela->buckets[indice];

    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            strcpy(atual->telefone, telefone_formatado);
            printf("Contato %s atualizado.\n", nome);
            return;
        }
        atual = atual->prox;
    }

    Contato *novo = (Contato *)malloc(sizeof(Contato));
    strcpy(novo->nome, nome);
    strcpy(novo->telefone, telefone_formatado);
    novo->prox = tabela->buckets[indice];
    tabela->buckets[indice] = novo;

    printf("Contato %s adicionado com sucesso.\n", nome);
}

char *buscar(TabelaHash *tabela, const char *nome) {
    unsigned int indice = funcao_hash(nome);
    Contato *atual = tabela->buckets[indice];

    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual->telefone;
        }
        atual = atual->prox;
    }
    return NULL;
}

void remover(TabelaHash *tabela, const char *nome) {
    unsigned int indice = funcao_hash(nome);
    Contato *atual = tabela->buckets[indice];
    Contato *anterior = NULL;

    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            if (anterior == NULL) {
                tabela->buckets[indice] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            printf("Contato %s removido com sucesso.\n", nome);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    printf("Contato %s nao encontrado.\n", nome);
}

void exibir(TabelaHash *tabela) {
    printf("Contatos na lista telefonica:\n");
    for (int i = 0; i < TAMANHO; i++) {
        Contato *atual = tabela->buckets[i];
        while (atual) {
            printf("%s: %s\n", atual->nome, atual->telefone);
            atual = atual->prox;
        }
    }
}

int main() {
    TabelaHash tabela;
    inicializar_tabela(&tabela);

    while (1) {
        printf("\n--- Lista Telefonica ---\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");

        int opcao;
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1: {
                char nome[100], telefone[20];
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0'; // Remove o '\n'
                printf("Telefone: ");
                fgets(telefone, sizeof(telefone), stdin);
                telefone[strcspn(telefone, "\n")] = '\0';

                clock_t inicio = clock();
                inserir(&tabela, nome, telefone);
                clock_t fim = clock();

                printf("Operaçao concluida em %.4f ms\n", (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC);
                break;
            }
            case 2: {
                char nome[100];
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                clock_t inicio = clock();
                char *resultado = buscar(&tabela, nome);
                clock_t fim = clock();

                if (resultado) {
                    printf("Telefone de %s: %s (tempo de busca: %.4f ms)\n", nome, resultado, (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC);
                } else {
                    printf("Contato %s nao encontrado. (tempo de busca: %.4f ms)\n", nome, (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC);
                }
                break;
            }
            case 3: {
                char nome[100];
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                clock_t inicio = clock();
                remover(&tabela, nome);
                clock_t fim = clock();

                printf("Operacao concluida em %.4f ms\n", (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC);
                break;
            }
            case 4: {
                clock_t inicio = clock();
                exibir(&tabela);
                clock_t fim = clock();

                printf("Operacao concluida em %.4f ms\n", (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC);
                break;
            }
            case 0:
                printf("Encerrando o programa. Ate logo!\n");
                return 0;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    }
}
