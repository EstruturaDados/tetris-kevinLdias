#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peca do jogo
typedef struct {
    char nome; // tipo da peca ('I', 'O', 'T', 'L')
    int id;    // identificador unico
} Peca;

// Estrutura da fila circular de pecas futuras
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Estrutura da pilha de pecas reservadas
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// Funcoes da fila
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

// Funcoes da pilha
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

// Gerar peca aleatoria
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Exibir estado atual
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
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Jogar peca (remove da fila)
            if (!filaVazia(&fila)) {
                Peca jogada = desenfileirar(&fila);
                printf("Peca jogada: [%c %d]\n", jogada.nome, jogada.id);
                enfileirar(&fila, gerarPeca(id++));
            } else {
                printf("Fila vazia!\n");
            }
        } 
        else if (opcao == 2) {
            // Reservar peca (fila -> pilha)
            if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                Peca reserva = desenfileirar(&fila);
                empilhar(&pilha, reserva);
                printf("Peca [%c %d] reservada!\n", reserva.nome, reserva.id);
                enfileirar(&fila, gerarPeca(id++));
            } else {
                printf("Nao foi possivel reservar.\n");
            }
        } 
        else if (opcao == 3) {
            // Usar peca reservada (remove da pilha)
            if (!pilhaVazia(&pilha)) {
                Peca usada = desempilhar(&pilha);
                printf("Peca reservada usada: [%c %d]\n", usada.nome, usada.id);
            } else {
                printf("Nao ha pecas reservadas.\n");
            }
        }

    } while (opcao != 0);

    printf("Saindo do jogo...\n");
    return 0;
}
