#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

// Função auxiliar para criar um novo nó
rb_no* criar_no(rb_arvore *arvore, int chave) {
    rb_no *no = (rb_no*)malloc(sizeof(rb_no));
    if (no != NULL) {
        no->chave = chave;
        no->cor = VERMELHO; // Novo nó sempre começa vermelho
        no->esquerda = arvore->sentinela;
        no->direita = arvore->sentinela;
        no->pai = arvore->sentinela;
    }
    return no;
}

// Função que cria uma árvore Rubro-Negra
rb_arvore* criar_arvore() {
    rb_arvore *arvore = (rb_arvore*)malloc(sizeof(rb_arvore));
    if (arvore != NULL) {
        arvore->sentinela = (rb_no*)malloc(sizeof(rb_no));
        if (arvore->sentinela != NULL) {
            arvore->sentinela->cor = PRETO;
            arvore->raiz = arvore->sentinela;
            arvore->num_elementos = 0;
        } else {
            free(arvore);
            arvore = NULL;
        }
    }
    return arvore;
}

// Funções auxiliares de rotação
void rotacao_esquerda(rb_arvore *arvore, rb_no *x) {
    rb_no *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != arvore->sentinela) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == arvore->sentinela) {
        arvore->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
}

void rotacao_direita(rb_arvore *arvore, rb_no *x) {
    rb_no *y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita != arvore->sentinela) {
        y->direita->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == arvore->sentinela) {
        arvore->raiz = y;
    } else if (x == x->pai->direita) {
        x->pai->direita = y;
    } else {
        x->pai->esquerda = y;
    }
    y->direita = x;
    x->pai = y;
}

// Função auxiliar para corrigir a árvore após a inserção
int corrigir_insercao(rb_arvore *arvore, rb_no *z) {
    int nRotacoes;
    nRotacoes = 0;
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            rb_no *y = z->pai->pai->direita;
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacao_esquerda(arvore, z);
                    nRotacoes += 1;
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacao_direita(arvore, z->pai->pai);
                nRotacoes += 1;
            }
        } else {
            rb_no *y = z->pai->pai->esquerda;
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacao_direita(arvore, z);
                    nRotacoes += 1;
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacao_esquerda(arvore, z->pai->pai);
                nRotacoes += 1;
            }
        }
    }
    arvore->raiz->cor = PRETO;
    return nRotacoes;
}

// Função que insere um nó na árvore Rubro-Negra
int inserir_no(rb_arvore *arvore, int chave, int *nRotacoes) {
    rb_no *z = criar_no(arvore, chave);
    if (z == NULL) return 0;

    rb_no *y = arvore->sentinela;
    rb_no *x = arvore->raiz;

    while (x != arvore->sentinela) {
        y = x;
        if (z->chave < x->chave) {
            x = x->esquerda;
        } else {
            x = x->direita;
        }
    }

    z->pai = y;
    if (y == arvore->sentinela) {
        arvore->raiz = z;
    } else if (z->chave < y->chave) {
        y->esquerda = z;
    } else {
        y->direita = z;
    }

    *nRotacoes += corrigir_insercao(arvore, z);
    arvore->num_elementos++;
    return 1;
}

// Função auxiliar para corrigir a árvore após a remoção
int corrigir_remocao(rb_arvore *arvore, rb_no *x) {
    int nRotacoes;
    nRotacoes = 0;
    while (x != arvore->raiz && x->cor == PRETO) {
        if (x == x->pai->esquerda) {
            rb_no *w = x->pai->direita;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacao_esquerda(arvore, x->pai);
                nRotacoes += 1;
                w = x->pai->direita;
            }
            if (w->esquerda->cor == PRETO && w->direita->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->direita->cor == PRETO) {
                    w->esquerda->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacao_direita(arvore, w);
                    nRotacoes += 1;
                    w = x->pai->direita;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->direita->cor = PRETO;
                rotacao_esquerda(arvore, x->pai);
                nRotacoes += 1;
                x = arvore->raiz;
            }
        } else {
            rb_no *w = x->pai->esquerda;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacao_direita(arvore, x->pai);
                nRotacoes += 1;
                w = x->pai->esquerda;
            }
            if (w->direita->cor == PRETO && w->esquerda->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->esquerda->cor == PRETO) {
                    w->direita->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacao_esquerda(arvore, w);
                    nRotacoes += 1;
                    w = x->pai->esquerda;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esquerda->cor = PRETO;
                rotacao_direita(arvore, x->pai);
                nRotacoes += 1;
                x = arvore->raiz;
            }
        }
    }
    x->cor = PRETO;
    return nRotacoes;
}

