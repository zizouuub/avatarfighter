#include "type.h" 

// Initialise un combattant avec des valeurs par défaut
void initialiser_combattant(Combattant *c);

// Applique un effet élémentaire à une cible 
void appliquerEffetElementaire(Combattant *cible, TechniqueSpeciale effet);

// Applique les dégâts ou conséquences d'un effet 
void appliquerdegats(Combattant *c);

// Met à jour la durée des effets
void mettreAJourEffets(Combattant *combattant, TechniqueSpeciale *tech);

// Retourne un multiplicateur de dégâts selon l’effet 
float multiplicateur(EffetType e);

