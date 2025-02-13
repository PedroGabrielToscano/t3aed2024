#ifndef ARVORE_H
#define ARVORE_H

#include "cadastro.h"

/* ==================== Protótipos das Funções da Árvore 2‑3 ==================== */

/**
 * //DESCRICAO: Cria um novo nó da árvore 2‑3.
 * //ENTRADA: int chave, int filho0, int filho1, int filho2, int ptDado, int nroChaves.
 * //SAIDA: Ponteiro para o nó criado.
 * //PRE-CONDICAO: Nenhuma.
 * //POS-CONDICAO: Nó criado e inicializado.
 */
Arvore23* criar_no_arvore23(int chave, int filho0, int filho1, int filho2, int ptDado, int nroChaves);

/**
 * //DESCRICAO: Lê um nó da árvore 2‑3 do arquivo de índices.
 * //ENTRADA: FILE *arq_indice, int pos.
 * //SAIDA: Ponteiro para o nó lido.
 * //PRE-CONDICAO: Arquivo aberto e posição válida.
 * //POS-CONDICAO: Nó retornado.
 */
Arvore23* le_no_arvore23(FILE *arq_indice, int pos);

/**
 * //DESCRICAO: Escreve um nó da árvore 2‑3 no arquivo de índices.
 * //ENTRADA: FILE *arq_indice, Arvore23 *no, int pos.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo aberto e nó válido.
 * //POS-CONDICAO: Nó escrito no arquivo.
 */
void escreve_no_arvore23_indice(FILE *arq_indice, Arvore23 *no, int pos);

/**
 * //DESCRICAO: Insere uma chave em um nó da árvore 2‑3.
 * //ENTRADA: Arvore23 *arvore, int cab_dado_topo, int chave, int subarvore.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nó existente.
 * //POS-CONDICAO: Nó atualizado com a nova chave.
 */
void inserir_chave(Arvore23 *arvore, int cab_dado_topo, int chave, int subarvore);

/**
 * //DESCRICAO: Verifica se o nó da árvore é folha.
 * //ENTRADA: Arvore23 *arvore.
 * //SAIDA: Retorna 1 se for folha, 0 caso contrário.
 * //PRE-CONDICAO: Nó existente.
 * //POS-CONDICAO: Resultado da verificação retornado.
 */
int eh_folha(Arvore23 *arvore);

/**
 * //DESCRICAO: Realiza o split (divisão) de um nó da árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int cab_dado_topo, Arvore23 *arvore, int raiz, int chave, int subarvore, int *chave_promovida, int *ponteiro_de_dados.
 * //SAIDA: Índice do novo nó criado.
 * //PRE-CONDICAO: Nó cheio e necessidade de divisão.
 * //POS-CONDICAO: Nó dividido e chave promovida.
 */
int split(FILE *arq_indice, CabecalhoIndice *cab_indice, int cab_dado_topo, Arvore23 *arvore, int raiz, int chave, int subarvore, int *chave_promovida, int *ponteiro_de_dados);

/**
 * //DESCRICAO: Função auxiliar para inserção recursiva na árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int raiz, int cab_dado_topo, int chave, int *chave_promovida, int *ponteiro_de_dados.
 * //SAIDA: Índice do novo nó se houver promoção, ou -1.
 * //PRE-CONDICAO: Árvore existente.
 * //POS-CONDICAO: Chave inserida com possível promoção.
 */
int inserir_aux(FILE* arq_indice, CabecalhoIndice *cab_indice, int raiz, int cab_dado_topo, int chave, int *chave_promovida, int *ponteiro_de_dados);

/**
 * //DESCRICAO: Busca uma chave na árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, int pos, int chave, int *ptDados.
 * //SAIDA: Posição do nó onde a chave foi encontrada ou -1.
 * //PRE-CONDICAO: Árvore indexada.
 * //POS-CONDICAO: Resultado da busca.
 */
int buscar_no_arvore23(FILE *arq_indice, int pos, int chave, int *ptDados);

