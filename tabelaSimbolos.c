/* 
 * File:   tabelaSimbolos.c
 * Author: José Félix Afonso Póvoa
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tabelaSimbolos.h"

/*adiciona símbolo ao topo da lista */
ListaSimbolos* AdicionarSimbolo(ListaSimbolos* lista, int tipo, char* nome) {
    /*alocar espaço para a estrutura */
    ListaSimbolos* elemento = (ListaSimbolos*) malloc(sizeof (ListaSimbolos));
    if (elemento != NULL) {
        elemento->tipo = tipo;
        strcpy(elemento->nome, nome);
        elemento->seguinte = lista;
    }
	/*printf("DEBUG: Adicionou %s\n", nome);*/
    /* em caso de falha de alocação de memória para elemento retorna NULL */
    return elemento;
}

/*procura um símbolo na lista: 1 - sucesso, 0 - falha */
int ProcurarSimbolo(ListaSimbolos* lista, char* nome) {
    ListaSimbolos* elemento = lista;
    while (elemento != NULL) {
        if (strcmp(elemento->nome, nome) == 0)
            return 1;
        elemento = elemento->seguinte;
    }
    return 0;
}

/*remove símbolo do topo e retorna a restante lista */
ListaSimbolos* RemoverSimboloTopo(ListaSimbolos* lista) {
    ListaSimbolos* elemento;
    if (lista != NULL) {
        elemento = lista->seguinte;
        free(lista);
        return elemento;
    }
    return NULL;
}

/*imprime a lista de símbolos*/
void ImprimirListaSimbolos(ListaSimbolos* lista) {
    ListaSimbolos* elemento = lista;
    while (elemento != NULL) {
        printf("Tipo: %d\tNome: %s\n", elemento->tipo, elemento->nome);
        elemento = elemento->seguinte;
    }
}



