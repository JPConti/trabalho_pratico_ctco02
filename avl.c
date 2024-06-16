#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

typedef struct no {
    int chave;
    int fb; // fator de balanceamento
    struct no *fesq;
    struct no *fdir;
    struct no *pai;
} no;

typedef struct avl {
    no *sentinela;
    int numElementos;
} avl;

avl* criaArvore() {
    avl *arvore = (avl*)malloc(sizeof(avl));
    if (arvore != NULL) {
        arvore->sentinela = (no*)malloc(sizeof(no));
        if (arvore->sentinela != NULL) {
            arvore->sentinela->chave = -1000;
            arvore->sentinela->fb = 0;
            arvore->sentinela->fesq = NULL;
            arvore->sentinela->fdir = NULL;
            arvore->sentinela->pai = NULL;
            arvore->numElementos = 0;
        } else {
            free(arvore);
            arvore = NULL;
        }
    }
    return arvore;
}


// Função que realiza a inserção de um nó na árvore AVL
int insereNo(avl *arvore, int valor, int *nRotacoes) {
    no *novo = (no*)malloc(sizeof(no));
    if (novo == NULL) return 0;
    novo->chave = valor;
    novo->fb = 0;
    novo->fesq = NULL;
    novo->fdir = NULL;
    novo->pai = NULL;

    if (arvore->sentinela->fesq == NULL) {
        arvore->sentinela->fesq = novo;
        novo->pai = arvore->sentinela;
    } 
    else {
        no *atual = arvore->sentinela->fesq;
        no *pai = NULL;

        while (atual != NULL) {
            pai = atual;
            if (valor < atual->chave) {
                atual = atual->fesq;
            } else if (valor > atual->chave) {
                atual = atual->fdir;
            } else {
                free(novo);
                return 0;
            }
        }

        novo->pai = pai;
        if (valor < pai->chave) {
            pai->fesq = novo;
        } else {
            pai->fdir = novo;
        }

        *nRotacoes += atualizaFB_insercao(arvore, novo);
    }

    arvore->numElementos++;
    return 1;
}

no* buscarNo(avl *arv, int valor) {
    if (arv == NULL || arv->sentinela->fesq == NULL)
        return NULL;

    no *atual = arv->sentinela->fesq;
    while (atual != NULL) {
        if (valor == atual->chave)
            return atual;
        else if (valor < atual->chave)
            atual = atual->fesq;
        else
            atual = atual->fdir;
    }
    return NULL;
}

int removeNo(avl *arv, int valor, int *nRotacoes) {
    if (arv == NULL || arv->sentinela->fesq == NULL)
        return 0;

    no *noRemover = buscarNo(arv, valor);
    if (noRemover == NULL)
        return -1;

    no *pai = noRemover->pai;
    int chaveRemovida = noRemover->chave;

    if (noRemover->fesq == NULL && noRemover->fdir == NULL) { // Caso 1: Nó é folha
        if (noRemover->pai->fesq == noRemover)
            noRemover->pai->fesq = NULL;
        else
            noRemover->pai->fdir = NULL;
    } else if (noRemover->fesq != NULL && noRemover->fdir != NULL) { // Caso 3: Nó tem dois filhos
        no *sucessor = noRemover->fdir;
        while (sucessor->fesq != NULL)
            sucessor = sucessor->fesq;
        noRemover->chave = sucessor->chave;
        if (sucessor->pai->fesq == sucessor)
            sucessor->pai->fesq = sucessor->fdir;
        else
            sucessor->pai->fdir = sucessor->fdir;
        if (sucessor->fdir != NULL)
            sucessor->fdir->pai = sucessor->pai;
        noRemover = sucessor;
    } else { // Caso 2: Nó tem um filho
        no *filho = (noRemover->fesq != NULL) ? noRemover->fesq : noRemover->fdir;
        if (noRemover->pai->fesq == noRemover)
            noRemover->pai->fesq = filho;
        else
            noRemover->pai->fdir = filho;
        if (filho != NULL)
            filho->pai = noRemover->pai;
    }

    // Atualizar a referência da raiz se necessário
    if (arv->sentinela->fesq == noRemover)
        arv->sentinela->fesq = (noRemover->fesq != NULL) ? noRemover->fesq : noRemover->fdir;

    *nRotacoes += atualizaFB_remocao(arv, pai, chaveRemovida);
    arv->numElementos--;
    free(noRemover);
    return 1;
}


int atualizaFB_insercao(avl *arv, no *novoNo) {
    int nRotacoes;
    no *atual = novoNo->pai;
    nRotacoes = 0;
    while (atual != NULL && atual != arv->sentinela) {
        if (novoNo == atual->fesq) {
            atual->fb--;
        } 
        else {
            atual->fb++;
        }
        if (atual->fb == 0) {
            break;
        } 
        else if (atual->fb == 2 || atual->fb == -2) {
            nRotacoes = nRotacoes + balanceamento(arv, atual);
            break;
        }

        novoNo = atual;
        atual = atual->pai;
    }
    return nRotacoes;
}


