%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include "lex.yy.c"
//	#define YYDEBUG 1
Node* root;
int error=0;
%}

%locations
%union {
	struct Node* cnt;
}
%token <cnt> INT
%token <cnt> FLOAT
%token <cnt> ID
%token <cnt> SEMI COMMA ASSIGNOP RELOP
%token <cnt> PLUS MINUS STAR DIV
%token <cnt> AND OR DOT NOT
%token <cnt> TYPE
%token <cnt> LP
%token <cnt> RP
%token <cnt> LB
%token <cnt> RB
%token <cnt> LC
%token <cnt> RC
%token <cnt> STRUCT
%token <cnt> RETURN
%token <cnt> IF
%token <cnt> ELSE
%token <cnt> WHILE
%token MULTINOTE;
%token LINENOTE;
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT
%type <cnt> Program 
%type <cnt> ExtDefList
%type <cnt> ExtDef
%type <cnt> ExtDecList
%type <cnt> Specifier
%type <cnt> StructSpecifier
%type <cnt> OptTag
%type <cnt> Tag
%type <cnt> VarDec
%type <cnt> FunDec
%type <cnt> VarList
%type <cnt> ParamDec
%type <cnt> CompSt
%type <cnt> StmtList
%type <cnt> Stmt
%type <cnt> DefList
%type <cnt> Def
%type <cnt> DecList
%type <cnt> Dec
%type <cnt> Exp
%type <cnt> Args
%%
Program : ExtDefList {root=create_node("Program");insert_node(root,$1);}
ExtDefList : {$$=NULL;}
	|ExtDef ExtDefList {$$=create_node("ExtDefList");insert_node($$,$1);insert_node($$,$2);}

ExtDef : Specifier ExtDecList SEMI {$$=create_node("ExtDef");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Specifier SEMI {$$=create_node("ExtDef");insert_node($$,$1);insert_node($$,$2);}
	| Specifier FunDec CompSt {$$=create_node("ExtDef");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Specifier FunDec SEMI {$$=create_node("ExtDef");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Specifier error FunDec CompSt
ExtDecList : VarDec {$$=create_node("ExtDecList");insert_node($$,$1);}
	| VarDec COMMA ExtDecList {$$=create_node("ExtDecList");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
Specifier : TYPE {$$=create_node("Specifier");insert_node($$,$1);}
	| StructSpecifier {$$=create_node("Specifier");insert_node($$,$1);}
StructSpecifier : STRUCT OptTag LC DefList RC {$$=create_node("StructSpecifier");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);insert_node($$,$5);}
	| STRUCT Tag {$$=create_node("StructSpecifier");insert_node($$,$1);insert_node($$,$2);}
	| STRUCT OptTag LC DefList error '\n'

OptTag : {$$=NULL;}
	| ID {$$=create_node("OptTag");insert_node($$,$1);}
Tag : ID {$$=create_node("Tag");insert_node($$,$1);}
VarDec : ID {$$=create_node("VarDec");insert_node($$,$1);}
	| VarDec LB INT RB {$$=create_node("VarDec");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);}
	| VarDec LB error RB
	| VarDec LB INT error
	| VarDec LB error
FunDec : ID LP VarList RP {$$=create_node("FunDec");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);}
	| ID LP RP {$$=create_node("FunDec");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| ID LP error
	| ID error RP
VarList : ParamDec COMMA VarList {$$=create_node("VarList");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| ParamDec {$$=create_node("VarList");insert_node($$,$1);}
ParamDec : Specifier VarDec {$$=create_node("ParamDec");insert_node($$,$1);insert_node($$,$2);}
CompSt : LC DefList StmtList RC {$$=create_node("CompSt");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);}
	| error RC 
	| error '\n'
	| error ID LP RP
	| error ID LP VarList RP
	| CompSt MULTINOTE
	| CompSt LINENOTE
	| LINENOTE CompSt
	| MULTINOTE CompSt
StmtList : {$$=NULL;}
	| Stmt StmtList {$$=create_node("StmtList");insert_node($$,$1);insert_node($$,$2);}
Stmt : Exp SEMI {$$=create_node("Stmt");insert_node($$,$1);insert_node($$,$2);}
	| CompSt {$$=create_node("Stmt");insert_node($$,$1);}
	| RETURN Exp SEMI {$$=create_node("Stmt");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| RETURN error SEMI
	| IF LP Exp RP Stmt {$$=create_node("Stmt");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);insert_node($$,$5);}
	| IF LP Exp RP Stmt ELSE Stmt {$$=create_node("Stmt");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);insert_node($$,$5);insert_node($$,$6);insert_node($$,$7);}
	| WHILE LP Exp RP Stmt {$$=create_node("Stmt");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);insert_node($$,$5);}
	| error SEMI 
	| error '\n' 
	| Exp error '\n'
	| RETURN Exp error '\n'
DefList : {$$=NULL;}
	| Def DefList {$$=create_node("DefList");insert_node($$,$1);insert_node($$,$2);}
Def : Specifier DecList SEMI {$$=create_node("Def");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| error SEMI
	| error '\n'
DecList : Dec {$$=create_node("DecList");insert_node($$,$1);}
	| Dec COMMA DecList {$$=create_node("DecList");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
Dec : VarDec {$$=create_node("Dec");insert_node($$,$1);}
	| VarDec ASSIGNOP Exp {$$=create_node("Dec");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
Exp : Exp ASSIGNOP Exp {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp AND Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp OR Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp RELOP Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp PLUS Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp MINUS Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp STAR Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp DIV Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| LP Exp RP  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| MINUS Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);}
	| NOT Exp  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);}
	| ID LP Args RP  {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);}
	| ID LP RP {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp LB Exp RB {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);insert_node($$,$4);} 
	| Exp DOT ID {$$=create_node("Exp");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| ID {$$=create_node("Exp");insert_node($$,$1);}
	| INT {$$=create_node("Exp");insert_node($$,$1);}
	| FLOAT {$$=create_node("Exp");insert_node($$,$1);}
	| Exp LB error RB
	| Exp DOT error
	| Exp error

Args : Exp COMMA Args {$$=create_node("Args");insert_node($$,$1);insert_node($$,$2);insert_node($$,$3);}
	| Exp {$$=create_node("Args");insert_node($$,$1);}
%%
yyerror(char* msg){
	printf("Error type B at Line %d.\n",yylineno);
	error=1;
}
