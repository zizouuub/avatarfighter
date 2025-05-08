
#include <stdio.h> 
#include <string.h>
#define MAX_NOM 50 
#define MAX_DESC 150 
#define NB_EFFETS 13 // Nombre d'effets possibles



typedef enum {
    FEU, EAU, TERRE, AIR
} Element;

typedef enum {
    AUCUN, ATTAQUE, DEFENSE, AGILITE, STUN, GEL, SOIN, BRULURE, CONTRE, POISON
} EffetType;


typedef struct {
    char nom[MAX_NOM];
    char description[MAX_DESC];
    int valeur;
    int duree; // en tours
    int recharge;
    int tours_restant; // à initialiser à 0 à la lecture
} TechniqueSpeciale;


typedef struct {
    char nom[MAX_NOM];
    Element element;
    int pv_max;
    int pv;
    int attaque;
    int defense;
    int agilite;
    int vitesse;
    int est_KO;
    EffetType effets[NB_EFFETS]; 
    int duree_effet[NB_EFFETS]; // durée de l'effet en cours
    TechniqueSpeciale techniques[3]; //fixe à 3 techniques
} Combattant;
