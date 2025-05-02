#include <string.h>
#include <stdio.h>
#define MAX_NOM 50 
#define MAX_DESC 150 

typedef enum {
    FEU, 
    EAU,
    TERRE,
    AIR
} Element;

typedef enum {
    ATTAQUE, 
    DEFENSE, 
    PV, 
    AGILITE, 
    STUN, 
    GEL,
    POISON,
    SOIN, 
    BRULURE, 
    CONTRE, 
} EffetType;



typedef struct {
    char nom[MAX_NOM];
    char description[MAX_DESC];
    EffetType effet; 
    int valeur;
    int nbeffet =13; // nombre d'effets possibles
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
