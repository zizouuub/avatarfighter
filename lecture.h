#ifndef LECTURE_H
#define LECTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "effets.h"

#define MAX_PERSOS 10


// Charge les combattants depuis un fichier
int charger_combattants(const char *nom_fichier, Combattant *persos, int max_persos);

// Convertit une chaîne en EffetType
EffetType convertirEffetType(const char *effetStr);

// Supprime un \n à la fin d'une ligne
void supprimer_saut_ligne(char *str);

#endif
