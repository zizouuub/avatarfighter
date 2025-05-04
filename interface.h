#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <stdlib.h>
#include <time.h>

int afficheMenuPrincipal();



typedef enum {
    FEU,
    EAU,
    TERRE,
    AIR
} Element;

#define RESET  "\033[0m"
#define ROUGE  "\033[1;31m"
#define VERT   "\033[1;32m"
#define JAUNE  "\033[1;33m"
#define BLEU   "\033[1;34m"
#define NOIR   "\033[1;30m"
#define CYAN   "\033[1;36m"


typedef struct {
    char nom[30];
char titre[500];
    Element element;
    int pv;
    int attaque;
    //float vitesse;
    int defense;
} Combattant;

void selectionnerEquipe(Combattant* disponibles, int* taille_dispo, Combattant* equipe, int taille_equipe);
void afficherListeCombattants(Combattant* tab, int taille);
