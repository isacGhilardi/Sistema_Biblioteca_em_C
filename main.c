#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas principais
typedef struct livro {
    int id;
    char titulo[50];
    char autor[30];
    int disponivel;
    struct livro* prox;
} Livro;

typedef struct leitor {
    int id;
    char nome[30];
    struct leitor* prox;
} Leitor;

typedef struct emprestimo {
    int idLivro;
    int idLeitor;
    struct emprestimo* prox;
    struct emprestimo* ant;
} Emprestimo;

typedef struct operacao {
    char descricao[100];
    struct operacao* prox;
} Operacao;

typedef struct sistema {
    Livro* listaLivros;
    Leitor* listaLeitores;
    Emprestimo* listaEmprestimos;
    Operacao* pilhaOperacoes;
    int proxIdLivro;
    int proxIdLeitor;
} Sistema;

// Criar sistema
Sistema* criarSistema() {
    Sistema* s = (Sistema*)malloc(sizeof(Sistema));
    s->listaLivros = NULL;
    s->listaLeitores = NULL;
    s->listaEmprestimos = NULL;
    s->pilhaOperacoes = NULL;
    s->proxIdLivro = 1;
    s->proxIdLeitor = 1;
    return s;
}

// Empilhar operacao
void empilharOperacao(Sistema* s, const char* texto) {
    Operacao* nova = (Operacao*)malloc(sizeof(Operacao));
    strcpy(nova->descricao, texto);
    nova->prox = s->pilhaOperacoes;
    s->pilhaOperacoes = nova;
}

// Desempilhar operacao
void desfazerOperacao(Sistema* s) {
    if (s->pilhaOperacoes == NULL) {
        printf("Nenhuma operacao para desfazer!\n");
        return;
    }
    Operacao* topo = s->pilhaOperacoes;
    printf("Desfeita: %s\n", topo->descricao);
    s->pilhaOperacoes = topo->prox;
    free(topo);
}

// Listar historico
void listarHistorico(Sistema* s) {
    Operacao* atual = s->pilhaOperacoes;
    int count = 0;
    printf("\n Historico de Operacoes \n");
    while (atual != NULL && count < 5) {
        printf("- %s\n", atual->descricao);
        atual = atual->prox;
        count++;
    }
}

// LIVROS
void cadastrarLivro(Sistema* s) {
    Livro* novo = (Livro*)malloc(sizeof(Livro));
    novo->id = s->proxIdLivro++;
    printf("Titulo: ");
    getchar();
    fgets(novo->titulo, 50, stdin);
    novo->titulo[strcspn(novo->titulo, "\n")] = '\0';
    printf("Autor: ");
    fgets(novo->autor, 30, stdin);
    novo->autor[strcspn(novo->autor, "\n")] = '\0';
    novo->disponivel = 1;
    novo->prox = s->listaLivros;
    s->listaLivros = novo;
    printf("Livro cadastrado! ID: %d\n", novo->id);

    char desc[100];
    sprintf(desc, "Cadastrado livro %d - %s", novo->id, novo->titulo);
    empilharOperacao(s, desc);
}

Livro* buscarLivro(Sistema* s, int id) {
    Livro* atual = s->listaLivros;
    while (atual != NULL) {
        if (atual->id == id) return atual;
        atual = atual->prox;
    }
    return NULL;
}

// LEITORES
void cadastrarLeitor(Sistema* s) {
    Leitor* novo = (Leitor*)malloc(sizeof(Leitor));
    novo->id = s->proxIdLeitor++;
    printf("Nome do leitor: ");
    getchar();
    fgets(novo->nome, 30, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';
    novo->prox = s->listaLeitores;
    s->listaLeitores = novo;
    printf("Leitor cadastrado! Codigo: %d\n", novo->id);

    char desc[100];
    sprintf(desc, "Cadastrado leitor %d - %s", novo->id, novo->nome);
    empilharOperacao(s, desc);
}

Leitor* buscarLeitor(Sistema* s, int id) {
    Leitor* atual = s->listaLeitores;
    while (atual != NULL) {
        if (atual->id == id) return atual;
        atual = atual->prox;
    }
    return NULL;
}

// EMPRESTIMOS
void realizarEmprestimo(Sistema* s) {
    int idLivro, idLeitor;
    printf("ID do livro: ");
    scanf("%d", &idLivro);
    Livro* livro = buscarLivro(s, idLivro);
    if (livro == NULL || !livro->disponivel) {
        printf("Livro nao disponivel!\n");
        return;
    }
    printf("ID do leitor: ");
    scanf("%d", &idLeitor);
    if (buscarLeitor(s, idLeitor) == NULL) {
        printf("Leitor nao encontrado!\n");
        return;
    }
    Emprestimo* novo = (Emprestimo*)malloc(sizeof(Emprestimo));
    novo->idLivro = idLivro;
    novo->idLeitor = idLeitor;
    novo->prox = s->listaEmprestimos;
    novo->ant = NULL;
    if (s->listaEmprestimos != NULL)
        s->listaEmprestimos->ant = novo;
    s->listaEmprestimos = novo;
    livro->disponivel = 0;
    printf("Emprestimo realizado!\n");

    char desc[100];
    sprintf(desc, "Emprestado livro %d para leitor %d", idLivro, idLeitor);
    empilharOperacao(s, desc);
}

void realizarDevolucao(Sistema* s) {
    int idLivro;
    printf("ID do livro para devolucao: ");
    scanf("%d", &idLivro);
    Emprestimo* atual = s->listaEmprestimos;
    while (atual != NULL) {
        if (atual->idLivro == idLivro) {
            if (atual->ant != NULL) atual->ant->prox = atual->prox;
            if (atual->prox != NULL) atual->prox->ant = atual->ant;
            if (atual == s->listaEmprestimos) s->listaEmprestimos = atual->prox;
            Livro* l = buscarLivro(s, idLivro);
            if (l != NULL) l->disponivel = 1;
            free(atual);
            printf("Devolucao realizada!\n");

            char desc[100];
            sprintf(desc, "Devolvido livro %d", idLivro);
            empilharOperacao(s, desc);
            return;
        }
        atual = atual->prox;
    }
    printf("Emprestimo nao encontrado.\n");
}

// LISTAGENS
void listarLivros(Sistema* s) {
    Livro* atual = s->listaLivros;
    printf("\n=== Lista de Livros ===\n");
    while (atual != NULL) {
        printf("[%d] %s - %s (%s)\n", atual->id, atual->titulo, atual->autor,
               atual->disponivel ? "Disponivel" : "Emprestado");
        atual = atual->prox;
    }
}

int main() {
    Sistema* sistema = criarSistema();
    int op;
    do {
        printf("---- SUPER MEGA BIBLIOTECA ----\n");
        printf("--------------------------------\n");
        printf("\n--- MENU ---\n");
        printf("1 - Cadastrar Livro\n");
        printf("2 - Cadastrar Leitor\n");
        printf("3 - Listar Livros\n");
        printf("4 - Realizar Emprestimo\n");
        printf("5 - Realizar Devolucao\n");
        printf("6 - Historico de Operacoes\n");
        printf("7 - Desfazer Operacao\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);
        switch(op) {
            case 1: cadastrarLivro(sistema); break;
            case 2: cadastrarLeitor(sistema); break;
            case 3: listarLivros(sistema); break;
            case 4: realizarEmprestimo(sistema); break;
            case 5: realizarDevolucao(sistema); break;
            case 6: listarHistorico(sistema); break;
            case 7: desfazerOperacao(sistema); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while(op != 0);
    return 0;
}
