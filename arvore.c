#include "arvore.h"
#include "arquivo.h"  // Necessário para usar escreve_cabecalho_indice()
#include <stdio.h>
#include <stdlib.h>

/* =================== Implementações das Funções da Árvore 2‑3 =================== */

/**
 * //DESCRICAO: Cria um novo nó da árvore 2‑3.
 * //ENTRADA: int chave, int filho0, int filho1, int filho2, int ptDado, int nroChaves.
 * //SAIDA: Ponteiro para o nó criado.
 * //PRE-CONDICAO: Nenhuma.
 * //POS-CONDICAO: Nó criado e inicializado.
 */
Arvore23* criar_no_arvore23(int chave, int filho0, int filho1, int filho2, int ptDado, int nroChaves) {
    Arvore23 *no = (Arvore23*) malloc(sizeof(Arvore23));
    no->nroChaves = nroChaves;
    no->chaves[0] = chave;
    no->ptDados[0] = ptDado;
    if(nroChaves == 2) {
        no->chaves[1] = -1;
        no->ptDados[1] = -1;
    }
    no->filhos[0] = filho0;
    no->filhos[1] = filho1;
    no->filhos[2] = filho2;
    return no;
}

/**
 * //DESCRICAO: Lê um nó da árvore 2‑3 do arquivo de índices.
 * //ENTRADA: FILE *arq_indice, int pos.
 * //SAIDA: Ponteiro para o nó lido.
 * //PRE-CONDICAO: Arquivo aberto e posição válida.
 * //POS-CONDICAO: Nó retornado.
 */
Arvore23* le_no_arvore23(FILE *arq_indice, int pos) {
    Arvore23 *no = (Arvore23*) malloc(sizeof(Arvore23));
    long offset = sizeof(CabecalhoIndice) + pos * sizeof(Arvore23);
    fseek(arq_indice, offset, SEEK_SET);
    fread(no, sizeof(Arvore23), 1, arq_indice);
    return no;
}

/**
 * //DESCRICAO: Escreve um nó da árvore 2‑3 no arquivo de índices.
 * //ENTRADA: FILE *arq_indice, Arvore23 *no, int pos.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo aberto e nó válido.
 * //POS-CONDICAO: Nó escrito no arquivo.
 */
void escreve_no_arvore23_indice(FILE *arq_indice, Arvore23 *no, int pos) {
    long offset = sizeof(CabecalhoIndice) + pos * sizeof(Arvore23);
    fseek(arq_indice, offset, SEEK_SET);
    fwrite(no, sizeof(Arvore23), 1, arq_indice);
    fflush(arq_indice);
}

/**
 * //DESCRICAO: Insere uma chave em um nó da árvore 2‑3.
 * //ENTRADA: Arvore23 *arvore, int cab_dado_topo, int chave, int subarvore.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nó existente.
 * //POS-CONDICAO: Nó atualizado com a nova chave.
 */
void inserir_chave(Arvore23 *arvore, int cab_dado_topo, int chave, int subarvore){
    if(arvore->chaves[0] < chave){
        arvore->chaves[1] = chave;
        arvore->ptDados[1] = cab_dado_topo;
        arvore->filhos[2] = subarvore;
    }
    else{
        arvore->chaves[1] = arvore->chaves[0];
        arvore->ptDados[1] = arvore->ptDados[0];
        arvore->ptDados[0] = cab_dado_topo;
        arvore->chaves[0] = chave;
        if(arvore->filhos[1] != -1)
            arvore->filhos[2] = arvore->filhos[1];
        arvore->filhos[1] = subarvore;
    }
    arvore->nroChaves = 2;
}

/**
 * //DESCRICAO: Verifica se o nó da árvore é folha.
 * //ENTRADA: Arvore23 *arvore.
 * //SAIDA: Retorna 1 se for folha, 0 caso contrário.
 * //PRE-CONDICAO: Nó existente.
 * //POS-CONDICAO: Resultado da verificação retornado.
 */
