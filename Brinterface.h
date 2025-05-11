#include "type.h" // toutes les bibliothèques dans type.h
#define MAX_PERSOS 100 
//pour le main 


//pour les couleurs dans l'interface
#define RESET   "\033[0m"
#define ROUGE     "\033[1;31m"
#define VERT   "\033[1;32m"
#define JAUNE  "\033[1;33m"
#define BLEU    "\033[1;34m"
#define NOIR   "\033[1;30m"
#define CYAN   "\033[1;36m"
#define BLANC  "\033[1;37m"

//protoype des fonctions 
void debutJeu();
int choixPrincipal();
int choixModeJeu();
int chargerCombattants(const char* nom_fichier, Combattant* persos);
void selectionnerEquipe(Combattant* disponibles, int* taille_dispo, Combattant* equipe, int taille_equipe);
void afficheCombattants(Combattant* tab, int nb_combattants);
void afficherEquipe(Combattant* equipe, int taille);
void choisirEquipe(Combattant* tous, int nb_combattants, Combattant* equipe, int taille);
void choisirEquipeAleatoire(Combattant* tous, int nb_combattants, Combattant* equipe, int taille);
void selectionnerEquipesJoueurs(Combattant* disponibles, int* taille_dispo, Combattant* equipeJ1, Combattant* equipeJ2, int taille_equipe);
const char* getEffetNom(EffetType effet);
