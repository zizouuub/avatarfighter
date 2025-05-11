#ifndef JOUEURVSORDI_H
#define JOUEURVSORDI_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combat.h"    // pour attaque_normale
#include "effets.h"    // pour appliquerEffetElementaire

// Niveau de difficulté de l'IA (0=noob, 1=facile, 2=moyen)
int niveau_ia;

// Affiche l'état d'un combattant
void afficher_etat_combattant(Combattant *c);

// Choisit une cible aléatoire parmi les combattants non-KO
Combattant* choisir_cible_aleatoire(Combattant *equipe, int taille);

// Choisit la cible avec le moins de PV
Combattant* choisir_cible_faible(Combattant *equipe, int taille);

// Choisit une cible selon la stratégie de l'IA
Combattant* choisir_cible_attaque(Combattant *equipe, int taille);

// Choisit un allié à soigner (celui avec le plus bas pourcentage de PV)
Combattant* choisir_cible_soin(Combattant *equipe, int taille);

// Vérifie si une technique est disponible
int technique_disponible(Combattant *c);

// Choisit l'action pour l'IA
int choisir_action_ia(Combattant *ia);

// Gère le tour d'un combattant (joueur ou IA)
void tour(Combattant *acteur, Combattant *equipe_ennemie, int taille_ennemie, Combattant *equipe_alliee, int taille_alliee, int est_joueur);

// Met à jour les temps de recharge des techniques
void maj_recharge(Combattant *c);

// Fonction pour choisir une cible ennemie
int choisir_cible1(Combattant *equipe, int taille);

// Fonction principale pour lancer le combat
void lancer_combat(Combattant *equipe_joueur, int taille_joueur, Combattant *equipe_ia, int taille_ia);

#endif