int eh_folha(Arvore23 *arvore){
    return (arvore->filhos[0] == -1 && arvore->filhos[1] == -1 && arvore->filhos[2] == -1);
}

/**
 * //DESCRICAO: Realiza o split (divisão) de um nó da árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int cab_dado_topo, Arvore23 *arvore, int raiz, int chave, int subarvore, int *chave_promovida, int *ponteiro_de_dados.
 * //SAIDA: Índice do novo nó criado.
 * //PRE-CONDICAO: Nó cheio que necessita de divisão.
 * //POS-CONDICAO: Nó dividido e chave promovida.
 */
int split(FILE *arq_indice, CabecalhoIndice *cab_indice, int cab_dado_topo, Arvore23 *arvore, int raiz, int chave, int subarvore, int *chave_promovida, int *ponteiro_de_dados){
    Arvore23 *novoNo = (Arvore23*) malloc(sizeof(Arvore23));
    int novoNoIdx;
    int chaveMeio;
    int posInserir;

    if(*ponteiro_de_dados != -1)
        cab_dado_topo = *ponteiro_de_dados;

    if(chave < arvore->chaves[0]){
        posInserir = 0;
        chaveMeio = arvore->chaves[0];
        *ponteiro_de_dados = arvore->ptDados[0];
    }
    else if(arvore->nroChaves == 1 || chave < arvore->chaves[1]){
        posInserir = 1;
        chaveMeio = chave;
        *ponteiro_de_dados = cab_dado_topo;
    }
    else{
        posInserir = 2;
        chaveMeio = arvore->chaves[1];
        *ponteiro_de_dados = arvore->ptDados[1];
    }

    novoNo->nroChaves = 1;
    novoNo->chaves[0] = (posInserir == 2) ? chave : arvore->chaves[1];
    novoNo->ptDados[0] = (posInserir == 2) ? cab_dado_topo : arvore->ptDados[1];
    novoNo->filhos[0] = (posInserir == 2) ? arvore->filhos[2] : subarvore;
    novoNo->filhos[1] = (posInserir == 2) ? subarvore : arvore->filhos[2];
    novoNo->filhos[2] = -1;

    if(posInserir == 0){
        arvore->chaves[0] = chave;
        arvore->ptDados[0] = cab_dado_topo;
    }
    arvore->nroChaves = 1;
    arvore->filhos[2] = -1;

    escreve_no_arvore23_indice(arq_indice, arvore, raiz);

    novoNoIdx = cab_indice->topo;
    cab_indice->topo++;
    escreve_no_arvore23_indice(arq_indice, novoNo, novoNoIdx);

    *chave_promovida = chaveMeio;

    free(novoNo);
    return novoNoIdx;
}

/**
 * //DESCRICAO: Função auxiliar para inserção recursiva na árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int raiz, int cab_dado_topo, int chave, int *chave_promovida, int *ponteiro_de_dados.
 * //SAIDA: Índice do novo nó se houver promoção, ou -1.
 * //PRE-CONDICAO: Árvore existente.
 * //POS-CONDICAO: Chave inserida com possível promoção.
 */
