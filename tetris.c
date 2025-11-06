#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peca
typedef struct {
    char nome; // tipo ('I', 'O', 'T', 'L')
    int id;    // identificador unico
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ---------- FUNCOES DA FILA ----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->quantidade = 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->itens[f->fim] = p;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca p = {'?', -1};
    if (filaVazia(f)) return p;
    p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return p;
}

// ---------- FUNCOES DA PILHA ----------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void empilhar(Pilha *p, Peca e) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Nao e possivel reservar mais pecas.\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = e;
}

Peca desempilhar(Pilha *p) {
    Peca e = {'?', -1};
    if (pilhaVazia(p)) {
        printf("Pilha vazia! Nao ha pecas reservadas.\n");
        return e;
    }
    e = p->itens[p->topo];
    p->topo--;
    return e;
}

// ---------- FUNCAO DE GERAR PECA ----------
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ---------- EXIBIR ESTADO ----------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n=== ESTADO ATUAL ===\n");

    printf("Fila de pecas: ");
    if (filaVazia(f)) printf("(vazia)");
    else {
        int i, idx = f->inicio;
        for (i = 0; i < f->quantidade; i++) {
            Peca atual = f->itens[idx];
            printf("[%c %d] ", atual.nome, atual.id);
            idx = (idx + 1) % TAM_FILA;
        }
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) printf("(vazia)");
    else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }

    printf("\n=====================\n");
}

// ---------- TROCAR PECA UNICA ----------
void trocarPecaTopoFila(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Nao foi possivel realizar a troca. Estruturas insuficientes.\n");
        return;
    }
    int idxFrente = f->inicio;
    Peca temp = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("Troca entre a frente da fila e o topo da pilha realizada!\n");
}

// ---------- TROCA MULTIPLA ----------
void trocarMultiplas(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) {
        printf("Nao e possivel realizar a troca multipla. Faltam pecas.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % TAM_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("Troca multipla entre as tres primeiras da fila e as tres da pilha realizada!\n");
}

// ---------- FUNCAO PRINCIPAL ----------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, id = 0;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(id++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpcoes:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peca
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("Peca jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca(id++));
                } else {
                    printf("Fila vazia!\n");
                }
                break;
            }
            case 2: { // Reservar peca
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reserva = desenfileirar(&fila);
                    empilhar(&pilha, reserva);
                    printf("Peca [%c %d] enviada para a pilha de reserva!\n", reserva.nome, reserva.id);
                    enfileirar(&fila, gerarPeca(id++));
                } else {
                    printf("Nao foi possivel reservar a peca.\n");
                }
                break;
            }
            case 3: { // Usar peca reservada
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("Peca reservada usada: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("Nao ha pecas reservadas.\n");
                }
                break;
            }
            case 4:
                trocarPecaTopoFila(&fila, &pilha);
                break;

            case 5:
                trocarMultiplas(&fila, &pilha);
                break;

            case 0:
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
