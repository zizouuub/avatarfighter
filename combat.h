#ifndef COMBAT_H
#define COMBAT_H

#include <stdio.h>
#include "effets.h"
#include "type.h" 
#include <string.h>
#include <time.h>

// Fonction à appeler après que le personnage a joué
void definir_prochain_tour(Combattant *perso);

int choisir_combattant(Combattant *equipe, int taille) ;

int choisir_cible(Combattant *equipe, int taille);

void maj_tours_combattants(Combattant *equipe, int taille);

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
