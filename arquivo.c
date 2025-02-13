#include "arquivo.h"
#include "utils.h"
#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * //DESCRICAO: Lê o cabeçalho do arquivo de índices.
 * //ENTRADA: FILE *arq.
 * //SAIDA: Ponteiro para o cabeçalho lido.
 * //PRE-CONDICAO: Arquivo aberto.
 * //POS-CONDICAO: Cabeçalho retornado.
 */
CabecalhoIndice* le_cabecalho_indice(FILE *arq) {
    CabecalhoIndice *cab = (CabecalhoIndice*) malloc(sizeof(CabecalhoIndice));
    fseek(arq, 0, SEEK_SET);
    fread(cab, sizeof(CabecalhoIndice), 1, arq);
    return cab;
}

/**
 * //DESCRICAO: Escreve o cabeçalho do arquivo de índices.
 * //ENTRADA: FILE *arq, CabecalhoIndice *cab.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo aberto e cabeçalho válido.
 * //POS-CONDICAO: Cabeçalho escrito no arquivo.
 */
void escreve_cabecalho_indice(FILE *arq, CabecalhoIndice *cab) {
    fseek(arq, 0, SEEK_SET);
    fwrite(cab, sizeof(CabecalhoIndice), 1, arq);
    fflush(arq);
}

/**
 * //DESCRICAO: Lê o cabeçalho do arquivo de dados.
 * //ENTRADA: FILE *arq.
 * //SAIDA: Ponteiro para o cabeçalho lido.
 * //PRE-CONDICAO: Arquivo aberto.
 * //POS-CONDICAO: Cabeçalho retornado.
 */
CabecalhoDado* le_cabecalho_dado(FILE *arq) {
    CabecalhoDado *cab = (CabecalhoDado*) malloc(sizeof(CabecalhoDado));
    fseek(arq, 0, SEEK_SET);
    fread(cab, sizeof(CabecalhoDado), 1, arq);
    return cab;
}

/**
 * //DESCRICAO: Escreve o cabeçalho do arquivo de dados.
 * //ENTRADA: FILE *arq, CabecalhoDado *cab.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo aberto e cabeçalho válido.
 * //POS-CONDICAO: Cabeçalho escrito no arquivo.
 */
void escreve_cabecalho_dado(FILE *arq, CabecalhoDado *cab) {
    fseek(arq, 0, SEEK_SET);
    fwrite(cab, sizeof(CabecalhoDado), 1, arq);
    fflush(arq);
}

/**
 * //DESCRICAO: Escreve um registro do tipo Livro no arquivo de dados.
 * //ENTRADA: FILE *arq_dado, int pos, Livro livro.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo de dados aberto.
 * //POS-CONDICAO: Registro escrito.
 */
void escreve_no_livro(FILE *arq_dado, int pos, Livro livro) {
    long offset = sizeof(CabecalhoDado) + pos * sizeof(Livro);
    fseek(arq_dado, offset, SEEK_SET);
    fwrite(&livro, sizeof(Livro), 1, arq_dado);
    fflush(arq_dado);
}

/**
 * //DESCRICAO: Lê um registro do tipo Livro do arquivo de dados.
 * //ENTRADA: FILE *arq_dado, int pos.
 * //SAIDA: Registro do livro lido.
 * //PRE-CONDICAO: Arquivo de dados aberto.
 * //POS-CONDICAO: Registro retornado.
 */
Livro ler_livro(FILE *arq_dado, int pos) {
    Livro livro;
    long offset = sizeof(CabecalhoDado) + pos * sizeof(Livro);
    fseek(arq_dado, offset, SEEK_SET);
    fread(&livro, sizeof(Livro), 1, arq_dado);
    return livro;
}

/**
 * //DESCRICAO: Insere um novo livro no sistema, atualizando os arquivos de índices e dados.
 * //ENTRADA: FILE *arq_indice, FILE *arq_dado, Livro info.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivos abertos e estruturados.
 * //POS-CONDICAO: Livro inserido.
 */
