/* 
 * File:   no.c
 * Author: José Félix Afonso Póvoa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "no.h"
#include "efBBison.h"

int temporariaAtual = -1;
int labelAtual = -1;

/*cria um nó para um número inteiro*/
No* NoInteiro(int atributo, int inteiro, int linha) {
    /*alocar memória para o nó*/
    No* novo = (No*) malloc(sizeof (No));
    if (novo != NULL) {
        novo->tipo = NOINTEIRO;
        novo->atributo = atributo;
        novo->linha = linha;
        novo->seguinte = NULL;
        novo->valor.inteiro = inteiro;
    }
	/*printf("DEBUG: Criou nó inteiro, linha %d\n", linha);*/
    /*caso a alocação falhe retorna NULL*/
    return novo;
}

/*cria um nó para um número real*/
No* NoReal(int atributo, double real, int linha) {
    /*alocar memória para o nó*/
    No* novo = (No*) malloc(sizeof (No));
    if (novo != NULL) {
        novo->tipo = NOREAL;
        novo->atributo = atributo;
        novo->linha = linha;
        novo->seguinte = NULL;
        novo->valor.real = real;
    }
	/*printf("DEBUG: Criou nó real, linha %d\n", linha);*/
    /*caso a alocação falhe retorna NULL*/
    return novo;
}

/*cria um nó para uma variável*/
No* NoVariavel(int atributo, char* variavelID, int linha) {
    /*alocar memória para o nó*/
    No* novo = (No*) malloc(sizeof (No));
    if (novo != NULL) {
        novo->tipo = NOVARIAVEL;
        novo->atributo = atributo;
        novo->linha = linha;
        novo->seguinte = NULL;
        strcpy(novo->valor.variavelID,variavelID);
    }
	/*printf("DEBUG: Criou nó variável, linha %d\n", linha);*/
    /*caso a alocação falhe retorna NULL*/
    return novo;
}

/*cria um nó para uma instrução com apenas um subnós*/
No* NoMono(int atributo, No* subNo, int linha) {
    /*alocar memória para o nó */
    No* novo = (No*) malloc(sizeof (No));
    if (novo != NULL) {
        novo->tipo = NOMONO;
        novo->atributo = atributo;
        novo->linha = linha;
        novo->seguinte = NULL;
        novo->valor.filhos[0] = subNo;
        novo->valor.filhos[1] = NULL;
        novo->valor.filhos[2] = NULL;
        novo->valor.filhos[3] = NULL;
    }
	/*printf("DEBUG: Criou nó mono, linha %d\n", linha);*/
    /*caso a alocação falhe retorna NULL*/
    return novo;
}

/*cria um nó para uma instrução com dois subnós*/
No* NoBi(int atributo, No* subNo0, No* subNo1, int linha) {
    /*alocar memória para o nó*/
    No* novo = (No*) malloc(sizeof (No));
    if (novo != NULL) {
        novo->tipo = NOBI;
        novo->atributo = atributo;
        novo->linha = linha;
        novo->seguinte = NULL;
        novo->valor.filhos[0] = subNo0;
        novo->valor.filhos[1] = subNo1;
        novo->valor.filhos[2] = NULL;
        novo->valor.filhos[3] = NULL;
    }
	/*printf("DEBUG: Criou nó bi, linha %d\n", linha);*/
    /*caso a alocação falhe retorna NULL*/
    return novo;
}

/*cria um nó para uma instrução com três subnós*/
No* NoTri(int atributo, No* subNo0, No* subNo1, No* subNo2, int linha) {
    /*alocar memória para o nó*/
    No* novo = (No*) malloc(sizeof (No));
    if (novo != NULL) {
        novo->tipo = NOTRI;
        novo->atributo = atributo;
        novo->linha = linha;
        novo->seguinte = NULL;
        novo->valor.filhos[0] = subNo0;
        novo->valor.filhos[1] = subNo1;
        novo->valor.filhos[2] = subNo2;
        novo->valor.filhos[3] = NULL;
    }
	/*printf("DEBUG: Criou nó tri, linha %d\n", linha);*/
    /*caso a alocação falhe retorna NULL*/
    return novo;
}

