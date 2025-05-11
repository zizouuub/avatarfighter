#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combat.h"  // Pour attaque_normale
#include "effets.h"  // Pour appliquerEffetElementaire

#ifndef JOUEURVSORDI_H
#define JOUEURVSORDI_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combat.h"    // pour attaque_normale
#include "effets.h"    // pour appliquerEffetElementaire

// Niveau d'IA global
int niveau_ia;

// Sélection de l'action pour le joueur (1 = attaque normale, 2 = technique spéciale)
int choisir_action_joueur();

// Sélection de l'action pour l'IA
int choisir_action_ia(Combattant *ia);

// Choix d'une cible (par index) pour le joueur
int choisir_cible(Combattant *equipe, int taille);

// Choix d'une cible aléatoire pour l'IA
Combattant* choisir_cible_aleatoire(Combattant *equipe, int taille);

// Choix de la cible la plus faible (PV les plus bas)
Combattant* choisir_cible_faible(Combattant *equipe, int taille);

// Choix intelligent de la cible selon le niveau de l'IA
Combattant* choisir_cible_attaque(Combattant *equipe, int taille);

// Choix de la cible à soigner
Combattant* choisir_cible_soin(Combattant *equipe, int taille);

// Vérifie si une technique spéciale est disponible
int technique_disponible(Combattant *c);

// Applique une attaque élémentaire (attaque normale + effet selon l'élément)
void attaque_elementaire(Combattant *attaquant, Combattant *cible);

// Utilise une technique spéciale
void utiliser_technique(Combattant *source, Combattant *cible, int indice);

// Exécute un tour pour un combattant (joueur ou IA)
void tour(Combattant *acteur, Combattant *equipe_ennemie, int taille_ennemie, Combattant *equipe_alliee, int taille_alliee, int est_joueur);

// Vérifie si une équipe est complètement K.O.
int equipe_est_KO(Combattant *equipe, int taille);

// Met à jour le temps de recharge des techniques spéciales
void maj_recharge(Combattant *c);

// Démarre le combat entre deux équipes
void lancer_combat(Combattant *equipe_joueur, int taille_joueur, Combattant *equipe_ia, int taille_ia);

#endif