int inserir_aux(FILE* arq_indice, CabecalhoIndice *cab_indice, int raiz, int cab_dado_topo, int chave, int *chave_promovida, int *ponteiro_de_dados){
    Arvore23 *arvore = le_no_arvore23(arq_indice, raiz);

    if(eh_folha(arvore)){
        if(arvore->nroChaves == 1){
            inserir_chave(arvore, cab_dado_topo, chave, -1);
            escreve_no_arvore23_indice(arq_indice, arvore, raiz);
            free(arvore);
            return -1;
        }
        else{
            int novoNoIdx = split(arq_indice, cab_indice, cab_dado_topo, arvore, raiz, chave, -1, chave_promovida, ponteiro_de_dados);
            free(arvore);
            return novoNoIdx;
        }
    }
    else{
        int ponteiro_aux = -1;
        int chave_auxiliar;
        int ptr_dados_aux = -1;

        if(chave < arvore->chaves[0])
            ponteiro_aux = inserir_aux(arq_indice, cab_indice, arvore->filhos[0], cab_dado_topo, chave, &chave_auxiliar, &ptr_dados_aux);
        else if(arvore->nroChaves == 1 || chave < arvore->chaves[1])
            ponteiro_aux = inserir_aux(arq_indice, cab_indice, arvore->filhos[1], cab_dado_topo, chave, &chave_auxiliar, &ptr_dados_aux);
        else
            ponteiro_aux = inserir_aux(arq_indice, cab_indice, arvore->filhos[2], cab_dado_topo, chave, &chave_auxiliar, &ptr_dados_aux);

        if(ponteiro_aux == -1) {
            free(arvore);
            return -1;
        }

        if(arvore->nroChaves == 1){
            inserir_chave(arvore, cab_dado_topo, chave_auxiliar, ponteiro_aux);
            if(ptr_dados_aux != -1){
                if(arvore->chaves[0] == chave_auxiliar)
                    arvore->ptDados[0] = ptr_dados_aux;
                else if(arvore->nroChaves == 2)
                    arvore->ptDados[1] = ptr_dados_aux;
            }
            escreve_no_arvore23_indice(arq_indice, arvore, raiz );
            free(arvore);
            return -1;
        }
        else {
            ponteiro_aux = split(arq_indice, cab_indice, cab_dado_topo, arvore, raiz, chave_auxiliar, ponteiro_aux, chave_promovida, &ptr_dados_aux);
            *ponteiro_de_dados = ptr_dados_aux;
            free(arvore);
            return ponteiro_aux;
        }
    }
}

/**
 * //DESCRICAO: Busca uma chave na árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, int pos, int chave, int *ptDados.
 * //SAIDA: Posição do nó onde a chave foi encontrada ou -1.
 * //PRE-CONDICAO: Árvore indexada.
 * //POS-CONDICAO: Resultado da busca.
 */
int buscar_no_arvore23(FILE *arq_indice, int pos, int chave, int *ptDados) {
    if(pos == -1) return -1;
    Arvore23 *no = le_no_arvore23(arq_indice, pos);

    if(no->nroChaves >= 1 && no->chaves[0] == chave) {
        *ptDados = no->ptDados[0];
        free(no);
        return pos;
    }
    if(no->nroChaves == 2 && no->chaves[1] == chave) {
        *ptDados = no->ptDados[1];
        free(no);
        return pos;
    }
    if(eh_folha(no)) {
        free(no);
        return -1;
    }
    int ret;
    if(chave < no->chaves[0])
        ret = buscar_no_arvore23(arq_indice, no->filhos[0], chave, ptDados);
    else if(no->nroChaves == 1 || chave < no->chaves[1])
        ret = buscar_no_arvore23(arq_indice, no->filhos[1], chave, ptDados);
    else
        ret = buscar_no_arvore23(arq_indice, no->filhos[2], chave, ptDados);
    free(no);
    return ret;
}

/**
 * //DESCRICAO: Realiza o percurso in‑ordem da árvore 2‑3 para listagem.
 * //ENTRADA: FILE *arq_indice, FILE *arq_dado, int pos.
 * //SAIDA: void.
 * //PRE-CONDICAO: Árvore não vazia.
 * //POS-CONDICAO: Livros listados em ordem.
 */
