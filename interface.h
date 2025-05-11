
#ifndef INTERFACE_H
#define INTERFACE_H

#include "type.h"

//pour les couleurs dans l'interface
#define RESET   "\033[0m"
#define ROUGE     "\033[1;31m"
#define VERT   "\033[1;32m"
#define JAUNE  "\033[1;33m"
#define BLEU    "\033[1;34m"
#define NOIR   "\033[1;30m"
#define CYAN   "\033[1;36m"
#define BLANC  "\033[1;37m"
#define VIOLET "\033[1;35m"
#define BEIGE "\033[38;5;230m"

//protoype des fonctions 
void clearScreen(void);

int lireChoix(int min, int max);

void debutJeu(void);

int choixPrincipal(void);

int choixModeJeu(void);

void selectionnerEquipe(Combattant* disponibles, int* taille_dispo, 

Combattant* equipe, int taille_equipe);

void afficheCombattants(Combattant* combattant, int nbCombattants);

void afficherEquipe(Combattant* equipe, int taille_equipe);

void choisirEquipeAleatoire(Combattant* disponibles, int 
taille_dispo, Combattant* equipe, int taille_equipe);

void selectionnerEquipesJoueurs(Combattant* disponibles, int* taille_dispo, Combattant* equipeJ1, Combattant* equipeJ2, int taille_equipe);

const char* getEffetNom(EffetType effet);

const char* getEmoji(Element element);

const char* getElementNom(Element elem);

void afficheTousLesCombattants(Combattant* combattants, int nbCombattants);

void jeuCommence(void);

void finJeu(void);

#endif
