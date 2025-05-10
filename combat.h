#ifndef COMBAT_H
#define COMBAT_H

#include <stdio.h>
#include "effets.h"
#include "type.h" 
#include "time.h"


// Fonction pour initialiser le combat
void initialiser_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2);

// Fonction pour vérifier si une équipe est KO
int equipe_est_KO(Combattant *equipe, int taille);

// Fonction pour calculer les dégâts d'une attaque normale  
void attaque_normale(Combattant *attaquant, Combattant *cible);

// Fonction pour utiliser une technique spéciale
void utiliserTechnique(Combattant *attaquant, Combattant *cible, TechniqueSpeciale *tech);

// Fonction pour choisir une action (attaquer ou utiliser une technique)
void effectuer_tour(Combattant *joueur, Combattant *adversaires, int taille_adversaires);

void initialiser_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2) {

// Fonction pour gérer le combat entre deux équipes
void boucle_de_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2);

#endif
