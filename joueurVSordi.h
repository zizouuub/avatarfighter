#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combat.h"  // Pour attaque_normale
#include "effets.h"  // Pour appliquerEffetElementaire

int choisir_action_joueur() ; 

int choisir_action_ia(Combattant ia);

// Simule l’utilisation d’une technique spéciale (exemple simplifié)
void utiliser_technique(Combattant *source, Combattant *cible);

void tour(Combattant *joueur, Combattant *ia, int est_joueur);

void lancer_combat(Combattant *joueur, Combattant *ia);
