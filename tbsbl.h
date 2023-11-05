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
    strcpy(ts->NomEntite, "");
    strcpy(ts->TypeEntite, "");
    strcpy(ts->CodeEntite, "");
    ts->constant = 0;
    ts->constInit = 0;
    ts->svt = NULL;
    //ts->svt = malloc(1 * sizeof(TypeTS));
}

void nv(TypeTS **ha){
    *ha = malloc(1 * sizeof(TypeTS));
    strcpy((*ha)->NomEntite, "");
    strcpy((*ha)->TypeEntite, "");
    strcpy((*ha)->CodeEntite, "");
    (*ha)->constant = 0;
    (*ha)->constInit = 0;
    (*ha)->svt = NULL;
}




//la fonction recherche
TypeTS* recherche(char entite[]){
    TypeTS *target;
    target = ts;
    bool stop = false;
    while(!stop){
        if(strcmp(target->NomEntite, "") == 0){
            stop = true;
        }else if(strcmp(target->NomEntite,entite)==0){
            stop = true;
        }else if(target-> svt != NULL){
            target = target->svt;
        }else{
            stop = true;
        }
    }
    return target;
}


void typee(char entite[], char *rslt){
    TypeTS *result = recherche(entite);
    strcpy(rslt, result->TypeEntite);
    
}

void nom(char entite[], char *test){
    TypeTS *result = recherche(entite);
    strcpy(test, result->NomEntite);
}

//une fontion qui va inserer les entites de programme dans la table de symbole
void inserer(char entite[], char code[])
{
    TypeTS *search = recherche(entite);
    TypeTS *nouveau;
    nv(&nouveau);
    bool stop = false;
    if(strcmp(entite, search->NomEntite) !=0)
    {
        while(!stop)
        {
            if(strcmp(search->NomEntite, "")== 0)
            {
                strcpy(search->NomEntite, entite);
                strcpy(search->CodeEntite, code);
                search->svt = nouveau;
                stop = true;
            }else if(search->svt == NULL)
            {
                strcpy(nouveau->NomEntite, entite);
                strcpy(nouveau->CodeEntite, code);
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
	printf("\n/********************************Table des symboles******************************/\n");
	printf("_________________________________________________________________________________\n");
	printf("\t| NomEntite |  CodeEntite  |  TypeEntite   | constante |   initConst \n");
	printf("_________________________________________________________________________________\n");
	TypeTS *show;
	show = ts;
	while(show != NULL){
		printf("\t|%10s |%12s  |%12s   | %7d   |%12d\n",show->NomEntite,show->CodeEntite,show->TypeEntite,show->constant,show->constInit);
		show = show->svt;
    }
}



// fonction qui change le type d'une etite une fois il va etre reconu dans la syntaxe 

void insererType(char entite[], char type[]){
    TypeTS *ins = recherche(entite);
    char tmp[50];
    nom(entite, tmp);
    if(strcmp(tmp, entite)==0){
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
    if(strcmp(cnst->NomEntite, entite)== 0){
        cnst->constInit = 1;
    }
}
///////////////////////////////////////////////////
typedef struct tablenames {
char name[20];
char tp[50];
struct tablenames *svt;
} TN;

TN *tblids;

void inittab(){
    tblids = (TN*) malloc(1 * sizeof(TN));
    strcpy(tblids->name, "");
    strcpy(tblids->tp, "");
    tblids->svt = NULL;
}

void nt(TN **newtab){
    (*newtab) = malloc(1 * sizeof(TN));
    strcpy((*newtab)->name, "");
    strcpy((*newtab)->tp, "");
    (*newtab)->svt = NULL;
}

void push(char nom[]){
    if(strcmp(tblids->name, "") != 0){
        TN *nouveau;
        nt(&nouveau);
        strcpy(nouveau->name, nom);
        nouveau->svt = tblids;
        tblids = nouveau;
    }else{
        strcpy(tblids->name, nom);
    }
}

void pop(){
    
    if(tblids->svt != NULL){
        TN *remove;
        remove = tblids;
        tblids = tblids->svt;
        remove->svt = NULL;
        free(remove);
    }else{
        strcpy(tblids->name, "");
        strcpy(tblids->tp, "");
    }
}

TN* parcours( char nom[]){
    TN *pcr = tblids;
    bool stop = false;
    while(!stop){
        if(pcr->svt == NULL){
            stop = true;
        }else if(strcmp(pcr->name, nom) == 0){
            stop = true;
        }else{
            pcr = pcr->svt;
        }
    }
    return pcr;
}
void ajouterIdNom(char nom[]){
    TN *target = parcours(nom);
    if(strcmp(nom, target->name) != 0){
        push(nom);
    }
}

void getname(char nom[], char *cp){
    TN *target = parcours(nom);
    if(strcmp(target->name, nom) == 0){
        strcpy(cp, target->name);
    }
}

void get(char *nom){
    strcpy(nom, tblids->name);
}


void afd(char type[], int numLigne){
    TN *pcr;
    pcr = tblids;
    while(pcr != NULL){
        if(doubleDeclaration(pcr->name) == 0){
            insererType(pcr->name, type);
        }else{
            printf("erreur semantique: double declaration de %s, a la ligne %d\n", pcr->name, numLigne);
        }
        pcr = pcr->svt;
        pop();
    }
}

void afc(char type[], int numLigne){
    TN *pcr;
    pcr = tblids;
    while(pcr != NULL){
        SauvInfoConst(pcr->name);
        if(doubleDeclaration(pcr->name) == 0){
            insererType(pcr->name, type);
        }else{
            printf("erreur semantique: double declaration de %s, a la ligne %d\n", pcr->name, numLigne);
        }
        pcr = pcr->svt;
        pop();
    }
}