void inserir_no_arquivo_binario(FILE* arq_indice, FILE* arq_dado, Livro info) {
    CabecalhoIndice *cab_arvore = le_cabecalho_indice(arq_indice);
    CabecalhoDado *cab_dado = le_cabecalho_dado(arq_dado);
    Arvore23 *novo_no = NULL;

    if(cab_arvore->raiz == -1) {
        cab_arvore->raiz = cab_arvore->topo;
        novo_no = criar_no_arvore23(info.codigo, -1, -1, -1, -1, 1);
        novo_no->ptDados[0] = cab_dado->topo;
        escreve_no_arvore23_indice(arq_indice, novo_no, cab_arvore->raiz);
        cab_arvore->topo++;
        free(novo_no);
    } else {
        int chave_promovida;
        int ponteiro_de_dados = -1;
        int aux = inserir_aux(arq_indice, cab_arvore, cab_arvore->raiz, cab_dado->topo, info.codigo, &chave_promovida, &ponteiro_de_dados);
        if(aux != -1){
            novo_no = criar_no_arvore23(chave_promovida, cab_arvore->raiz, aux, -1, -1, 1);
            if(ponteiro_de_dados != -1)
                novo_no->ptDados[0] = ponteiro_de_dados;
            cab_arvore->raiz = cab_arvore->topo;
            escreve_no_arvore23_indice(arq_indice, novo_no, cab_arvore->raiz);
            cab_arvore->topo++;
            free(novo_no);
        }
    }

    escreve_no_livro(arq_dado, cab_dado->topo, info);
    cab_dado->topo++;

    escreve_cabecalho_indice(arq_indice, cab_arvore);
    escreve_cabecalho_dado(arq_dado, cab_dado);

    free(cab_arvore);
    free(cab_dado);
}

/**
 * //DESCRICAO: Executa operações em lote a partir de um arquivo .txt.
 * //ENTRADA: FILE *arq_indice, FILE *arq_dado.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivos abertos e arquivo de lote existente.
 * //POS-CONDICAO: Operações processadas.
 */
