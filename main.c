#include <stdio.h>
#include <stdlib.h>
#include "rb.h"
#include "avl.h"
#include <time.h>

int main() {
  avl *arvore = criaArvore();
  if (arvore == NULL) {
      printf("Erro ao criar a árvore AVL.\n");
      return 1;
  }

  rb_arvore *arvore2 = criar_arvore();
  if (arvore2 == NULL) {
      printf("Erro ao criar a árvore Rubro-Negra\n");
      return 1;
  }
  
  int i, n, nRotacoesAvlIns, nRotacoesRbIns, nRotacoesAvlRem, nRotacoesRbRem;
  float tempAvlIns, tempRbIns, tempAvlRem, tempRbRem, tempAvlBusca, tempRbBusca;
  clock_t relogio;  // variavel que guarda clocks do processador
  n = 10000;
  
  nRotacoesAvlIns = 0;
  nRotacoesRbIns = 0;

  nRotacoesAvlRem = 0; 
  nRotacoesRbRem = 0;
    
  // Inserir nós na árvore AVL
  relogio = clock();
  for (i = 0; i < n; i++) {
    insereNo(arvore, i, &nRotacoesAvlIns);
  }
  relogio = clock() - relogio;  // clock atual - clock registrado anteriormente
  tempAvlIns = ((float)relogio)/CLOCKS_PER_SEC; // convertendo valor de clock para valor de tempo
  
  // Inserir nós na árvore Rubro-Negra
  relogio = clock();
  for (i = 0; i < n; i++) {
    inserir_no(arvore2, i, &nRotacoesRbIns);
  }
  relogio = clock() - relogio;
  tempRbIns = ((float)relogio)/CLOCKS_PER_SEC; 

  // Remover nós na árvore AVL
  relogio = clock();
  for (i = 0; i < n; i++) {
    removeNo(arvore, i, &nRotacoesAvlRem);
  }
  relogio = clock() - relogio;
  tempAvlRem = ((float)relogio)/CLOCKS_PER_SEC; 

  // Remover nós na árvore Rubro-Negra
  relogio = clock();
  for (i = 0; i < n; i++) {
    remover_no(arvore2, i, &nRotacoesRbRem);
  }
  relogio = clock() - relogio;
  tempRbRem = ((float)relogio)/CLOCKS_PER_SEC; 
  
  srand(time(0));
  // Buscar nós na árvore AVL
  relogio = clock();
  for(i = 0; i < 1000; i++) {
    buscarNo(arvore, rand() % 1000);
  }
  relogio = clock() - relogio;
  tempAvlBusca = ((float)relogio)/CLOCKS_PER_SEC; 

  srand(time(0));
  // Buscar nós na árvore Rubro-Negra
  relogio = clock();
  for(i = 0; i < 1000; i++) {
    buscar_no(arvore2, rand() % 1000);
  }
  relogio = clock() - relogio;
  tempRbBusca = ((float)relogio)/CLOCKS_PER_SEC; 

  printf("Rotações após inserções em AVL: %d\nTempo gasto: %f\n\n",nRotacoesAvlIns, tempAvlIns);
  printf("Rotações após inserções em Rubro-Negra: %d\nTempo gasto: %f\n\n",nRotacoesRbIns, tempRbIns);

  printf("Rotações após remoções em AVL: %d\nTempo gasto: %f\n\n",nRotacoesAvlRem, tempAvlRem);
  printf("Rotações após remoções em Rubro-Negra: %d\nTempo gasto: %f\n\n",nRotacoesRbRem, tempRbRem);

  printf("Tempo para busca de 1000 elementos aleátorios em AVL: %f\n\n",tempAvlBusca);
  printf("Tempo para busca de 1000 elementos aleátorios em Rubro_negra: %f\n\n",tempRbBusca);
  
  // Liberar a memória da árvore
  free(arvore);
  free(arvore2);

  return 0;
}