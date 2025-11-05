#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5

// Estrutura que representa uma peca do jogo
typedef struct
{
    char nome; // tipo da peca (I, O, T, L)
    int id;    // identificador unico
} Peca;

// Estrutura que representa a fila de pecas
typedef struct
{
    Peca pecas[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Prototipos
void inicializarFila(Fila *fila);
int filaCheia(Fila *fila);
int filaVazia(Fila *fila);
void enfileirar(Fila *fila, Peca nova);
Peca desenfileirar(Fila *fila);
void exibirFila(Fila *fila);
Peca gerarPeca(int id);

// Funcao principal
int main()
{
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));

    // Inicializa a fila com 5 pecas
    for (int i = 0; i < TAM_FILA; i++)
    {
        Peca p = gerarPeca(i);
        enfileirar(&fila, p);
    }

    int opcao, contadorID = TAM_FILA;

    do
    {
        printf("\n==============================\n");
        printf("Fila de pecas:\n");
        exibirFila(&fila);
        printf("\n==============================\n");
        printf("Opcoes:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            if (!filaVazia(&fila))
            {
                Peca jogada = desenfileirar(&fila);
                printf("\nPeca [%c %d] jogada!\n", jogada.nome, jogada.id);
            }
            else
            {
                printf("\nFila vazia! Nao ha pecas para jogar.\n");
            }
            break;

        case 2:
            if (!filaCheia(&fila))
            {
                Peca nova = gerarPeca(contadorID++);
                enfileirar(&fila, nova);
                printf("\nNova peca [%c %d] adicionada!\n", nova.nome, nova.id);
            }
            else
            {
                printf("\nFila cheia! Nao e possivel adicionar nova peca.\n");
            }
            break;

        case 0:
            printf("\nEncerrando o programa...\n");
            break;

        default:
            printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// Inicializa a fila
void inicializarFila(Fila *fila)
{
    fila->inicio = 0;
    fila->fim = -1;
    fila->quantidade = 0;
}

// Verifica se a fila esta cheia
int filaCheia(Fila *fila)
{
    return fila->quantidade == TAM_FILA;
}

// Verifica se a fila esta vazia
int filaVazia(Fila *fila)
{
    return fila->quantidade == 0;
}

// Insere nova peca no final da fila
void enfileirar(Fila *fila, Peca nova)
{
    if (filaCheia(fila))
        return;
    fila->fim = (fila->fim + 1) % TAM_FILA;
    fila->pecas[fila->fim] = nova;
    fila->quantidade++;
}

// Remove peca do inicio da fila
Peca desenfileirar(Fila *fila)
{
    Peca removida = {'?', -1};
    if (filaVazia(fila))
        return removida;
    removida = fila->pecas[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_FILA;
    fila->quantidade--;
    return removida;
}

// Exibe todas as pecas da fila
void exibirFila(Fila *fila)
{
    if (filaVazia(fila))
    {
        printf("[Fila vazia]\n");
        return;
    }

    int i = fila->inicio;
    for (int c = 0; c < fila->quantidade; c++)
    {
        printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

// Gera uma nova peca aleatoria
Peca gerarPeca(int id)
{
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}
