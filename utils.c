#include "utils.h"
#include <stdio.h>

/**
 * //DESCRICAO: Limpa o buffer de entrada.
 * //ENTRADA: Nenhum.
 * //SAIDA: void.
 * //PRE-CONDICAO: Nenhum.
 * //POS-CONDICAO: Buffer limpo.
 */
void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
