%{
#include "bis.tab.h"
extern int numLigne;
extern YYSTYPE yylval;
#include "tbsbl.h"
char test[40];
void linecount(char* commentaire){
	int i = 0;
	while( commentaire[i] != 0){
		if(commentaire[i] == 10){
			numLigne++;
		}
		i++;
	}
}
%}

ltr [a-zA-Z]
nbr [0-9]
id {ltr}({ltr}|{nbr}|_({nbr}|{ltr}))*
integers {nbr}+
reels {nbr}+\.{nbr}+


comment \{([^}]|\n)*\}


%%
LANGAGE return mc_langage;
VAR return mc_var;
INT { yylval.str=strdup(yytext);
      return mc_int;}
FLOAT { yylval.str=strdup(yytext);
         return mc_float;}
BOOL { yylval.str=strdup(yytext);
		return mc_bool;}
BEGIN return mc_bgn;
END return mc_end;
CONST  return mc_const;
While return mc_whl;
"If" return mc_if;
Function return mc_fnc;
Return return mc_return;
{id} { if(yyleng>12) {printf("erreur lexicale a la ligne %d, sur l'entite %s, qui a une taille de %d >12\n",numLigne,yytext,yyleng);
        }else {  yylval.str=strdup(yytext); nom(yytext, test);
		   if(strcmp(yytext, test) != 0){inserer(yytext,"id");}
	       return id;}
	    }
{integers} {   yylval.entier=atoi(yytext);  // 5.7
          return integers;}
{reels} { yylval.flottant = atof(yytext);
			return reels;}


{comment}       {linecount(yytext);}
: return col;
; return scol;
, return com;
= return aff;
\- return minus;
\+ return plus;
\* return mult;
\/ return dvd;
"<" return inf;
"<=" return infeg;
">" return supp;
">=" return suppeg;
"<>" return diff;
"==" egg;
"(" return paro;
")" return parf;
[ \t]
\n numLigne++;
. {printf("erreur lexicale a la ligne %d sur l'entite %s\n",numLigne,yytext);}
	