/*cria um nó para uma instrução com quatro subnós*/
No* NoQua(int atributo, No* subNo0, No* subNo1, No* subNo2, No* subNo3, int linha) {
    /*alocar memória para o nó*/
    No* novo = (No*) malloc(sizeof (No));
    if (novo != NULL) {
        novo->tipo = NOQUA;
        novo->atributo = atributo;
        novo->linha = linha;
        novo->seguinte = NULL;
        novo->valor.filhos[0] = subNo0;
        novo->valor.filhos[1] = subNo1;
        novo->valor.filhos[2] = subNo2;
        novo->valor.filhos[3] = subNo3;
    }
	/*printf("DEBUG: Criou nó qua, linha %d\n", linha);*/
    /*caso a alocação falhe retorna NULL*/
    return novo;
}

/*junta uma instrução ao final de uma lista de instruções*/
No* AdicionarNo(No* lista, No* no) {
    if (lista == NULL) {
        lista = no;
		/*printf("DEBUG: Iniciou lista\n");*/
        return lista;
    }
    No* elemento = lista;
    while (elemento->seguinte != NULL)
        elemento = elemento->seguinte;
    elemento->seguinte = no;
	/*printf("DEBUG: Adicionou nó a lista\n");*/
    return lista;
}

/*liberta a memória alocada para os nós*/
void LibertarNo(No* no) {
    if (no == NULL)
        return;
    if (no->tipo == NOMONO || no->tipo == NOBI || no->tipo == NOTRI || no->tipo == NOQUA) {
        int i;
        for (i = 0; i < MAXIMOFILHOS; i++)
            LibertarNo(no->valor.filhos[i]);
    }
    if (no->seguinte != NULL)
        LibertarNo(no->seguinte);
    free(no);
    /*printf("DEBUG: Nó eliminado!\n");*/
}

/*retorna o número de uma nova variável temporária*/
int NovaTemporaria() {
    temporariaAtual++;
    return temporariaAtual;
}

/*retorna o número de uma nova label*/
int NovaLabel() {
    labelAtual++;
    return labelAtual;
}

/*traduz operações aritméticas ou lógicas*/
void TraduzirOperacao(char* sinal, No* op1, No* op2) {
    int trad1 = 0;
    int trad2 = 0;
	int numTrad = 0;
    /*se o operador 1 não for inteiro, real ou variável traduzir*/
    if (op1->tipo != NOINTEIRO && op1->tipo != NOREAL && op1->tipo != NOVARIAVEL) {
        TraduzirParaTAC(op1);
        trad1 = 1;
    }
    /*se o operador 2 não for inteiro ou variável traduzir*/
    if (op2->tipo != NOINTEIRO && op2->tipo != NOREAL && op2->tipo != NOVARIAVEL) {
        TraduzirParaTAC(op2);
        trad2 = 1;
    }
	/*contabilizar quantos operadores foram traduzidos*/
	numTrad = trad1 + trad2;
	/*traduzir*/
    printf("_t%d = ", NovaTemporaria());
    if (trad1 == 1) {
        printf("_t%d %s ", temporariaAtual - numTrad, sinal);
		numTrad--;
	}
    else {
        if (op1->tipo == NOINTEIRO)
            printf("%d %s ", op1->valor.inteiro, sinal);
		if (op1->tipo == NOREAL)
            printf("%.2f %s ", op1->valor.real, sinal);
        if (op1->tipo == NOVARIAVEL)
            printf("%s %s ", op1->valor.variavelID, sinal);
    }
    if (trad2 == 1)
        printf("_t%d\n", temporariaAtual - numTrad);
    else {
        if (op2->tipo == NOINTEIRO)
            printf("%d\n", op2->valor.inteiro);
		if (op2->tipo == NOREAL)
            printf("%.2f\n", op2->valor.real);
        if (op2->tipo == NOVARIAVEL)
            printf("%s\n", op2->valor.variavelID);
    }
}

/*traduz funções*/
void TraduzirFuncao(char* funcao, No* arg) {
	int trad = 0;
	/*se o argumento não for inteiro ou variável traduzir*/
	if (arg->tipo != NOINTEIRO && arg->tipo != NOVARIAVEL) {
    	TraduzirParaTAC(arg);
		trad = 1;
	}
	/*traduzir*/
	printf("_t%d = %s(", NovaTemporaria(), funcao);
	if (trad == 1)
    	printf("_t%d)\n", temporariaAtual - 1);
	else {
		if (arg->tipo == NOINTEIRO)
			printf("%d)\n", arg->valor.inteiro);
		if (arg->tipo == NOVARIAVEL)
			printf("%s)\n", arg->valor.variavelID);
	}	
}

