#ifndef RB_TREE_H
#define RB_TREE_H

typedef enum { VERMELHO, PRETO } cor_no;

typedef struct rb_no {
    int chave;
    cor_no cor;
    struct rb_no *esquerda;
    struct rb_no *direita;
    struct rb_no *pai;
} rb_no;

typedef struct rb_arvore {
    rb_no *raiz;
    rb_no *sentinela; // Sentinela
    int num_elementos;
} rb_arvore;

// Função que cria uma árvore Rubro-Negra
rb_arvore* criar_arvore();

// Função que insere um nó na árvore Rubro-Negra
int inserir_no(rb_arvore *arvore, int chave, int *nRotacoes);

// Função que remove um nó da árvore Rubro-Negra
int remover_no(rb_arvore *arvore, int chave, int *nRotacoes);

// Função que imprime a árvore em ordem
void imprimir_em_ordem(rb_arvore *arvore);

// Função que imprime a árvore em pré-ordem
void imprimir_pre_ordem(rb_arvore *arvore);

// Função que busca um nó na árvore Rubro-Negra
rb_no* buscar_no(rb_arvore *arvore, int chave);

// Função que retorna a quantidade de elementos da árvore
int obter_num_elementos(rb_arvore *arvore);

#endif // RB_TREE_H
