#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "cadastro.h"

/* ==================== Prot�tipos das Fun��es de Manipula��o de Arquivos ==================== */

/**
 * //DESCRICAO: L� o cabe�alho do arquivo de �ndices.
 * //ENTRADA: FILE *arq.
 * //SAIDA: Ponteiro para o cabe�alho lido.
 * //PRE-CONDICAO: Arquivo aberto.
 * //POS-CONDICAO: Cabe�alho retornado.
 */
CabecalhoIndice* le_cabecalho_indice(FILE *arq);

/**
 * //DESCRICAO: Escreve o cabe�alho do arquivo de �ndices.
 * //ENTRADA: FILE *arq, CabecalhoIndice *cab.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo aberto e cabe�alho v�lido.
 * //POS-CONDICAO: Cabe�alho escrito no arquivo.
 */
void escreve_cabecalho_indice(FILE *arq, CabecalhoIndice *cab);

/**
 * //DESCRICAO: L� o cabe�alho do arquivo de dados.
 * //ENTRADA: FILE *arq.
 * //SAIDA: Ponteiro para o cabe�alho lido.
 * //PRE-CONDICAO: Arquivo aberto.
 * //POS-CONDICAO: Cabe�alho retornado.
 */
CabecalhoDado* le_cabecalho_dado(FILE *arq);

/**
 * //DESCRICAO: Escreve o cabe�alho do arquivo de dados.
 * //ENTRADA: FILE *arq, CabecalhoDado *cab.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo aberto e cabe�alho v�lido.
 * //POS-CONDICAO: Cabe�alho escrito no arquivo.
 */
void escreve_cabecalho_dado(FILE *arq, CabecalhoDado *cab);

/**
 * //DESCRICAO: Escreve um registro do tipo Livro no arquivo de dados.
 * //ENTRADA: FILE *arq_dado, int pos, Livro livro.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo de dados aberto.
 * //POS-CONDICAO: Registro escrito.
 */
void escreve_no_livro(FILE *arq_dado, int pos, Livro livro);

/**
 * //DESCRICAO: L� um registro do tipo Livro do arquivo de dados.
 * //ENTRADA: FILE *arq_dado, int pos.
 * //SAIDA: Registro do livro lido.
 * //PRE-CONDICAO: Arquivo de dados aberto.
 * //POS-CONDICAO: Registro retornado.
 */
Livro ler_livro(FILE *arq_dado, int pos);

/**
 * //DESCRICAO: Insere um novo livro no sistema, atualizando os arquivos de �ndices e dados.
 * //ENTRADA: FILE *arq_indice, FILE *arq_dado, Livro info.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivos abertos e estruturados.
 * //POS-CONDICAO: Livro inserido.
 */
void inserir_no_arquivo_binario(FILE* arq_indice, FILE* arq_dado, Livro info);

/**
 * //DESCRICAO: Executa opera��es em lote a partir de um arquivo .txt.
 * //ENTRADA: FILE *arq_indice, FILE *arq_dado.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivos abertos e arquivo de lote existente.
 * //POS-CONDICAO: Opera��es processadas.
 */
void operacoes_em_lote(FILE *arq_indice, FILE *arq_dado);

/**
 * //DESCRICAO: Imprime a lista de n�s livres no arquivo de �ndices.
 * //ENTRADA: FILE *arq_indice.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo de �ndices aberto.
 * //POS-CONDICAO: Lista de n�s livres impressa.
 */
void imprimir_nos_livres_indice(FILE *arq_indice);

/**
 * //DESCRICAO: Imprime a lista de registros livres no arquivo de dados.
 * //ENTRADA: FILE *arq_dado.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo de dados aberto.
 * //POS-CONDICAO: Lista de registros livres impressa.
 */
void imprimir_registros_livres_dado(FILE *arq_dado);

/**
 * //DESCRICAO: Calcula e exibe o total de livros cadastrados.
 * //ENTRADA: FILE *arq_dado.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo de dados aberto.
 * //POS-CONDICAO: Total de livros exibido.
 */
void calcular_total_livros(FILE *arq_dado);

/**
 * //DESCRICAO: Exclui um livro do sistema e marca o registro como livre.
 * //ENTRADA: FILE *arq_indice, FILE *arq_dado, int codigo.
 * //SAIDA: void.
 * //PRE-CONDICAO: Livro existente.
 * //POS-CONDICAO: Livro removido e registro marcado como livre.
 */
void excluir_livro(FILE *arq_indice, FILE *arq_dado, int codigo);

/**
 * //DESCRICAO: Marca um registro de dados como livre (remo��o l�gica).
 * //ENTRADA: FILE *arq_dado, int pos_registro.
 * //SAIDA: void.
 * //PRE-CONDICAO: Registro existente.
 * //POS-CONDICAO: Registro marcado como livre.
 */
void marcar_registro_livre(FILE *arq_dado, int pos_registro);

#endif // ARQUIVO_H
