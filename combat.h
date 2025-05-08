#ifndef COMBAT_H
#define COMBAT_H

#include <stdio.h>
#include "effets.h"
#include "type.h" 

// Initialise les membres de chaque équipe
void initialiser_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2);

// Vérifie si une équipe est complètement KO
int equipe_est_KO(Combattant *equipe, int taille);

// Attaque normale d’un combattant sur une cible
void attaque_normale(Combattant *attaquant, Combattant *cible);

// Gère le tour d’un combattant contre une équipe adverse
void effectuer_tour(Combattant *joueur, Combattant *adversaires, int taille_adversaires);

// Gère toute la boucle de combat entre deux équipes
void boucle_de_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2);

#endif
