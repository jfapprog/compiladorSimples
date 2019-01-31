/* 
 * File:   no.h
 * Author: José Félix Afonso Póvoa
 */

#ifndef NO_H
#define NO_H

#define MAXIMOFILHOS 4
#define MAXIMOSTR 32

typedef enum {
    NOINTEIRO, NOREAL, NOVARIAVEL, NOMONO, NOBI, NOTRI, NOQUA, NOLISTA
} TipoNo;

typedef struct SNo {
    TipoNo tipo; /*tipo de nó*/
    int atributo; /*tipo de token  - vem de tabela simbolos*/
    int linha; /*número da linha da instrução com o token*/
    struct SNo* seguinte; /*nó seguinte*/

    union {
        int inteiro; /*valor inteiro (caso seja inteiro)*/
        double real; /*valor real (caso seja real)*/
        char variavelID[MAXIMOSTR+1]; /*id de variável (caso seja variável)*/
        struct SNo* filhos[MAXIMOFILHOS]; /*ponteiro para os nós filhos*/
    } valor;
} No;

No* NoInteiro(int atributo, int inteiro, int linha);
No* NoReal(int atributo, double real, int linha);
No* NoVariavel(int atributo, char* variavelID, int linha);
No* NoMono(int atributo, No* subNo, int linha);
No* NoBi(int atributo, No* subNo0, No* subNo1, int linha);
No* NoTri(int atributo, No* subNo0, No* subNo1, No* subNo2, int linha);
No* NoQua(int atributo, No* subNo0, No* subNo1, No* subNo2, No* subNo3, int linha);
No* AdicionarNo(No* lista, No* no);
void LibertarNo(No* no);
int NovaTemporaria();
int NovaLabel();
void TraduzirOperacao(char* sinal, No* op1, No* op2);
void TraduzirFuncao(char* funcao, No* arg);
void TraduzirParaTAC(No* no);

#endif /* NO_H */

