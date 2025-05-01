#include "types.h" 
#include <stdio.h> 
#include "effets.h"
#include <string.h>
#define MAX_NOM 50 // taille nom
#define MAX_DESC 150 // taille description


typedef enum {
    FEU, EAU, TERRE, AIR
} Element;

typedef enum {
    ATTAQUE, DEFENSE, PV, AGILITE, VITESSE, STUN, SOIN, BRULURE, CONTRE, SILENCE
} EffetType;



typedef struct {
    char nom[MAX_NOM];
    char description[MAX_DESC];
    EffetType effet; 
    int valeur;
    int duree; // en tours
    int recharge;
    int tours_restant; // à initialiser à 0 à la lecture
} TechniqueSpeciale;


typedef struct {
    char nom[MAX_NOM];
    Element element;
    int pv_max;
    int attaque;
    int defense;
    int agilite;
    int vitesse;
    TechniqueSpeciale techniques[3]; //fixe à 3 techniques
} Combattant;