// Função auxiliar para substituir um nó por outro
void rb_transplantar(rb_arvore *arvore, rb_no *u, rb_no *v) {
    if (u->pai == arvore->sentinela) {
        arvore->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

// Função auxiliar para encontrar o menor nó de uma subárvore
rb_no* minimo_arvore(rb_arvore *arvore, rb_no *x) {
    while (x->esquerda != arvore->sentinela) {
        x = x->esquerda;
    }
    return x;
}

// Função que remove um nó da árvore Rubro-Negra
int remover_no(rb_arvore *arvore, int chave, int *nRotacoes) {
    rb_no *z = buscar_no(arvore, chave);
    if (z == arvore->sentinela) return -1;

    rb_no *y = z;
    rb_no *x;
    cor_no cor_original_y = y->cor;

    if (z->esquerda == arvore->sentinela) {
        x = z->direita;
        rb_transplantar(arvore, z, z->direita);
    } else if (z->direita == arvore->sentinela) {
        x = z->esquerda;
        rb_transplantar(arvore, z, z->esquerda);
    } else {
        y = minimo_arvore(arvore, z->direita);
        cor_original_y = y->cor;
        x = y->direita;
        if (y->pai == z) {
            x->pai = y;
        } else {
            rb_transplantar(arvore, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        rb_transplantar(arvore, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }
    free(z);

    if (cor_original_y == PRETO) {
        *nRotacoes += corrigir_remocao(arvore, x);
    }

    arvore->num_elementos--;
    return 1;
}

// Função que busca um nó na árvore Rubro-Negra
rb_no* buscar_no(rb_arvore *arvore, int chave) {
    rb_no *atual = arvore->raiz;
    while (atual != arvore->sentinela) {
        if (chave < atual->chave) {
            atual = atual->esquerda;
        } else if (chave > atual->chave) {
            atual = atual->direita;
        } else {
            return atual;
        }
    }
    return arvore->sentinela;
}

// Função auxiliar para impressão em ordem
void auxiliar_em_ordem(rb_arvore *arvore, rb_no *no) {
    if (no != arvore->sentinela) {
        auxiliar_em_ordem(arvore, no->esquerda);
        printf("%d ", no->chave);
        auxiliar_em_ordem(arvore, no->direita);
    }
}

// Função que imprime a árvore em ordem
void imprimir_em_ordem(rb_arvore *arvore) {
    auxiliar_em_ordem(arvore, arvore->raiz);
    printf("\n");
}

// Função auxiliar para impressão em pré-ordem
void auxiliar_pre_ordem(rb_arvore *arvore, rb_no *no) {
    if (no != arvore->sentinela) {
        printf("%d ", no->chave);
        auxiliar_pre_ordem(arvore, no->esquerda);
        auxiliar_pre_ordem(arvore, no->direita);
    }
}

// Função que imprime a árvore em pré-ordem
void imprimir_pre_ordem(rb_arvore *arvore) {
    auxiliar_pre_ordem(arvore, arvore->raiz);
    printf("\n");
}

// Função que retorna a quantidade de elementos da árvore
int obter_num_elementos(rb_arvore *arvore) {
    return arvore->num_elementos;
}

// Função que libera a memória da árvore
void liberar_no(rb_arvore *arvore, rb_no *no) {
    if (no != arvore->sentinela) {
        liberar_no(arvore, no->esquerda);
        liberar_no(arvore, no->direita);
        free(no);
    }
}