int balanceamento(avl *arv, no *noDesbal) {
    int nRotacoes;
    if (noDesbal->fb == -2) {
        if (noDesbal->fesq->fb <= 0) {
            rotacaoDir(noDesbal);
            nRotacoes = 1;
        } 
        else {
            rotacaoEsq(noDesbal->fesq);
            rotacaoDir(noDesbal);
            nRotacoes = 2;
        }
    } 
    else if (noDesbal->fb == 2) {
        if (noDesbal->fdir->fb >= 0) { 
            rotacaoEsq(noDesbal);
            nRotacoes = 1;
        } 
        else {
            rotacaoDir(noDesbal->fdir);
            rotacaoEsq(noDesbal);
            nRotacoes = 2;
        }
    }
    return nRotacoes;
}

void rotacaoDir(no *noDesbal) {
    no *esq = noDesbal->fesq;
    noDesbal->fesq = esq->fdir;
    if (esq->fdir != NULL) {
        esq->fdir->pai = noDesbal;
    }
    esq->pai = noDesbal->pai;
    if (noDesbal->pai != NULL) {
        if (noDesbal == noDesbal->pai->fesq) {
            noDesbal->pai->fesq = esq;
        } 
        else {
            noDesbal->pai->fdir = esq;
        }
    }
    esq->fdir = noDesbal;
    noDesbal->pai = esq;
    noDesbal->fb = noDesbal->fb + 1 - (esq->fb < 0 ? esq->fb : 0);
    esq->fb = esq->fb + 1 + (noDesbal->fb > 0 ? noDesbal->fb : 0);
}

void rotacaoEsq(no *noDesbal) {
    no *dir = noDesbal->fdir;
    noDesbal->fdir = dir->fesq;
    if (dir->fesq != NULL) {
        dir->fesq->pai = noDesbal;
    }
    dir->pai = noDesbal->pai;
    if (noDesbal->pai != NULL) {
        if (noDesbal == noDesbal->pai->fesq) {
            noDesbal->pai->fesq = dir;
        } 
        else {
            noDesbal->pai->fdir = dir;
        }
    }
    dir->fesq = noDesbal;
    noDesbal->pai = dir;
    noDesbal->fb = noDesbal->fb - 1 - (dir->fb > 0 ? dir->fb : 0);
    dir->fb = dir->fb - 1 + (noDesbal->fb < 0 ? noDesbal->fb : 0);
}

void imprimeOrdem(no *raiz) {
    if (raiz != NULL) {
        imprimeOrdem(raiz->fesq);
        int paiChave = (raiz->pai != NULL) ? raiz->pai->chave : -1000;
        printf("%d - %d - %d\n", raiz->chave, paiChave, raiz->fb);
        imprimeOrdem(raiz->fdir);
    }
}

void imprimePreOrdem(no *raiz) {
    if (raiz != NULL) {
        int paiChave = (raiz->pai != NULL) ? raiz->pai->chave : -1000;
        printf("%d - %d - %d\n", raiz->chave, paiChave, raiz->fb);
        imprimePreOrdem(raiz->fesq);
        imprimePreOrdem(raiz->fdir);
    }
}

no* getRaiz(avl *arv) {
    return arv->sentinela->fesq;
}

int getNumElementos(avl *arv) {
    return arv->numElementos;
}

int atualizaFB_remocao(avl *arv, no *pai, int chaveRemovida) {
    int nRotacoes;
    if (pai == NULL || pai == arv->sentinela)
        return 0;
    nRotacoes = 0;
    int altura_esq = -1; // Se o filho esquerdo for NULL, a altura é -1
    if (pai->fesq != NULL)
        altura_esq = alturaNo(pai->fesq);

    int altura_dir = -1; // Se o filho direito for NULL, a altura é -1
    if (pai->fdir != NULL)
        altura_dir = alturaNo(pai->fdir);

    int fb_pai = altura_dir - altura_esq;

    pai->fb = fb_pai;

    if (abs(fb_pai) > 1) {
        nRotacoes += balanceamento(arv, pai);
    }

    nRotacoes += atualizaFB_remocao(arv, pai->pai, chaveRemovida);
    return nRotacoes;
}

int alturaNo(no *raiz) {
    if (raiz == NULL)
        return -1;

    int altura_esq = alturaNo(raiz->fesq);
    int altura_dir = alturaNo(raiz->fdir);

    return 1 + (altura_esq > altura_dir ? altura_esq : altura_dir);
}