void inOrder(FILE *arq_indice, FILE *arq_dado, int pos) {
    if(pos == -1) return;
    Arvore23 *no = le_no_arvore23(arq_indice, pos);
    if(no->nroChaves == 1) {
        if(no->filhos[0] != -1)
            inOrder(arq_indice, arq_dado, no->filhos[0]);
        Livro livro = ler_livro(arq_dado, no->ptDados[0]);
        printf("Código: %d - Título: %s\n", livro.codigo, livro.titulo);
        if(no->filhos[1] != -1)
            inOrder(arq_indice, arq_dado, no->filhos[1]);
    } else if(no->nroChaves == 2) {
        if(no->filhos[0] != -1)
            inOrder(arq_indice, arq_dado, no->filhos[0]);
        Livro livro = ler_livro(arq_dado, no->ptDados[0]);
        printf("Código: %d - Título: %s\n", livro.codigo, livro.titulo);
        if(no->filhos[1] != -1)
            inOrder(arq_indice, arq_dado, no->filhos[1]);
        livro = ler_livro(arq_dado, no->ptDados[1]);
        printf("Código: %d - Título: %s\n", livro.codigo, livro.titulo);
        if(no->filhos[2] != -1)
            inOrder(arq_indice, arq_dado, no->filhos[2]);
    }
    free(no);
}

/**
 * //DESCRICAO: Imprime a árvore 2‑3 por níveis.
 * //ENTRADA: FILE *arq_indice, int raiz.
 * //SAIDA: void.
 * //PRE-CONDICAO: Árvore existente.
 * //POS-CONDICAO: Árvore impressa por níveis.
 */
void imprimir_arvore_niveis(FILE *arq_indice, int raiz) {
    typedef struct {
        int index;
        int nivel;
    } Nodo;

    Nodo fila[1000];
    int inicio = 0, fim = 0;
    fila[fim].index = raiz;
    fila[fim].nivel = 0;
    fim++;
    int nivel_atual = 0;

    while(inicio < fim) {
        Nodo atual = fila[inicio++];
        if(atual.nivel != nivel_atual) {
            printf("\n");
            nivel_atual = atual.nivel;
        }
        Arvore23 *no = le_no_arvore23(arq_indice, atual.index);
        printf("[Nó %d: ", atual.index);
        for(int i = 0; i < no->nroChaves; i++){
            printf("%d ", no->chaves[i]);
        }
        printf("] ");
        if(no->filhos[0] != -1) {
            fila[fim].index = no->filhos[0];
            fila[fim].nivel = atual.nivel + 1;
            fim++;
        }
        if(no->filhos[1] != -1) {
            fila[fim].index = no->filhos[1];
            fila[fim].nivel = atual.nivel + 1;
            fim++;
        }
        if(no->nroChaves == 2 && no->filhos[2] != -1) {
            fila[fim].index = no->filhos[2];
            fila[fim].nivel = atual.nivel + 1;
            fim++;
        }
        free(no);
    }
    printf("\n");
}

/* =================== Funções de Remoção =================== */

/**
 * //DESCRICAO: Remove uma chave da árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int codigo, int *chave_substituir, int *ponteiro_substituir, int *flag_remove.
 * //SAIDA: 1 se removido, -1 se não encontrado.
 * //PRE-CONDICAO: Árvore indexada.
 * //POS-CONDICAO: Chave removida ou ajustada.
 */
int remover_chave_arvore23(FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int codigo, int *chave_substituir, int *ponteiro_substituir, int *flag_remove) {
  if (no_idx == -1) {
    return -1;
  }

  Arvore23 *no = le_no_arvore23(arq_indice, no_idx);

  int i;
  int pos_chave = -1;
  for (i = 0; i < no->nroChaves; i++) {
    if (no->chaves[i] == codigo) {
      pos_chave = i;
      break;
    }
  }

  if (pos_chave != -1) {
    *flag_remove = 1;
    if (eh_folha(no)) {
      *ponteiro_substituir = no->ptDados[pos_chave];
      remover_de_no_folha(no, codigo);
      escreve_no_arvore23_indice(arq_indice, no, no_idx);
    } else {
      *ponteiro_substituir = no->ptDados[pos_chave];
      remover_de_no_interno(arq_indice, cab_indice, no_idx, no, pos_chave);
    }
  } else {
    int direcao = 0;
    if (no->nroChaves == 1) {
      direcao = (codigo < no->chaves[0]) ? 0 : 1;
    } else {
      if (codigo < no->chaves[0])
        direcao = 0;
      else if (codigo < no->chaves[1])
        direcao = 1;
      else
        direcao = 2;
    }

    int filho_idx = no->filhos[direcao];
    remover_chave_arvore23(arq_indice, cab_indice, filho_idx, codigo, chave_substituir, ponteiro_substituir, flag_remove);

    if (filho_idx != -1) {
      Arvore23 *filho_no = le_no_arvore23(arq_indice, filho_idx);
      if (filho_no->nroChaves == 0) {
        free(filho_no);
        ajustar_no(arq_indice, cab_indice, no_idx, direcao);
      } else {
        free(filho_no);
      }
    }
  }

  free(no);
  return 1;
}

