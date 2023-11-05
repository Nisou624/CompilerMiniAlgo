%{
int numLigne=1;
int codeOPR=0; // 1:+, 2=-, 3:*, 4:/
char sauvType[20];
char typecnst[50];
char nomIdf[20];
char sauvNom[50];
char uno[50];
char dos[50];
char tp1[50];
char tp2[50];

%}


%union{
    int entier;
    float flottant;
    char* str;
}

%token <str>id mc_langage mc_var mc_bgn mc_end mc_whl mc_if mc_int mc_bool mc_float mc_const mc_fnc mc_return 
%token col scol com egg plus mult dvd minus
%token inf infeg supp suppeg diff paro parf aff
%token <entier>integers
%token <flottant>reels

%%

S: mc_langage id mc_var Listdec mc_bgn ListInst mc_end {printf("syntaxe correcte\n"); YYACCEPT;};

Listdec: ListDV ListDM | ListDV | ListDM;

ListDV: DecV ListDV | DecV

DecV: ListeId col Type scol {
                                afd(sauvType, numLigne);
                            }
    | ListeCst col mc_const Type scol {
                                        afc(sauvType, numLigne); if(strcpy(sauvType, typecnst) != 0){printf("erreur semantique : non compatibilite de type a la ligne %d\n", numLigne);}
                                    }
    | ListeId col mc_const Type scol {afc(sauvType, numLigne);};
ListDM: Methode ListDM | Methode;

Methode: Type mc_fnc id mc_var ListDV mc_bgn ListInst mc_return id scol mc_end;

Type: mc_int {strcpy(sauvType, $1);}| mc_float {strcpy(sauvType, $1);}| mc_bool {strcpy(sauvType, $1);};

ListeId: id com ListeId {ajouterIdNom($1);}| id {ajouterIdNom($1);};

ListeCst: id aff cnst com ListeCst {InitConst($1);ajouterIdNom($1);}| id aff cnst {InitConst($1);ajouterIdNom($1);};

cnst: integers {strcpy(typecnst, "INT");} | reels {strcpy(typecnst, "FLOAT");}

ListInst: Inst ListInst | Inst;

Inst: Affectation | Boucle | Condition;

Affectation: id aff EXP scol    {nom($1, uno);
                                if(doubleDeclaration($1) == 0)
                                {
                                    printf("erreur semantique a la ligne %d :  l'idf %s n'a pas ete declare\n", numLigne, $1);};
                                    if(ConstanteInitialise($1)==1)
                                    {
                                        printf("erreur semantique: changement de la valeur de la constante %s a la ligne %d\n", $1, numLigne);
                                    }
                                } 

EXP: id OPR EXP {nom($1, uno);
                if(doubleDeclaration($1) == 0){
                    printf("erreur semantique a la ligne %d : l'idf %s n'a pas ete declare\n", numLigne, $1);
                    }else {
                        typee(uno, tp1);
                        if(strcmp(tp1, tp2)!=0){
                            printf("erreur semantique a la ligne %d: %s et %s ne sont pas du meme type\n", numLigne, $1);
                        };
                    }
                };
    | cnst OPR EXP 
    | id {nom($1, dos); if(strcmp($1, dos) != 0){printf("erreur semantique a la ligne %d: l'idf %s n'a pas ete declare", numLigne, $1); }else{typee($1, tp2);}}
    | cnst ;

OPR: plus {codeOPR=1;}| minus {codeOPR=2;}| mult {codeOPR=3;}| dvd {codeOPR=4;}

Condition:  mc_if paro Cond parf mc_bgn ListInst mc_end;

Boucle:     mc_whl paro Cond parf mc_bgn ListInst mc_end;

Cond: id CMP Cond | cnst CMP Cond | id | cnst;

CMP: supp | suppeg | inf | infeg | diff | egg;



%%

main () 
{
inittab();
init(); 
printf("C'est le compilateur de langage MiniAlgo: \n");
yyparse();
afficher();
}
yywrap()
{}

int yyerror ( char*  msg )  
{
printf("Errreur syntaxique a la ligne %d\n",numLigne);

}