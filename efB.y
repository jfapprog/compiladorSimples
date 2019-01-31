%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "tabelaSimbolos.h"
	#include "no.h"
	
	/*variáveis globais externas - ficheiro efBFlex.c*/	
	extern int yylineno;	
	extern FILE* yyin;

	/*variáveis globais*/
	int numeroErros = 0;
	ListaSimbolos* listaVariaveis;

	/*função externa - ficheiro efBFlex.c*/
	extern int yylex();
	
	/*protótipo da função chamada em caso de não reconhecimento da sintaxe*/
	int yyerror(char* s);
	
%}

%union {
	int numInt;
	double numDou;
	char nomeVar[32+1];
	struct SNo* no;
}

%type <no> listaInst 
%type <no> instrucao
%type <no> condicional
%type <no> afirmacao
%type <no> atribuicao
%type <no> ciclo
%type <no> condicao
%type <no> expressao
%token <numInt> INTEIRO
%token <numDou> REAL
%token <nomeVar> VARIAVEL
%right ATRIB
%left SOMA SUB 
%left MULT DIV
%left IGUAL DIFERENTE MAIOR MENOR MAIORIGUAL MENORIGUAL
%token SE SENAO ENQUANTO PARA
%token SEN COS ARCSEN ARCCOS LOG INT
%token PONTOVIR ABREPAR FECHAPAR ABRECHA FECHACHA

%%

programa	:	listaInst	{TraduzirParaTAC($1); LibertarNo($1);}
			;

listaInst	:	listaInst instrucao		{$$ = $1; AdicionarNo($$,$2);}
			|	instrucao				{$$ = $1;}			
			;

instrucao	:	condicional 	{$$ = $1;}
			|	afirmacao		{$$ = $1;}
			|	error PONTOVIR	{yyerrok; $$ = NULL; yyclearin;}
			|	error FECHACHA	{yyerrok; $$ = NULL; yyclearin;}	
			;

condicional	:	SE ABREPAR condicao FECHAPAR ABRECHA listaInst FECHACHA SENAO ABRECHA listaInst FECHACHA	{$$ = NoTri(SE,$3,$6,$10,yylineno);}
			;

afirmacao	:	atribuicao	{$$ = $1;}
			|	ciclo		{$$ = $1;}
			;

atribuicao	:	VARIAVEL ATRIB expressao PONTOVIR	{if(ProcurarSimbolo(listaVariaveis,$1)==0) listaVariaveis = AdicionarSimbolo(listaVariaveis, VARIAVEL, $1); $$ = NoBi(ATRIB, NoVariavel(VARIAVEL, $1, yylineno), $3, yylineno);}
			;

ciclo		:	ENQUANTO ABREPAR condicao FECHAPAR ABRECHA listaInst FECHACHA {$$ = NoBi(ENQUANTO, $3, $6, yylineno);}
			|	PARA ABREPAR VARIAVEL ATRIB expressao PONTOVIR condicao PONTOVIR VARIAVEL ATRIB expressao FECHAPAR ABRECHA listaInst FECHACHA		{if(ProcurarSimbolo(listaVariaveis,$3)==0) listaVariaveis = AdicionarSimbolo(listaVariaveis, VARIAVEL, $3); if(ProcurarSimbolo(listaVariaveis,$9)==0) listaVariaveis = AdicionarSimbolo(listaVariaveis, VARIAVEL, $9); $$ = NoQua(PARA, NoBi(ATRIB, NoVariavel(VARIAVEL, $3, yylineno),$5,yylineno), $7, NoBi(ATRIB, NoVariavel(VARIAVEL, $9, yylineno),$11,yylineno), $14, yylineno);}				
			;

condicao	:	expressao IGUAL expressao		{$$ = NoBi(IGUAL,$1,$3,yylineno);}
			|	expressao DIFERENTE expressao	{$$ = NoBi(DIFERENTE,$1,$3,yylineno);}
			|	expressao MAIOR expressao		{$$ = NoBi(MAIOR,$1,$3,yylineno);}
			|	expressao MENOR expressao		{$$ = NoBi(MENOR,$1,$3,yylineno);}
			|	expressao MAIORIGUAL expressao	{$$ = NoBi(MAIORIGUAL,$1,$3,yylineno);}
			|	expressao MENORIGUAL expressao	{$$ = NoBi(MENORIGUAL,$1,$3,yylineno);}
			;

expressao	:	expressao SOMA expressao				{$$ = NoBi(SOMA,$1,$3,yylineno);}
			|	expressao SUB expressao					{$$ = NoBi(SUB,$1,$3,yylineno);}
			|	expressao MULT expressao				{$$ = NoBi(MULT,$1,$3,yylineno);}
			|	expressao DIV expressao					{$$ = NoBi(DIV,$1,$3,yylineno);}
			|	ABREPAR expressao FECHAPAR				{$$ = $2;}
			|	SEN ABREPAR expressao FECHAPAR			{$$ = NoMono(SEN,$3,yylineno);}	
			|	COS ABREPAR expressao FECHAPAR			{$$ = NoMono(COS,$3,yylineno);}			
			|	ARCSEN ABREPAR expressao FECHAPAR		{$$ = NoMono(ARCSEN,$3,yylineno);}	
			|	ARCCOS ABREPAR expressao FECHAPAR 		{$$ = NoMono(ARCCOS,$3,yylineno);}	
			|	LOG ABREPAR expressao FECHAPAR			{$$ = NoMono(LOG,$3,yylineno);}	
			|	INT ABREPAR expressao FECHAPAR			{$$ = NoMono(INT,$3,yylineno);}				
			|	INTEIRO									{$$ = NoInteiro(INTEIRO, $1, yylineno);}
			|	SUB INTEIRO 							{int n = -$2; $$ = NoInteiro(INTEIRO, n, yylineno);}
			|	REAL									{$$ = NoReal(REAL, $1, yylineno);}
			|	SUB REAL 								{double r = -$2; $$ = NoReal(REAL, r, yylineno);}	
			|	VARIAVEL								{if(ProcurarSimbolo(listaVariaveis,$1)==0) listaVariaveis = AdicionarSimbolo(listaVariaveis, VARIAVEL, $1); $$ = NoVariavel(VARIAVEL, $1, yylineno);}
			|	SUB VARIAVEL 							{if(ProcurarSimbolo(listaVariaveis,$2)==0) listaVariaveis = AdicionarSimbolo(listaVariaveis, VARIAVEL, $2); $$ = NoMono(SUB,NoVariavel(VARIAVEL, $2, yylineno), yylineno);}	
			;	
%%

int main(int argc, char** argv) {
	
	if (argc > 2) {
		fprintf(stderr,"Sintaxe: efA [nome_ficheiro]\n");
		return 1;
	}

	if (argc == 2) {
		yyin = fopen(argv[1],"r");
		if (!yyin) {
			fprintf(stderr,"Erro ao abrir o ficheiro \"%s\"\n", argv[1]);
			return 1;
		}
	}

	yyparse();
	
	printf("\nAnálise completa.\n");
	if (numeroErros > 0)
		printf("%d erro(s) encontrados(s) no código!\n", numeroErros);
	else
		printf("Código sem erros.\n");
	
	/* remover lista de variáveis */
	while(listaVariaveis != NULL)
		listaVariaveis = RemoverSimboloTopo(listaVariaveis);

	return 0;
}

int yyerror(char* s) {
		fprintf(stderr,"Linha %d:\t%s\n", yylineno, s);
		numeroErros++;
		return 1;
}

