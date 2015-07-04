%{
#include "node.h"
int chars=0;
int yycolumn = 1;
extern int error;
#define YY_USER_ACTION yylloc.first_line=yylloc.last_line=yylineno;\
	yylloc.first_column=yycolumn;\
	yylloc.last_column=yycolumn+yyleng-1;\
	yycolumn+=yyleng;


%}
%option yylineno
digit [0-9]
octdigit [0-7]
oct "0"[1-7]{octdigit}*
hexdigit [0-9A-Fa-f]
hex 0[xX][1-9A-Fa-f]{hexdigit}*
letter [_a-zA-Z]
int [1-9]{digit}*|"0"
expfloat {digit}+\.{digit}+[eE][+-]?{digit}+
float {digit}+\.{digit}+ 
id {letter}({letter}|{digit})* 
semi ";"
comma ","
assignop "="
relop ">"|"<"|">="|"<="|"=="|"!="
plus "+"
minus "-"
star "*"
div "/"
and "&&"
or "||"
dot "."
not "!"
lp "("
rp ")"
lb "["
rb "]"
lc "{"
rc "}"
struct "struct"
return "return"
if "if"
else "else"
while "while"
type "int"|"float"
%%
{semi} {yylval.cnt=create_node_term("SEMI");return SEMI;}
{comma} {yylval.cnt=create_node_term("COMMA");return COMMA;}
{assignop} {yylval.cnt=create_node_term("ASSIGNOP");return ASSIGNOP;}
{relop} {yylval.cnt=create_node_term("RELOP");strcpy(yylval.cnt->relop,yytext);return RELOP;}
{plus} {yylval.cnt=create_node_term("PLUS");return PLUS;}
{minus} {yylval.cnt=create_node_term("MINUS");return MINUS;}
{star} {yylval.cnt=create_node_term("STAR");return STAR;}
{div} {
char ch=input();
if (ch=='/'){
	while (ch!='\n') ch=input();
}
else if (ch=='*'){
	char ch1=input();
	while (ch!='*'||ch1!='/'){
		ch=ch1;ch1=input();
	}
}
else {
unput(ch);
yylval.cnt=create_node_term("DIV");return DIV;
}
}
{and} {yylval.cnt=create_node_term("AND");return AND;}
{or} {yylval.cnt=create_node_term("OR");return OR;}
{dot} {yylval.cnt=create_node_term("DOT");return DOT;}
{not} {yylval.cnt=create_node_term("NOT");return NOT;}
{lp} {yylval.cnt=create_node_term("LP");return LP;}
{rp} {yylval.cnt=create_node_term("RP");return RP;}
{type} {yylval.cnt=create_node_type("TYPE",yytext);return TYPE;}
{lb} {yylval.cnt=create_node_term("LB");return LB;}
{rb} {yylval.cnt=create_node_term("RB");return RB;}
{lc} {yylval.cnt=create_node_term("LC");return LC;}
{rc} {yylval.cnt=create_node_term("RC");return RC;}
{struct} {yylval.cnt=create_node_term("STRUCT");return STRUCT;}
{return} {yylval.cnt=create_node_term("RETURN");return RETURN;}
{if} {yylval.cnt=create_node_term("IF");return IF;}
{else} {yylval.cnt=create_node_term("ELSE");return ELSE;}
{while} {yylval.cnt=create_node_term("WHILE");return WHILE;}
" " {chars++;}

{hex}|{int}|{oct} {
	yylval.cnt=create_node_int(strtol(yytext,NULL,0));
    return INT;
}
{float}|{expfloat} {
	yylval.cnt=create_node_float(atof(yytext));
	return FLOAT;
}
{id} {yylval.cnt=create_node_ID(yytext);return ID;}

\t {chars++;}
\n {yycolumn=1;}
. {error=1;printf("Error type A at Line %d: Mysterious character \"%c\".\n",yylineno,yytext[0]);}


%%