/**
 * //DESCRICAO: Realiza o percurso in‑ordem da árvore 2‑3 para listagem.
 * //ENTRADA: FILE *arq_indice, FILE *arq_dado, int pos.
 * //SAIDA: void.
 * //PRE-CONDICAO: Árvore não vazia.
 * //POS-CONDICAO: Livros listados em ordem.
 */
void inOrder(FILE *arq_indice, FILE *arq_dado, int pos);

/**
 * //DESCRICAO: Imprime a árvore 2‑3 por níveis.
 * //ENTRADA: FILE *arq_indice, int raiz.
 * //SAIDA: void.
 * //PRE-CONDICAO: Árvore existente.
 * //POS-CONDICAO: Árvore impressa por níveis.
 */
void imprimir_arvore_niveis(FILE *arq_indice, int raiz);

/* Funções de remoção */

/**
 * //DESCRICAO: Remove uma chave da árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int codigo, int *chave_substituir, int *ponteiro_substituir, int *flag_remove.
 * //SAIDA: 1 se removido, -1 se não encontrado.
 * //PRE-CONDICAO: Árvore indexada.
 * //POS-CONDICAO: Chave removida ou ajustada.
 */
int remover_chave_arvore23(FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int codigo, int *chave_substituir, int *ponteiro_substituir, int *flag_remove);

/**
 * //DESCRICAO: Remove uma chave de um nó folha da árvore 2‑3.
 * //ENTRADA: Arvore23 *no, int codigo.
 * //SAIDA: 1 se removido com sucesso, 0 caso contrário.
 * //PRE-CONDICAO: Nó folha válido.
 * //POS-CONDICAO: Chave removida do nó.
 */
int remover_de_no_folha(Arvore23 *no, int codigo);

/**
 * //DESCRICAO: Remove uma chave de um nó interno da árvore 2‑3.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, Arvore23 *no, int pos_chave.
 * //SAIDA: 1 se removido com sucesso, 0 caso contrário.
 * //PRE-CONDICAO: Nó interno válido.
 * //POS-CONDICAO: Chave removida e substituída.
 */
int remover_de_no_interno(FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, Arvore23 *no, int pos_chave);

/**
 * //DESCRICAO: Ajusta um nó que sofreu underflow após remoção.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int pos_filho.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nó com underflow.
 * //POS-CONDICAO: Nó ajustado.
 */
void ajustar_no(FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int pos_filho);

/**
 * //DESCRICAO: Pega emprestado uma chave do irmão à esquerda para corrigir underflow.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, Arvore23 *no, int no_idx, int pos_filho.
 * //SAIDA: void.
 * //PRE-CONDICAO: Irmão à esquerda disponível.
 * //POS-CONDICAO: Underflow corrigido.
 */
void pegar_emprestado_da_esquerda(FILE *arq_indice, CabecalhoIndice *cab_indice, Arvore23 *no, int no_idx, int pos_filho);

/**
 * //DESCRICAO: Pega emprestado uma chave do irmão à direita para corrigir underflow.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, Arvore23 *no, int no_idx, int pos_filho.
 * //SAIDA: void.
 * //PRE-CONDICAO: Irmão à direita disponível.
 * //POS-CONDICAO: Underflow corrigido.
 */
void pegar_emprestado_da_direita(FILE *arq_indice, CabecalhoIndice *cab_indice, Arvore23 *no, int no_idx, int pos_filho);

/**
 * //DESCRICAO: Fundir dois nós irmãos para corrigir underflow.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int pos_filho_esq, int pos_filho_dir.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nós irmãos disponíveis para fusão.
 * //POS-CONDICAO: Nós fundidos e árvore ajustada.
 */
void fundir_nos(FILE *arq_indice, CabecalhoIndice *cab_indice, int no_idx, int pos_filho_esq, int pos_filho_dir);

/**
 * //DESCRICAO: Marca um nó da árvore como livre.
 * //ENTRADA: FILE *arq_indice, CabecalhoIndice *cab, int no_idx.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nó existente.
 * //POS-CONDICAO: Nó marcado como livre.
 */
void marcar_no_livre(FILE *arq_indice, CabecalhoIndice *cab, int no_idx);

#endif // ARVORE_H
