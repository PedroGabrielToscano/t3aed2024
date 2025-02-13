#ifndef CADASTRO_H
#define CADASTRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_ARQUIVO_INDICE "indice.dat"
#define NOME_ARQUIVO_DADO   "dado.dat"

/* ============================ Estruturas ============================== */

/**
 * @brief Estrutura que representa um livro.
 */
//DESCRICAO: Define os dados do livro.
//ENTRADA: Nenhuma.
//SAIDA: Estrutura Livro.
//PRE-CONDICAO: Nenhuma.
//POS-CONDICAO: Estrutura disponível para uso.
typedef struct {
    int codigo;
    char titulo[151];
    char autor[201];
    char editora[51];
    int edicao;
    int ano;
    double preco;
    int estoque;
} Livro;

/**
 * @brief Cabeçalho do arquivo de índices (árvore 2‑3).
 */
//DESCRICAO: Armazena os dados do cabeçalho do arquivo de índices.
//ENTRADA: Nenhuma.
//SAIDA: Estrutura CabecalhoIndice.
//PRE-CONDICAO: Nenhuma.
//POS-CONDICAO: Estrutura disponível para controle da árvore.
typedef struct {
    int raiz;   // índice do nó raiz da árvore 2‑3
    int topo;   // próximo índice disponível para escrita
    int livre;  // índice do primeiro nó livre (lista de nós removidos)
} CabecalhoIndice;

/**
 * @brief Nó da árvore 2‑3.
 */
//DESCRICAO: Estrutura que representa um nó (interno ou folha) da árvore 2‑3.
//ENTRADA: Nenhuma.
//SAIDA: Estrutura Arvore23.
//PRE-CONDICAO: Nenhuma.
//POS-CONDICAO: Nó criado para armazenar chaves, ponteiros e filhos.
typedef struct {
    int chaves[2];   // no máximo 2 chaves (código do livro)
    int ptDados[2];  // ponteiro(s) para o registro no arquivo de dados
    int filhos[3];   // índices dos filhos (caso não seja folha, -1 se não existir)
    int nroChaves;   // número de chaves armazenadas (1 ou 2)
} Arvore23;

/**
 * @brief Cabeçalho do arquivo de dados.
 */
//DESCRICAO: Armazena os dados do cabeçalho do arquivo de dados.
//ENTRADA: Nenhuma.
//SAIDA: Estrutura CabecalhoDado.
//PRE-CONDICAO: Nenhuma.
//POS-CONDICAO: Estrutura disponível para controle dos registros.
typedef struct {
    int topo;   // próximo índice disponível para escrita
    int livre;  // índice do primeiro registro livre
} CabecalhoDado;

#endif // CADASTRO_H
