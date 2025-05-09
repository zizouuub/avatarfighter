#include "type.h" 

// Initialise un combattant avec des valeurs par défaut
void initialiser_combattant(Combattant *c);

// Applique un effet élémentaire à une cible 
void appliquerEffetElementaire(Combattant *cible, TechniqueSpeciale effet);

// Applique les dégâts ou conséquences d'un effet 
void appliquerDegats(Combattant *c);

// Met à jour la durée des effets
void mettreAJourEffets(Combattant *combattant, TechniqueSpeciale *tech);

// Retourne un multiplicateur de dégâts selon l’effet 
float multiplicateur(Element attaquant, Element defenseur);

// gel un combattant
int est_incapacite(Combattant *c);

// Fonction pour calculer les dégâts d'une attaque
int calculerDegats(Combattant *attaquant, TechniqueSpeciale *tech);

// Fonction qui utilise l'agilité pour déterminer si l'esquive réussit
int tentativeEsquive(Combattant *defenseur);
