#define MAX_STR 50
#define MAX_TECHS 3  // Nombre de techniques par personnage

// Enumération pour les éléments
typedef enum {
    FEU, EAU, TERRE, AIR
} Element;

// Enumération pour les types d'effets d'une technique
typedef enum {
    AUCUN, ATTAQUE, DEFENSE, AGILITE, STUN, GEL, SOIN, BRULURE, CONTRE, POISON
} EffetType;

// Structure pour représenter une technique
typedef struct {
    char nom[MAX_STR];     // Nom de la technique
    char type[MAX_STR];    // Type de la technique (attaque, défense, etc.)
    int puissance;         // Puissance de la technique
    int portee;            // Portée de la technique
    int tours;             // Nombre de tours de l'effet de la technique
} TechniqueSpeciale;

// Structure pour représenter un personnage
typedef struct {
    char nom[MAX_STR];     // Nom du personnage
    char element[MAX_STR]; // Élément (Feu, Eau, Terre, Air)
    int pv_max;            // Points de vie maximum
    int attaque;           // Attaque
    int defense;           // Défense
    int agilite;           // Agilité
    int vitesse;           // Vitesse
    Technique techniques[MAX_TECHS]; // Techniques spéciales
} Combattant;
