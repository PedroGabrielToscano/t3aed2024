#include <stdio.h>
#include <stdlib.h>
#include "cadastro.h"
#include "arvore.h"
#include "arquivo.h"
#include "utils.h"
#include "utils.c"
#include "arvore.c"
#include "arquivo.c"

/**
 * //DESCRICAO: Função principal do sistema de cadastro de livros.
 * //ENTRADA: Nenhum.
 * //SAIDA: Retorna 0 ao finalizar.
 * //PRE-CONDICAO: Arquivos e módulos corretamente configurados.
 * //POS-CONDICAO: Sistema encerrado.
 */
int main(void) {
    FILE *arq_indice = fopen(NOME_ARQUIVO_INDICE, "r+b");
    FILE *arq_dado   = fopen(NOME_ARQUIVO_DADO, "r+b");

    /* Se os arquivos não existem, cria-os e inicializa os cabeçalhos */
    if(arq_indice == NULL) {
        arq_indice = fopen(NOME_ARQUIVO_INDICE, "w+b");
        if(arq_indice == NULL) {
            printf("Erro ao criar arquivo de índices.\n");
            exit(1);
        }
        CabecalhoIndice cab_ind = { .raiz = -1, .topo = 0, .livre = -1 };
        fwrite(&cab_ind, sizeof(CabecalhoIndice), 1, arq_indice);
    }
    if(arq_dado == NULL) {
        arq_dado = fopen(NOME_ARQUIVO_DADO, "w+b");
        if(arq_dado == NULL) {
            printf("Erro ao criar arquivo de dados.\n");
            exit(1);
        }
        CabecalhoDado cab_dado = { .topo = 0, .livre = -1 };
        fwrite(&cab_dado, sizeof(CabecalhoDado), 1, arq_dado);
    }

    int opcao;
    do {
        printf("\n=== Sistema de Cadastro de Livros ===\n");
        printf("1. Cadastrar um livro\n");
        printf("2. Consultar livro por código\n");
        printf("3. Listar todos os livros (in-ordem)\n");
        printf("4. Imprimir árvore por níveis\n");
        printf("5. Imprimir lista de nós livres (arquivo de índices)\n");
        printf("6. Imprimir lista de registros livres (arquivo de dados)\n");
        printf("7. Calcular total de livros cadastrados\n");
        printf("8. Operações em lote\n");
        printf("9. Excluir um livro\n");
        printf("10. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch(opcao) {
            case 1: {
                Livro novoLivro;
                printf("Digite o código do livro: ");
                scanf("%d", &novoLivro.codigo);
                limpar_buffer();
                printf("Digite o título: ");
                fgets(novoLivro.titulo, sizeof(novoLivro.titulo), stdin);
                novoLivro.titulo[strcspn(novoLivro.titulo, "\n")] = '\0';
                printf("Digite o autor: ");
                fgets(novoLivro.autor, sizeof(novoLivro.autor), stdin);
                novoLivro.autor[strcspn(novoLivro.autor, "\n")] = '\0';
                printf("Digite a editora: ");
                fgets(novoLivro.editora, sizeof(novoLivro.editora), stdin);
                novoLivro.editora[strcspn(novoLivro.editora, "\n")] = '\0';
                printf("Digite a edição: ");
                scanf("%d", &novoLivro.edicao);
                printf("Digite o ano: ");
                scanf("%d", &novoLivro.ano);
                printf("Digite o preço: ");
                scanf("%lf", &novoLivro.preco);
                printf("Digite o estoque: ");
                scanf("%d", &novoLivro.estoque);
                limpar_buffer();

                inserir_no_arquivo_binario(arq_indice, arq_dado, novoLivro);
                printf("Livro cadastrado com sucesso!\n");
                break;
            }
            case 2: {
                int cod;
                printf("Digite o código do livro para consulta: ");
                scanf("%d", &cod);
                limpar_buffer();

                CabecalhoIndice *cab_ind = le_cabecalho_indice(arq_indice);
                int ptDados = -1;
                int pos = buscar_no_arvore23(arq_indice, cab_ind->raiz, cod, &ptDados);
                free(cab_ind);
                if(pos == -1) {
                    printf("Livro com código %d não encontrado.\n", cod);
                } else {
                    Livro livro = ler_livro(arq_dado, ptDados);
                    printf("\n--- Dados do Livro ---\n");
                    printf("Código: %d\nTítulo: %s\nAutor: %s\nEditora: %s\nEdição: %d\nAno: %d\nPreço: %.2lf\nEstoque: %d\n",
                           livro.codigo, livro.titulo, livro.autor, livro.editora, livro.edicao, livro.ano, livro.preco, livro.estoque);
                }
                break;
            }
            case 3: {
                CabecalhoIndice *cab_ind = le_cabecalho_indice(arq_indice);
                if(cab_ind->raiz == -1)
                    printf("Árvore vazia.\n");
                else
                    inOrder(arq_indice, arq_dado, cab_ind->raiz);
                free(cab_ind);
                break;
            }
            case 4: {
                CabecalhoIndice *cab_ind = le_cabecalho_indice(arq_indice);
                if(cab_ind->raiz == -1)
                    printf("Árvore vazia.\n");
                else
                    imprimir_arvore_niveis(arq_indice, cab_ind->raiz);
                free(cab_ind);
                break;
            }
            case 5: {
                imprimir_nos_livres_indice(arq_indice);
                break;
            }
            case 6: {
                imprimir_registros_livres_dado(arq_dado);
                break;
            }
            case 7: {
                calcular_total_livros(arq_dado);
                break;
            }
            case 8: {
                operacoes_em_lote(arq_indice, arq_dado);
                break;
            }
            case 9: {
                int cod;
                printf("Digite o código do livro a ser excluído: ");
                scanf("%d", &cod);
                limpar_buffer();
                excluir_livro(arq_indice, arq_dado, cod);
                break;
            }
            case 10:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 10);

    fclose(arq_indice);
    fclose(arq_dado);
    return 0;
}
