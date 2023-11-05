#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//structure de la table de symbole
typedef struct tas {
char NomEntite[20];
char CodeEntite[20];
char TypeEntite[20];
int constant;
int constInit;
struct tas *svt;
} TypeTS;


//initiation d'une LLC qui va contenir les elements de la table de symbole
//creation de la tete
TypeTS *ts;
//initialisation de la tete
void init(){
    ts = (TypeTS*) malloc(1 * sizeof(TypeTS));
    ts->svt = NULL;
    //ts->svt = malloc(1 * sizeof(TypeTS));
}





//la fonction recherche
TypeTS* recherche(char entite[]){
    TypeTS *target;
    target = ts;
    bool stop = false;
    if(strcmp(target->NomEntite,entite)!=0){
        while(!stop){
            if(strcmp(target->NomEntite, "") == 0){
                stop = true;
            }else if(target-> svt != NULL){
                target = target->svt;
            }else{
                stop = true;
            }
        }
    }
    return target;
}


char * trouver(char entite[]){
    TypeTS *result = recherche(entite);
    return result->TypeEntite;
}

char * comparer(char entite[]){
    TypeTS *result = recherche(entite);
    char *test;
    strcpy(test, result->NomEntite);
    return test;
}

//une fontion qui va inserer les entites du programme dans la table de symbole
void inserer(char entite[], char code[])
{
    TypeTS *search = recherche(entite);
    TypeTS *nouveau = (TypeTS*)malloc(1 * sizeof(TypeTS));
    nouveau->svt = NULL;
    strcpy(nouveau->NomEntite, entite);
    strcpy(nouveau->CodeEntite, code);
    bool stop = false;
    if(strcmp(entite, search->NomEntite) !=0)
    {
        while(!stop)
        {
            if(strcmp(search->NomEntite, "")== 0)
            {
                strcpy(search->NomEntite, entite);
                stop = true;
            }else if(search->svt == NULL)
            {
                search->svt = nouveau;
                stop = true;
            }else
            {
                search = search->svt;
            }
        }
    }
}


//une fonction pour afficher la table de symbole
void afficher (){
	printf("\n/***************Table des symboles ******************/\n");
	printf("_________________________________________________________________________\n");
	printf("\t| NomEntite |  CodeEntite  |  TypeEntite | constante    |initConst \n");
	printf("______________________________________________________________________\n");
	TypeTS *show;
	show = ts;
	while(show != NULL){
		printf("\t|%10s |%12s  |%12s   | %12d   |%12d\n",show->NomEntite,show->CodeEntite,show->TypeEntite,show->constant,show->constInit);
		show = show->svt;
    }
}



// fonction qui change le type d'une etite une fois il va etre reconu dans la syntaxe 

void insererType(char entite[], char type[]){
    TypeTS *ins = recherche(entite);
    if(strcmp(ins->NomEntite, entite)==0){
        strcpy(ins->TypeEntite, type);
    }
}

////Les routines semantiques


int doubleDeclaration(char entite[]){
    TypeTS *pos = recherche(entite);
    if(strcmp(pos->TypeEntite, "")==0){
        return 0;
    }else{
        return 1;
    }
}

void SauvInfoConst(char entite[]){
    TypeTS *info = recherche(entite);
    info->constant = 1;
}

int ConstanteInitialise(char entite[]){
    TypeTS *cnst = recherche(entite);
    if(cnst->constant ==1 && cnst->constInit == 1){
        return 1;
    }else{
        return 0;
    }
}

void InitConst(char entite[]){
    TypeTS *cnst = recherche(entite);
    cnst->constInit = 1;
}

