#ifndef COMBAT_H
#define COMBAT_H

#include <stdio.h>
#include "effets.h"
#include "type.h" 
#include <string.h>
#include <time.h>

// Initialise les données des deux équipes (PV, effets, etc.)
void initialiser_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2);

// Affiche une introduction au combat (et initialise le random)
void initialiser_comb(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2);

// -------------------------
// MISE À JOUR DES COMBATTANTS
// -------------------------

// Met à jour les compteurs de tour et de recharge des techniques
void maj_tours_combattants(Combattant *equipe, int taille);

// Définit quand le personnage pourra rejouer
void definir_prochain_tour(Combattant *perso);

// -------------------------
// CHOIX DU JOUEUR
// -------------------------

// Permet de choisir un combattant actif parmi l'équipe
int choisir_combattant(Combattant *equipe, int taille);

// Permet de choisir une cible parmi l'équipe adverse
int choisir_cible(Combattant *equipe, int taille);

// -------------------------
// ACTIONS EN COMBAT
// -------------------------

// Effectue une attaque normale (calcul de dégâts, etc.)
void attaque_normale(Combattant *attaquant, Combattant *cible);

// Utilise une technique spéciale (attaque, soin, effet, etc.)
void utiliserTechnique(Combattant *attaquant, Combattant *cible, TechniqueSpeciale *tech);

// Réalise un tour pour un combattant (choisit une action)
void effectuer_tour(Combattant *joueur, Combattant *adversaires, int taille_adversaires);

// -------------------------
// CONTRÔLE DU COMBAT
// -------------------------

// Vérifie si toute une équipe est KO
int equipe_est_KO(Combattant *equipe, int taille);

// Gère toute la boucle du combat entre deux équipes
void boucle_de_combat(Combattant *equipe1, int taille1, Combattant *equipe2, int taille2);

#endif // COMBAT_H
