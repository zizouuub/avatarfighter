#ifndef EFFETS_H
#define EFFETS_H

#include "type.h"
#include <stdio.h>
#include <stdlib.h>

// Initialise un combattant avec des valeurs par défaut
void initialiser_combattant(Combattant *c);

// Applique un effet élémentaire à une cible 
void appliquerEffetElementaire(Combattant *cible, TechniqueSpeciale effet);

// Applique les dégâts ou conséquences d'un effet 
void appliquerDegats(Combattant *c);

// Met à jour la durée des effets
void mettreAJourEffets(Combattant *combattant, TechniqueSpeciale *tech);

// Retourne un multiplicateur de dégâts selon les éléments
float multiplicateur(Element attaquant, Element defenseur);

// Vérifie si le combattant est gelé ou étourdi
int est_incapacite(Combattant *c);

// Calcule les dégâts d'une attaque
int calculerDegats(Combattant *attaquant, TechniqueSpeciale *tech);

// Détermine si une esquive réussit
int tentativeEsquive(Combattant *defenseur);

#endif