void operacoes_em_lote(FILE *arq_indice, FILE *arq_dado) {
    char nomeArquivo[100];
    printf("Digite o nome do arquivo de operações (.txt): ");
    fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
    nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';

    FILE *arq = fopen(nomeArquivo, "r");
    if(arq == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    char linha[512];
    while(fgets(linha, sizeof(linha), arq)) {
        char *token;
        Livro livro;
        token = strtok(linha, ";");
        if(token == NULL) continue;
        livro.codigo = atoi(token);

        token = strtok(NULL, ";");
        if(token == NULL) continue;
        strcpy(livro.titulo, token);

        token = strtok(NULL, ";");
        if(token == NULL) continue;
        strcpy(livro.autor, token);

        token = strtok(NULL, ";");
        if(token == NULL) continue;
        strcpy(livro.editora, token);

        token = strtok(NULL, ";");
        if(token == NULL) continue;
        livro.edicao = atoi(token);

        token = strtok(NULL, ";");
        if(token == NULL) continue;
        livro.ano = atoi(token);

        token = strtok(NULL, ";");
        if(token == NULL) continue;
        for(int i = 0; token[i]; i++){
            if(token[i] == ',')
                token[i] = '.';
        }
        livro.preco = atof(token);

        token = strtok(NULL, ";");
        if(token == NULL) continue;
        livro.estoque = atoi(token);

        inserir_no_arquivo_binario(arq_indice, arq_dado, livro);
        printf("Livro com código %d inserido em lote.\n", livro.codigo);
    }
    fclose(arq);
}

/**
 * //DESCRICAO: Imprime a lista de nós livres no arquivo de índices.
 * //ENTRADA: FILE *arq_indice.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo de índices aberto.
 * //POS-CONDICAO: Lista de nós livres impressa.
 */
void imprimir_nos_livres_indice(FILE *arq_indice) {
    CabecalhoIndice *cab = le_cabecalho_indice(arq_indice);
    int freeIndex = cab->livre;
    if(freeIndex == -1)
        printf("Nenhum nó livre.\n");
    else {
        printf("Nós livres no arquivo de índices:\n");
        while(freeIndex != -1) {
            printf("Nó livre: %d\n", freeIndex);
            Arvore23 *no = le_no_arvore23(arq_indice, freeIndex);
            freeIndex = no->filhos[0];
            free(no);
        }
    }
    free(cab);
}

/**
 * //DESCRICAO: Imprime a lista de registros livres no arquivo de dados.
 * //ENTRADA: FILE *arq_dado.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo de dados aberto.
 * //POS-CONDICAO: Lista de registros livres impressa.
 */
void imprimir_registros_livres_dado(FILE *arq_dado) {
    CabecalhoDado *cab = le_cabecalho_dado(arq_dado);
    int freeIndex = cab->livre;
    if(freeIndex == -1)
        printf("Nenhum registro livre.\n");
    else {
        printf("Registros livres no arquivo de dados:\n");
        printf("Registro livre: %d\n", freeIndex);
    }
    free(cab);
}

/**
 * //DESCRICAO: Calcula e exibe o total de livros cadastrados.
 * //ENTRADA: FILE *arq_dado.
 * //SAIDA: void.
 * //PRE-CONDICAO: Arquivo de dados aberto.
 * //POS-CONDICAO: Total de livros exibido.
 */
void calcular_total_livros(FILE *arq_dado) {
    CabecalhoDado *cab = le_cabecalho_dado(arq_dado);
    int total = cab->topo;
    int livres = 0;
    int freeIndex = cab->livre;
    while(freeIndex != -1) {
        livres++;
        break;
    }
    printf("Total de livros cadastrados: %d\n", total - livres);
    free(cab);
}

/**
 * //DESCRICAO: Marca um registro de dados como livre (remoção lógica).
 * //ENTRADA: FILE *arq_dado, int pos_registro.
 * //SAIDA: void.
 * //PRE-CONDICAO: Registro existente.
 * //POS-CONDICAO: Registro marcado como livre.
 */
void marcar_registro_livre(FILE *arq_dado, int pos_registro) {
  if (pos_registro < 0)
    return;

  CabecalhoDado *cab = le_cabecalho_dado(arq_dado);

  long offset = sizeof(CabecalhoDado) + pos_registro * sizeof(Livro);
  fseek(arq_dado, offset, SEEK_SET);

  int prox_livre = cab->livre;
  fwrite(&prox_livre, sizeof(int), 1, arq_dado);

  cab->livre = pos_registro;
  escreve_cabecalho_dado(arq_dado, cab);

  free(cab);
}

/**
 * //DESCRICAO: Exclui um livro do sistema e marca o registro como livre.
 * //ENTRADA: FILE *arq_indice, FILE *arq_dado, int codigo.
 * //SAIDA: void.
 * //PRE-CONDICAO: Livro existente.
 * //POS-CONDICAO: Livro removido e registro marcado como livre.
 */
void excluir_livro(FILE *arq_indice, FILE *arq_dado, int codigo) {
  CabecalhoIndice *cab_indice = le_cabecalho_indice(arq_indice);
  CabecalhoDado *cab_dado = le_cabecalho_dado(arq_dado);

  if (cab_indice->raiz == -1) {
    printf("Árvore vazia. Nenhum livro para excluir.\n");
    free(cab_indice);
    free(cab_dado);
    return;
  }

  int chave_substituir = -1;
  int ponteiro_substituir = -1;
  int flag_remove = 0;

  remover_chave_arvore23(arq_indice, cab_indice, cab_indice->raiz, codigo,
                         &chave_substituir, &ponteiro_substituir, &flag_remove);

  if (flag_remove == 0) {
    printf("Livro com código %d não encontrado.\n", codigo);
  } else {
    if (ponteiro_substituir != -1) {
      marcar_registro_livre(arq_dado, ponteiro_substituir);
      printf("Livro de código %d removido.\n", codigo);
    }

    Arvore23 *no_raiz = le_no_arvore23(arq_indice, cab_indice->raiz);
    if (no_raiz->nroChaves == 0) {
      int novo_raiz = no_raiz->filhos[0];
      int raiz_antiga_idx = cab_indice->raiz;
      free(no_raiz);

      if (novo_raiz == -1) {
        cab_indice->raiz = -1;
      } else {
        cab_indice->raiz = novo_raiz;
        marcar_no_livre(arq_indice, cab_indice, raiz_antiga_idx);
      }
      escreve_cabecalho_indice(arq_indice, cab_indice);
    } else {
      free(no_raiz);
    }
  }

  free(cab_indice);
  free(cab_dado);
}
