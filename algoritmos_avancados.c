

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.


    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//   NÍVEL 1 — Árvore Binária

struct Sala {
    char nome[50];
    char pista[50];      // pista opcional
    char suspeito[50];   // suspeito da pista
    struct Sala *esq;
    struct Sala *dir;
};

struct Sala* criarSala(char nome[], char pista[], char suspeito[]) {
    struct Sala *s = (struct Sala*) malloc(sizeof(struct Sala));
    strcpy(s->nome, nome);
    strcpy(s->pista, pista);
    strcpy(s->suspeito, suspeito);
    s->esq = NULL;
    s->dir = NULL;
    return s;
}

//   NÍVEL 2 — Árvore de Busca

struct NodoBST {
    char pista[50];
    struct NodoBST *esq;
    struct NodoBST *dir;
};

struct NodoBST* inserirPista(struct NodoBST *raiz, char pista[]) {
    if (raiz == NULL) {
        struct NodoBST *n = (struct NodoBST*) malloc(sizeof(struct NodoBST));
        strcpy(n->pista, pista);
        n->esq = n->dir = NULL;
        return n;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

void mostrarPistas(struct NodoBST *raiz) {
    if (raiz == NULL) return;
    mostrarPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    mostrarPistas(raiz->dir);
}

//   NÍVEL 3 — Tabela Hash

#define TAM_HASH 10

struct ItemHash {
    char pista[50];
    char suspeito[50];
    struct ItemHash *prox;
};

struct ItemHash* tabela[TAM_HASH];

int funcaoHash(char pista[]) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % TAM_HASH;
}

void inserirHash(char pista[], char suspeito[]) {
    int h = funcaoHash(pista);

    struct ItemHash *novo = (struct ItemHash*) malloc(sizeof(struct ItemHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->prox = tabela[h];
    tabela[h] = novo;
}

void mostrarHash() {
    printf("\n=== Pistas e Suspeitos ===\n");
    for (int i = 0; i < TAM_HASH; i++) {
        struct ItemHash *aux = tabela[i];
        while (aux != NULL) {
            printf("%s -> %s\n", aux->pista, aux->suspeito);
            aux = aux->prox;
        }
    }
}

void suspeitoMaisCitado() {
    char nomes[20][50];
    int cont[20];
    int total = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        struct ItemHash *a = tabela[i];
        while (a != NULL) {
            int achou = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(nomes[j], a->suspeito) == 0) {
                    cont[j]++;
                    achou = 1;
                    break;
                }
            }
            if (!achou) {
                strcpy(nomes[total], a->suspeito);
                cont[total] = 1;
                total++;
            }
            a = a->prox;
        }
    }

    if (total == 0) {
        printf("Nenhuma pista encontrada.\n");
        return;
    }

    int maior = 0;
    for (int i = 1; i < total; i++) {
        if (cont[i] > cont[maior])
            maior = i;
    }

    printf("\n🔍 Suspeito mais citado: %s (%d pistas)\n", nomes[maior], cont[maior]);
}

//      EXPLORAÇÃO DO JOGO

void explorar(struct Sala *atual, struct NodoBST **bst) {
    char op;

    while (atual != NULL) {
        printf("\n📍 Você está em: %s\n", atual->nome);

        // Se tiver pista, registrar
        if (strlen(atual->pista) > 0) {
            printf("🔎 Você encontrou uma pista: %s\n", atual->pista);

            *bst = inserirPista(*bst, atual->pista);         // insere no BST
            inserirHash(atual->pista, atual->suspeito);     // insere na hash
        }

        printf("Ir para (e) esquerda | (d) direita | (s) sair: ");
        scanf(" %c", &op);

        if (op == 'e') atual = atual->esq;
        else if (op == 'd') atual = atual->dir;
        else if (op == 's') break;
        else printf("Opção inválida!\n");
    }

    printf("\nFim da exploração.\n");
}

//            MAIN

int main() {
    for (int i = 0; i < TAM_HASH; i++) tabela[i] = NULL;

    // Criação simples da árvore da mansão
    struct Sala *hall = criarSala("Hall de Entrada", "", "");

    hall->esq = criarSala("Biblioteca", "Livro rasgado", "Sr. Black");
    hall->dir = criarSala("Cozinha", "Faca suja", "Chef Marco");

    hall->esq->esq = criarSala("Sala de Estudos", "", "");
    hall->esq->dir = criarSala("Porão", "Pegada", "Jardineiro");

    hall->dir->esq = criarSala("Despensa", "Pano queimado", "Dona Rosa");
    hall->dir->dir = criarSala("Jardim", "", "");

    struct NodoBST *pistas = NULL;

    printf("=== DETECTIVE QUEST — Versão Simples ===\n");

    explorar(hall, &pistas);

    printf("\n=== PISTAS ENCONTRADAS ===\n");
    mostrarPistas(pistas);

    mostrarHash();
    suspeitoMaisCitado();

    return 0;
}

