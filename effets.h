#include "types.h" // en supposant que Lina a appelé comme ca le fichier qui contient les structures pour les elements, effet spectiaux, combattants

// Applique un effet élémentaire à une cible 
void appliquerEffetElementaire(Combattant *cible, Effetspecial effet);

// Applique les dégâts ou conséquences d'un effet 
void appliquerdegats(Combattant *c);

// Met à jour la durée des effets
void mettreAJourEffets(Combattant *c);

// Retourne un multiplicateur de dégâts selon l’effet 
float multiplicateur(Effet e);