/**
 * //DESCRICAO: Remove uma chave de um nó folha da árvore 2‑3.
 * //ENTRADA: Arvore23 *no, int codigo.
 * //SAIDA: Retorna 1 se removido, 0 caso contrário.
 * //PRE-CONDICAO: Nó folha válido.
 * //POS-CONDICAO: Chave removida do nó.
 */
int remover_de_no_folha(Arvore23 *no, int codigo) {
  int i, pos = -1;
  for (i = 0; i < no->nroChaves; i++) {
    if (no->chaves[i] == codigo) {
      pos = i;
      break;
    }
  }
  if (pos == -1)
    return 0;

  for (i = pos; i < no->nroChaves - 1; i++) {
    no->chaves[i] = no->chaves[i + 1];
    no->ptDados[i] = no->ptDados[i + 1];
  }
  no->chaves[no->nroChaves - 1] = -1;
  no->ptDados[no->nroChaves - 1] = -1;
  no->nroChaves--;

  return 1;
}

/**
 * //DESCRICAO: Remove uma chave de um nó interno da árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, Arvore23 *no, int pos_chave.
 * //SAIDA: Retorna 1 se removido com sucesso, 0 caso contrário.
 * //PRE-CONDICAO: Nó interno válido.
 * //POS-CONDICAO: Chave removida e substituída.
 */
int remover_de_no_interno(FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, Arvore23 *no, int pos_chave) {
  int filho_pre_idx = no->filhos[pos_chave];

  int atual_idx = filho_pre_idx;
  Arvore23 *atual_no = le_no_arvore23(arq_indice, atual_idx);

  while (!eh_folha(atual_no)) {
    int prox_idx = atual_no->filhos[atual_no->nroChaves];
    free(atual_no);
    atual_no = le_no_arvore23(arq_indice, prox_idx);
    atual_idx = prox_idx;
  }
  int pos_pred = atual_no->nroChaves - 1;

  no->chaves[pos_chave] = atual_no->chaves[pos_pred];
  no->ptDados[pos_chave] = atual_no->ptDados[pos_pred];
  escreve_no_arvore23_indice(arq_indice, no, no_idx);

  int codigo_pred = atual_no->chaves[pos_pred];
  remover_de_no_folha(atual_no, codigo_pred);
  escreve_no_arvore23_indice(arq_indice, atual_no, atual_idx);
  free(atual_no);

  Arvore23 *check_no = le_no_arvore23(arq_indice, atual_idx);
  if (check_no->nroChaves == 0) {
    free(check_no);
  } else {
    free(check_no);
  }

  return 1;
}

/**
 * //DESCRICAO: Ajusta um nó que sofreu underflow após remoção.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int pos_filho.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nó com underflow.
 * //POS-CONDICAO: Nó ajustado.
 */
