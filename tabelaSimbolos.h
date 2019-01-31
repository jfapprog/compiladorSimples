/* 
 * File:   tabelaSimbolos.h
 * Author: José Félix Afonso Póvoa
 */

#ifndef TABELASIMBOLOS_H
#define TABELASIMBOLOS_H

#define MAXSTR 32

typedef struct SListaSimbolos {
    int tipo;
    char nome[MAXSTR + 1];
    struct SListaSimbolos* seguinte;
} ListaSimbolos;

ListaSimbolos* AdicionarSimbolo(ListaSimbolos* lista, int tipo, char* nome);
int ProcurarSimbolo(ListaSimbolos* lista, char* nome);
ListaSimbolos* RemoverSimboloTopo(ListaSimbolos* lista);
void ImprimirListaSimbolos(ListaSimbolos* lista);

#endif /* TABELASIMBOLOS_H */

