#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PERSOS 20
#define MAX_TECHS 3
#define MAX_STR 50

typedef struct {
    char nom[MAX_STR];
    char type[MAX_STR];
    int puissance;
    int portee;
    int tours;
} Technique;

typedef struct {
    char nom[MAX_STR];
    char element[MAX_STR];
    int pv_max;
    int attaque;
    int defense;
    int agilite;
    int vitesse;
    Technique techniques[MAX_TECHS];
} Personnage;

// Supprimer le saut de ligne à la fin d'une chaîne
void supprimer_saut_ligne(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

int main() {
    FILE *fichier = fopen("personnages.txt", "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    Personnage persos[MAX_PERSOS];
    int nb_persos = 0;
    char ligne[100];

    while (fgets(ligne, sizeof(ligne), fichier) && nb_persos < MAX_PERSOS) {
        Personnage p;

        // Lecture des chaînes avec fgets + sscanf
        sscanf(ligne, "Nom: %[^\n]", p.nom);

        fgets(ligne, sizeof(ligne), fichier);
        sscanf(ligne, "Element: %[^\n]", p.element);

        // Lecture directe des entiers avec fscanf
        fscanf(fichier, "PV_max: %d\n", &p.pv_max);
        fscanf(fichier, "Attaque: %d\n", &p.attaque);
        fscanf(fichier, "Defense: %d\n", &p.defense);
        fscanf(fichier, "Agilite: %d\n", &p.agilite);
        fscanf(fichier, "Vitesse: %d\n", &p.vitesse);

        for (int i = 0; i < MAX_TECHS; i++) {
            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Technique: %[^\n]", p.techniques[i].nom);

            fgets(ligne, sizeof(ligne), fichier);
            sscanf(ligne, "Type: %[^\n]", p.techniques[i].type);

            // Les valeurs numériques avec fscanf
            fscanf(fichier, "Puissance: %d\n", &p.techniques[i].puissance);
            fscanf(fichier, "Portée: %d\n", &p.techniques[i].portee);
            fscanf(fichier, "Tours: %d\n", &p.techniques[i].tours);
        }

        persos[nb_persos++] = p;
    }

    fclose(fichier);

    return 0;
}