void ajustar_no(FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int pos_filho) {
  if (no_idx == -1)
    return;

  Arvore23 *no = le_no_arvore23(arq_indice, no_idx);
  int filho_idx = no->filhos[pos_filho];

  if (pos_filho > 0) {
    int irmao_esq_idx = no->filhos[pos_filho - 1];
    if (irmao_esq_idx != -1) {
      Arvore23 *irmao_esq = le_no_arvore23(arq_indice, irmao_esq_idx);
      if (irmao_esq->nroChaves > 1) {
        pegar_emprestado_da_esquerda(arq_indice, cab_indice, no, no_idx, pos_filho);
        free(irmao_esq);
        escreve_no_arvore23_indice(arq_indice, no, no_idx);
        free(no);
        return;
      }
      free(irmao_esq);
    }
  }
  if (pos_filho < no->nroChaves) {
    int irmao_dir_idx = no->filhos[pos_filho + 1];
    if (irmao_dir_idx != -1) {
      Arvore23 *irmao_dir = le_no_arvore23(arq_indice, irmao_dir_idx);
      if (irmao_dir->nroChaves > 1) {
        pegar_emprestado_da_direita(arq_indice, cab_indice, no, no_idx, pos_filho);
        free(irmao_dir);
        escreve_no_arvore23_indice(arq_indice, no, no_idx);
        free(no);
        return;
      }
      free(irmao_dir);
    }
  }

  if (pos_filho == no->nroChaves) {
    fundir_nos(arq_indice, cab_indice, no_idx, pos_filho - 1, pos_filho);
  } else {
    fundir_nos(arq_indice, cab_indice, no_idx, pos_filho, pos_filho + 1);
  }

  escreve_no_arvore23_indice(arq_indice, no, no_idx);
  free(no);
}

/**
 * //DESCRICAO: Pega emprestado uma chave do irmão à esquerda para corrigir underflow.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, Arvore23 *no, int no_idx, int pos_filho.
 * //SAIDA: void.
 * //PRE-CONDICAO: Irmão à esquerda disponível.
 * //POS-CONDICAO: Underflow corrigido.
 */
void pegar_emprestado_da_esquerda(FILE *arq_indice, CabecalhoIndice *cab_indice, Arvore23 *no, int no_idx, int pos_filho) {
  int filho_idx = no->filhos[pos_filho];
  int irmao_idx = no->filhos[pos_filho - 1];

  Arvore23 *filho = le_no_arvore23(arq_indice, filho_idx);
  Arvore23 *irmao = le_no_arvore23(arq_indice, irmao_idx);

  inserir_chave(filho, no->ptDados[pos_filho - 1], no->chaves[pos_filho - 1], -1);
  filho->filhos[2] = filho->filhos[1];
  filho->filhos[1] = -1;

  no->chaves[pos_filho - 1] = irmao->chaves[irmao->nroChaves - 1];
  no->ptDados[pos_filho - 1] = irmao->ptDados[irmao->nroChaves - 1];

  irmao->chaves[irmao->nroChaves - 1] = -1;
  irmao->ptDados[irmao->nroChaves - 1] = -1;
  irmao->nroChaves--;

  escreve_no_arvore23_indice(arq_indice, filho, filho_idx);
  escreve_no_arvore23_indice(arq_indice, irmao, irmao_idx);
  free(filho);
  free(irmao);
}

/**
 * //DESCRICAO: Pega emprestado uma chave do irmão à direita para corrigir underflow.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, Arvore23 *no, int no_idx, int pos_filho.
 * //SAIDA: void.
 * //PRE-CONDICAO: Irmão à direita disponível.
 * //POS-CONDICAO: Underflow corrigido.
 */
void pegar_emprestado_da_direita(FILE *arq_indice, CabecalhoIndice *cab_indice, Arvore23 *no, int no_idx, int pos_filho) {
  int filho_idx = no->filhos[pos_filho];
  int irmao_idx = no->filhos[pos_filho + 1];

  Arvore23 *filho = le_no_arvore23(arq_indice, filho_idx);
  Arvore23 *irmao = le_no_arvore23(arq_indice, irmao_idx);

  inserir_chave(filho, no->ptDados[pos_filho], no->chaves[pos_filho], -1);

  no->chaves[pos_filho] = irmao->chaves[0];
  no->ptDados[pos_filho] = irmao->ptDados[0];

  int i;
  for (i = 0; i < irmao->nroChaves - 1; i++) {
    irmao->chaves[i] = irmao->chaves[i + 1];
    irmao->ptDados[i] = irmao->ptDados[i + 1];
  }
  irmao->chaves[irmao->nroChaves - 1] = -1;
  irmao->ptDados[irmao->nroChaves - 1] = -1;
  irmao->nroChaves--;

  escreve_no_arvore23_indice(arq_indice, filho, filho_idx);
  escreve_no_arvore23_indice(arq_indice, irmao, irmao_idx);
  free(filho);
  free(irmao);
}

