#ifndef TYPE_H
#define TYPE_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define MAX_STR 80
#define MAX_TECHS 3      // Nombre de techniques par personnage
#define NB_EFFETS 8     // Nombre d'effets possibles



// Enumération pour les éléments
typedef enum {
    FEU, EAU, TERRE, AIR
} Element;

// Enumération pour les types d'effets d'une technique
typedef enum {
    AUCUN, ATTAQUE, DEFENSE, AGILITE, STUN, GEL, SOIN, BRULURE, CONTRE, POISON
} EffetType;

// Structure pour représenter une technique spéciale
typedef struct {
    char nom[MAX_STR];       // Nom de la technique
    char type[MAX_STR];      // Type (attaque, défense...)
    int puissance;           // Dégâts ou valeur de l'effet
    int portee;              // Portée
    int tours;               // Durée de l'effet
    EffetType effet;         // Type d'effet appliqué (ex : POISON, BRULURE...)
    char description[MAX_STR];
} TechniqueSpeciale;

// Structure pour représenter un combattant
typedef struct {
    char nom[MAX_STR];                       // Nom
    Element element;                         // Élément (Feu, Eau...)
    float pv_max;                              // PV max
    float pv;                                  // PV actuels
    int attaque;                             // Attaque
    int defense;                             // Défense
    int agilite;                             // Agilité
    int vitesse;                             // Vitesse
    int est_KO;                              // KO = 1 si mort
    EffetType effets[NB_EFFETS];             // Liste des effets actifs
    int duree_effet[NB_EFFETS];              // Durées des effets
    TechniqueSpeciale techniques[MAX_TECHS]; // Techniques spéciales
    int temps_recharge[MAX_TECHS];           // nombre de tours restants avant de réutiliser la technique
    int prochain_tour;                       // nombre de tours à attendre avant de rejouer (lié à vitesse)
} Combattant;

#endif // TYPE_H