/*traduz a lista de instruções para TAC (three-address code)*/
void TraduzirParaTAC(No* no) {
    if (no == NULL)
        return;
    int i;
    switch (no->atributo) {

        case SE:
            TraduzirParaTAC(no->valor.filhos[0]);
            printf("ifZ _t%d goto L%d\n", temporariaAtual, NovaLabel());
            TraduzirParaTAC(no->valor.filhos[1]);
            printf("goto L%d\n", NovaLabel());
            printf("L%d: ", labelAtual - 1);
            TraduzirParaTAC(no->valor.filhos[2]);
            printf("L%d:\n", labelAtual);
            break;

        case ATRIB:
            TraduzirParaTAC(no->valor.filhos[1]);
            printf("%s = _t%d\n", no->valor.filhos[0]->valor.variavelID, temporariaAtual);
            break;

        case ENQUANTO:
            printf("L%d: ", NovaLabel());
            TraduzirParaTAC(no->valor.filhos[0]);
            printf("ifZ _t%d goto L%d\n", temporariaAtual, NovaLabel());
            TraduzirParaTAC(no->valor.filhos[1]);
            printf("goto L%d\n", labelAtual - 1);
            printf("L%d:\n", labelAtual);
            break;

        case PARA:
            TraduzirParaTAC(no->valor.filhos[0]);
            printf("L%d: ", NovaLabel());
            TraduzirParaTAC(no->valor.filhos[1]);
            printf("ifZ _t%d goto L%d\n", temporariaAtual, NovaLabel());
            TraduzirParaTAC(no->valor.filhos[3]);
            TraduzirParaTAC(no->valor.filhos[2]);
            printf("goto L%d\n", labelAtual - 1);
            printf("L%d:\n", labelAtual);
            break;

        case IGUAL:
            TraduzirOperacao("=", no->valor.filhos[0], no->valor.filhos[1]);
            break;
        case DIFERENTE:
            TraduzirOperacao("<>", no->valor.filhos[0], no->valor.filhos[1]);
            break;
        case MAIOR:
            TraduzirOperacao(">", no->valor.filhos[0], no->valor.filhos[1]);
            break;
        case MENOR:
            TraduzirOperacao("<", no->valor.filhos[0], no->valor.filhos[1]);
            break;
        case MAIORIGUAL:
            TraduzirOperacao(">=", no->valor.filhos[0], no->valor.filhos[1]);
            break;
        case MENORIGUAL:
            TraduzirOperacao("<=", no->valor.filhos[0], no->valor.filhos[1]);
            break;

        case SOMA:
            TraduzirOperacao("+", no->valor.filhos[0], no->valor.filhos[1]);
            break;
        case SUB:
            if (no->tipo == NOMONO)
                printf("_t%d = -%s\n", NovaTemporaria(), no->valor.filhos[0]->valor.variavelID);
            else
                TraduzirOperacao("-", no->valor.filhos[0], no->valor.filhos[1]);
            break;
        case MULT:
            TraduzirOperacao("*", no->valor.filhos[0], no->valor.filhos[1]);
            break;
        case DIV:
            TraduzirOperacao("/", no->valor.filhos[0], no->valor.filhos[1]);
            break;

        case SEN:
            TraduzirFuncao("seno", no->valor.filhos[0]);
            break;
        case COS:
            TraduzirFuncao("coseno", no->valor.filhos[0]);
            break;
        case ARCSEN:
            TraduzirFuncao("arcosseno", no->valor.filhos[0]);
            break;
        case ARCCOS:
            TraduzirFuncao("arcocosseno", no->valor.filhos[0]);
            break;
        case LOG:
            TraduzirFuncao("log", no->valor.filhos[0]);
            break;
        case INT:
            TraduzirFuncao("int", no->valor.filhos[0]);
            break;

        case INTEIRO:
            printf("_t%d = %d\n", NovaTemporaria(), no->valor.inteiro);
            break;
        case REAL:
            printf("_t%d = %.2f\n", NovaTemporaria(), no->valor.real);
            break;
        case VARIAVEL:
            printf("_t%d = %s\n", NovaTemporaria(), no->valor.variavelID);
            break;
    }
    if (no->seguinte != NULL)
        TraduzirParaTAC(no->seguinte);
}