/**
 * //DESCRICAO: Fundir dois nós irmãos para corrigir underflow.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int pos_filho_esq, int pos_filho_dir.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nós irmãos disponíveis para fusão.
 * //POS-CONDICAO: Nós fundidos e árvore ajustada.
 */
void fundir_nos(FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int pos_filho_esq, int pos_filho_dir) {
  Arvore23 *no_pai = le_no_arvore23(arq_indice, no_idx);
  int esq_idx = no_pai->filhos[pos_filho_esq];
  int dir_idx = no_pai->filhos[pos_filho_dir];

  Arvore23 *no_esq = le_no_arvore23(arq_indice, esq_idx);
  Arvore23 *no_dir = le_no_arvore23(arq_indice, dir_idx);

  int chave_fusao = no_pai->chaves[pos_filho_esq];
  int dado_fusao = no_pai->ptDados[pos_filho_esq];

  if (no_esq->nroChaves == 0) {
    no_esq->chaves[0] = chave_fusao;
    no_esq->ptDados[0] = dado_fusao;
    no_esq->nroChaves = 1;
  } else {
    no_esq->chaves[no_esq->nroChaves] = chave_fusao;
    no_esq->ptDados[no_esq->nroChaves] = dado_fusao;
    no_esq->nroChaves++;
  }
  int i;
  for (i = 0; i < no_dir->nroChaves; i++) {
    no_esq->chaves[no_esq->nroChaves] = no_dir->chaves[i];
    no_esq->ptDados[no_esq->nroChaves] = no_dir->ptDados[i];
    no_esq->nroChaves++;
  }

  for (i = 0; i <= no_dir->nroChaves; i++) {
    no_esq->filhos[no_esq->nroChaves - (no_dir->nroChaves - i)] =
        no_dir->filhos[i];
  }

  for (i = pos_filho_esq; i < no_pai->nroChaves - 1; i++) {
    no_pai->chaves[i] = no_pai->chaves[i + 1];
    no_pai->ptDados[i] = no_pai->ptDados[i + 1];
    no_pai->filhos[i + 1] = no_pai->filhos[i + 2];
  }
  no_pai->chaves[no_pai->nroChaves - 1] = -1;
  no_pai->ptDados[no_pai->nroChaves - 1] = -1;
  no_pai->filhos[no_pai->nroChaves] = -1;
  no_pai->nroChaves--;

  escreve_no_arvore23_indice(arq_indice, no_esq, esq_idx);
  escreve_no_arvore23_indice(arq_indice, no_pai, no_idx);

  marcar_no_livre(arq_indice, cab_indice, dir_idx);

  free(no_esq);
  free(no_dir);
  free(no_pai);
}

/**
 * //DESCRICAO: Marca um nó da árvore como livre.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab, int no_idx.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nó existente.
 * //POS-CONDICAO: Nó marcado como livre.
 */
void marcar_no_livre(FILE *arq_indice, CabecalhoIndice *cab, int no_idx) {
  if (no_idx < 0)
    return;

  Arvore23 *no = le_no_arvore23(arq_indice, no_idx);

  no->nroChaves = 0;

  no->chaves[0] = -1;
  no->chaves[1] = -1;
  no->ptDados[0] = -1;
  no->ptDados[1] = -1;

  no->filhos[0] = cab->livre;
  no->filhos[1] = -1;
  no->filhos[2] = -1;

  escreve_no_arvore23_indice(arq_indice, no, no_idx);

  cab->livre = no_idx;
  escreve_cabecalho_indice(arq_indice, cab);

  free(no);
}
